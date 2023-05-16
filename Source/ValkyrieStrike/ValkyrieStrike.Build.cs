// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ValkyrieStrike : ModuleRules
{
    public ValkyrieStrike(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "PhysXVehicles",
            "GameplayTasks",
            "Niagara",
            "NavigationSystem"
        });

        PublicIncludePaths.AddRange(new string[]
        {
            "ValkyrieStrike/Public/Player",
            "ValkyrieStrike/Public/AI",
            "ValkyrieStrike/Public/AI/Services",
            "ValkyrieStrike/Public/Components",
            "ValkyrieStrike/Public/GameLevelsConfig",
            "ValkyrieStrike/Public/GameMenu",
            "ValkyrieStrike/Public/Weapon"
        });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");

        //PublicDefinitions.Add("HMD_MODULE_INCLUDED=1");
    }
}
