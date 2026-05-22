// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Rogue : ModuleRules
{
	public Rogue(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.Add("Rogue");
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "UMG",
			"StateTreeModule", "GameplayStateTreeModule", "GameplayTags",
			"AIModule", "NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"EnhancedInput", "Niagara"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
