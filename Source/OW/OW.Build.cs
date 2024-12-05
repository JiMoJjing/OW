// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OW : ModuleRules
{
	public OW(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Niagara", "SlateCore", "AnimGraphRuntime", "AIModule", "NavigationSystem", "GameplayTasks" });
		
		//PrivateDependencyModuleNames.AddRange(new string[] { "AnimGraphRuntime", "AIModule" });
	}
}
