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

    UE_LOG(LogTemp, Display, TEXT("Coins: %i"), Coins);
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

void UVehicleIndicatorsComponent::RepairVehicle(int32 Price)
{
    const int32 TotalCost = GetNeededHealth() * Price;
    if (Coins >= TotalCost)
    {
        SetHealth(GetHealth() + static_cast<float>(GetNeededHealth()));
        AddCoins(-TotalCost);
    }
    else
    {
        SetHealth(GetHealth() + static_cast<float>(Coins));
        AddCoins(-Coins);
    }
}

void UVehicleIndicatorsComponent::RepairArmor(int32 Price) {}

void UVehicleIndicatorsComponent::Refuel(int32 Price)
{
    UE_LOG(LogTemp, Display, TEXT("Price: %i"), Price);
    const int32 TotalCost = static_cast<int32>(MaxFuelValue - FuelValue) * Price;
    if (Coins >= TotalCost)
    {
        AddFuel(static_cast<int32>(MaxFuelValue - FuelValue));
        AddCoins(-TotalCost);
    }
    else
    {
        AddFuel(Coins);
        AddCoins(-Coins);
    }
}

int32 UVehicleIndicatorsComponent::GetNeededValue(EItemPropertyType Type)
{
    switch (Type)
    {
        case EItemPropertyType::Endurance: return GetNeededHealth();
        case EItemPropertyType::Armor: return 0;
        case EItemPropertyType::Fuel: return static_cast<int32>(MaxFuelValue - FuelValue);
        default: return 0;
    }
}

void UVehicleIndicatorsComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(MaxFuelValue > 0, TEXT("MaxFuelValue can't be less or equals 0!"));
    AddFuel(MaxFuelValue);  // Server?
    OnFuelValueChanged.Broadcast(FuelValue / MaxFuelValue);
    GetWorld()->GetTimerManager().SetTimer(FuelChangeTimer, this, &UVehicleIndicatorsComponent::SpendFuel, 1.f, true);
}

void UVehicleIndicatorsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UVehicleIndicatorsComponent, FuelValue);
    DOREPLIFETIME(UVehicleIndicatorsComponent, FuelConsumptionModifier);
    DOREPLIFETIME(UVehicleIndicatorsComponent, Coins);
}

void UVehicleIndicatorsComponent::SpendFuel()
{
    const float NewValue = --FuelValue;
    FuelValue = FMath::Clamp(NewValue, 0.f, MaxFuelValue);

    OnFuelValueChanged.Broadcast(FuelValue / MaxFuelValue);
}
