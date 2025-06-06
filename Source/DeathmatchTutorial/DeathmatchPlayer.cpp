// © Ville Ruokonen 2025


#include "DeathmatchPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ADeathmatchPlayer::ADeathmatchPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Hide mesh for owner
	GetMesh()->SetOwnerNoSee(true);

	Camera1P = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera1P"));
	Camera1P->SetupAttachment(GetCapsuleComponent());
	Camera1P->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	Camera1P->bUsePawnControlRotation = true;

	Health = 100.0f;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ADeathmatchPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADeathmatchPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADeathmatchPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADeathmatchPlayer::Shoot()
{
	FVector cameraPos = Camera1P->GetComponentLocation();
	FVector end = cameraPos + Camera1P->GetForwardVector() * 1000.0f;

	FHitResult hit;

	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(hit, cameraPos, end, ECC_Visibility, params))
	{
		if (hit.GetActor() == this)
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "Player hit himself. This shouldn't happen");
			return;
		}

		ServerShoot(cameraPos, end, hit.GetActor());
	}
}

void ADeathmatchPlayer::MulticastOnDeath_Implementation()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMovementComponent()->StopMovementImmediately();
}

void ADeathmatchPlayer::TakeDamage(float DamageAmount, AActor* DamageCauser)
{
	Health -= DamageAmount;

	if (Health <= 0)
	{
		Health = 0;
		bIsDead = true;

		ServerOnDeath(DamageCauser);

		MulticastOnDeath();
	}
}

void ADeathmatchPlayer::ServerShoot_Implementation(FVector ClientLineStart, FVector ClientLineEnd, AActor* ClientHitActor)
{
	FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	if (!GetWorld()->LineTraceSingleByChannel(hit, ClientLineStart, ClientLineEnd, ECC_Visibility, params))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No hit.");

		return;
	}

	AActor* hitActor = hit.GetActor();
	if (!hitActor)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No hit actor.");
		return;
	}

	if (ClientHitActor && hitActor != ClientHitActor)
	{
		if (GEngine)
		{
			FString debugMessage = FString::Printf(TEXT("ClientHitActor: %s, hitActor: %s"), 
				*ClientHitActor->GetName(), *hitActor->GetName());

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, debugMessage);
		}
		return;
	}

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Raycast validated!");

	if (ADeathmatchPlayer* Other = Cast<ADeathmatchPlayer>(hitActor))
	{
		Other->TakeDamage(1000.0f, this);
	}
}

