// © Ville Ruokonen 2025

using UnrealBuildTool;
using System.Collections.Generic;

public class DeathmatchTutorialEditorTarget : TargetRules
{
	public DeathmatchTutorialEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "DeathmatchTutorial" } );
	}
}
