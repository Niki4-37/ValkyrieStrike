// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/VehicleIndicatorsComponent.h"
#include "Net/UnrealNetwork.h"

UVehicleIndicatorsComponent::UVehicleIndicatorsComponent()
{
    SetIsReplicatedByDefault(true);
}

bool UVehicleIndicatorsComponent::AddFuel(int32 Value)
{
    bool bCanChanged{FuelValue < MaxFuelValue};

    FuelValue = FMath::Clamp(FuelValue + FMath::CeilToFloat(Value), 0.f, MaxFuelValue);
    OnFuelValueChanged_Multicast(FuelValue, MaxFuelValue);
    return bCanChanged;
}

void UVehicleIndicatorsComponent::SetFuelConsumptionModifier_OnServer_Implementation(float Percentage)
{
    FuelConsumptionModifier = FMath::Lerp(0.f, 2.f, Percentage);
}

void UVehicleIndicatorsComponent::UpdateWidgetsInfo()
{
    Super::UpdateWidgetsInfo();
    OnFuelValueChanged_Multicast(FuelValue, MaxFuelValue);
}

void UVehicleIndicatorsComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld() && GetOwnerRole() == ENetRole::ROLE_Authority)
    {
        checkf(MaxFuelValue > 0, TEXT("MaxFuelValue can't be less or equals 0!"));
        AddFuel(MaxFuelValue);
        OnFuelValueChanged_Multicast(FuelValue, MaxFuelValue);
        GetWorld()->GetTimerManager().SetTimer(FuelChangeTimer, this, &UVehicleIndicatorsComponent::SpendFuel, 1.f, true);
    }
}

void UVehicleIndicatorsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UVehicleIndicatorsComponent, FuelValue);
    DOREPLIFETIME(UVehicleIndicatorsComponent, FuelConsumptionModifier);
}

void UVehicleIndicatorsComponent::SpendFuel()
{
    const float NewValue = FuelValue - FuelConsumptionModifier;
    FuelValue = FMath::Clamp(NewValue, 0.f, MaxFuelValue);

    OnFuelValueChanged_Multicast(FuelValue, MaxFuelValue);
}

void UVehicleIndicatorsComponent::OnFuelValueChanged_Multicast_Implementation(float Value, float MaxValue)
{
    OnItemValueChanged.Broadcast(EItemPropertyType::Fuel, Value, MaxValue);
}
