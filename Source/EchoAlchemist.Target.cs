// Copyright 2025 Voidzyy. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class EchoAlchemistTarget : TargetRules
{
	public EchoAlchemistTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "EchoAlchemist" } );
	}
}
