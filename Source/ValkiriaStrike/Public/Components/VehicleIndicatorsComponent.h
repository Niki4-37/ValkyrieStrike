// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponent.h"
#include "VehicleIndicatorsComponent.generated.h"

UCLASS()
class VALKIRIASTRIKE_API UVehicleIndicatorsComponent : public UHealthComponent
{
    GENERATED_BODY()

public:
    UVehicleIndicatorsComponent();
};
