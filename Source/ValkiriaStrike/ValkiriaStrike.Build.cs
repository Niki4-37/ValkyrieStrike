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

        //PublicIncludePaths.AddRange(new string[]
        //{
        //    "ValkiriaStrike/Public/Player"
        //});

        //PublicDefinitions.Add("HMD_MODULE_INCLUDED=1");
    }
}
