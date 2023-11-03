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
            "NavigationSystem",
            "OnlineSubsystem",
            "OnlineSubsystemUtils"
        });

        PublicIncludePaths.AddRange(new string[]
        {
            "ValkyrieStrike/Public/AI",
            "ValkyrieStrike/Public/AI/Services",
            "ValkyrieStrike/Public/Components",
            "ValkyrieStrike/Public/GameLevelsConfig",
            "ValkyrieStrike/Public/GameMenu",
            "ValkyrieStrike/Public/Interactables",
            "ValkyrieStrike/Public/Interfaces",
            "ValkyrieStrike/Public/LobbyMenu",
            "ValkyrieStrike/Public/Player",
            "ValkyrieStrike/Public/Subsystems",
            "ValkyrieStrike/Public/UI",
            "ValkyrieStrike/Public/Weapon"
        });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");

        //PublicDefinitions.Add("HMD_MODULE_INCLUDED=1");
    }
}
