// © Ville Ruokonen 2025

using UnrealBuildTool;
using System.Collections.Generic;

public class DeathmatchTutorialTarget : TargetRules
{
	public DeathmatchTutorialTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "DeathmatchTutorial" } );
	}
}
