// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/VehicleIndicatorsComponent.h"
#include "Net/UnrealNetwork.h"

UVehicleIndicatorsComponent::UVehicleIndicatorsComponent()
{
    SetIsReplicatedByDefault(true);
}

void UVehicleIndicatorsComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(MaxFuelValue > 0, TEXT("MaxFuelValue can't be less or equals 0!"));
    FuelValue = MaxFuelValue;  // Server?

    GetWorld()->GetTimerManager().SetTimer(FuelChangeTimer, this, &UVehicleIndicatorsComponent::ChangeFuelValue, 1.f, true);
}

void UVehicleIndicatorsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UVehicleIndicatorsComponent, FuelValue);
}

void UVehicleIndicatorsComponent::ChangeFuelValue()
{
    const float NewValue = --FuelValue;
    FuelValue = FMath::Clamp(NewValue, 0.f, MaxFuelValue);

    OnFuelValueChanged.Broadcast(FuelValue / MaxFuelValue);
}
