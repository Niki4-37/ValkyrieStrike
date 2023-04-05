// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ValkiriaStrike : ModuleRules
{
    public ValkiriaStrike(ReadOnlyTargetRules Target) : base(Target)
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
            "ValkiriaStrike/Public/Player",
            "ValkiriaStrike/Public/AI",
            "ValkiriaStrike/Public/AI/Service",
            "ValkiriaStrike/Public/Components",
            "ValkiriaStrike/Public/GameLevelsConfig",
            "ValkiriaStrike/Public/GameMenu",
            "ValkiriaStrike/Public/Weapon"
        });

        //PublicDefinitions.Add("HMD_MODULE_INCLUDED=1");
    }
}
