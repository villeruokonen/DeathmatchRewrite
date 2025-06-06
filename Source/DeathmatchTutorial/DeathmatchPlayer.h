// © Ville Ruokonen 2025

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "DeathmatchPlayer.generated.h"

UCLASS()
class DEATHMATCHTUTORIAL_API ADeathmatchPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADeathmatchPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnDeath();

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UFUNCTION(BlueprintImplementableEvent)
	void ServerOnDeath(AActor* Killer);

private:
	void TakeDamage(float DamageAmount, AActor* DamageCauser);

	UFUNCTION(Server, Reliable)
	void ServerShoot(FVector ClientLineStart, FVector ClientLineEnd, AActor* ClientHitActor);

private:
	float						Health;

	bool						bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent*			Camera1P;

};
