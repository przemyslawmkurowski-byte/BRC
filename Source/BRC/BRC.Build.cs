// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BRC : ModuleRules
{
	public BRC(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"BRC",
			"BRC/Variant_Platforming",
			"BRC/Variant_Platforming/Animation",
			"BRC/Variant_Combat",
			"BRC/Variant_Combat/AI",
			"BRC/Variant_Combat/Animation",
			"BRC/Variant_Combat/Gameplay",
			"BRC/Variant_Combat/Interfaces",
			"BRC/Variant_Combat/UI",
			"BRC/Variant_SideScrolling",
			"BRC/Variant_SideScrolling/AI",
			"BRC/Variant_SideScrolling/Gameplay",
			"BRC/Variant_SideScrolling/Interfaces",
			"BRC/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
