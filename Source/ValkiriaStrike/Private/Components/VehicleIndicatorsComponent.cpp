// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/VehicleIndicatorsComponent.h"
#include "Net/UnrealNetwork.h"

UVehicleIndicatorsComponent::UVehicleIndicatorsComponent()
{
    SetIsReplicatedByDefault(true);
}

void UVehicleIndicatorsComponent::AddCoins(int32 Value)
{
    Coins += Value;
    OnCoinsChanged.Broadcast(Coins);
}

bool UVehicleIndicatorsComponent::AddFuel(int32 Value)
{
    bool bCanChanged{true};
    if (FuelValue == MaxFuelValue)
    {
        bCanChanged = false;
    }

    FuelValue = FMath::Clamp(FuelValue + static_cast<float>(Value), 0.f, MaxFuelValue);
    OnFuelValueChanged.Broadcast(FuelValue / MaxFuelValue);
    return bCanChanged;
}

void UVehicleIndicatorsComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(MaxFuelValue > 0, TEXT("MaxFuelValue can't be less or equals 0!"));
    FuelValue = MaxFuelValue;  // Server?
    OnFuelValueChanged.Broadcast(FuelValue / MaxFuelValue);
    GetWorld()->GetTimerManager().SetTimer(FuelChangeTimer, this, &UVehicleIndicatorsComponent::SpendFuel, 1.f, true);
}

void UVehicleIndicatorsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UVehicleIndicatorsComponent, FuelValue);
    DOREPLIFETIME(UVehicleIndicatorsComponent, Coins);
}

void UVehicleIndicatorsComponent::SpendFuel()
{
    const float NewValue = --FuelValue;
    FuelValue = FMath::Clamp(NewValue, 0.f, MaxFuelValue);

    OnFuelValueChanged.Broadcast(FuelValue / MaxFuelValue);
}
