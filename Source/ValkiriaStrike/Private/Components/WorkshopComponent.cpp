// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/WorkshopComponent.h"
#include "Components/VehicleIndicatorsComponent.h"
#include "Interfaces/GameInterface.h"
#include "Net/UnrealNetwork.h"

#include "Engine.h"

UWorkshopComponent::UWorkshopComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    for (EItemPropertyType Type = EItemPropertyType::Begin; Type != EItemPropertyType::End; ++Type)
    {
        VehicleConsumables.Add(FInteractionData(Type, 1, nullptr));
    }
}

void UWorkshopComponent::AddCoins(int32 Value)
{
    Coins += Value;
    OnRep_Coins();
}

void UWorkshopComponent::SetWorkshopTasks(const TArray<FInteractionData>& Tasks)
{
    WorkshopTasks = Tasks;
    OnRep_WorkshopTasks();
    for (auto& Task : Tasks)
    {
        if (Task.Type == EItemPropertyType::Ammo)
        {
            OnUpdateCost_Client(Task.Type, 1);
        }
        else
        {
            OnUpdateCost_Client(Task.Type, GetValueToAdd(Task.Type));
        }
    }
}

int32 UWorkshopComponent::GetValueToAdd(EItemPropertyType Type)
{
    auto ConsumablesPtr = VehicleConsumables.FindByPredicate([&](const FInteractionData& Data) { return Data.Type == Type; });
    if (!ConsumablesPtr) return 0;
    return ConsumablesPtr->Amount;
}

int32 UWorkshopComponent::GetTaskPrice(EItemPropertyType Type)
{
    auto TaskPtr = WorkshopTasks.FindByPredicate([&](const FInteractionData& Data) { return Data.Type == Type; });
    if (!TaskPtr) return 0;
    return TaskPtr->Amount;
}

void UWorkshopComponent::MakeMaintenance(EItemPropertyType Type)
{
    const auto GameInterface = Cast<IGameInterface>(GetOwner());
    if (!GameInterface) return;
    const int32 ValueToAdd = GetValueToAdd(Type);
    const int32 TaskPrice = GetTaskPrice(Type);
    const int32 TotalCost = ValueToAdd * TaskPrice;

    if (Coins >= TotalCost)
    {
        if (GameInterface->AddAmount(FInteractionData(Type, ValueToAdd)))
        {
            AddCoins(-TotalCost);
        }
    }
    else
    {
        if (GameInterface->AddAmount(FInteractionData(Type, Coins)))
        {
            AddCoins(-Coins);
        }
    }
}

void UWorkshopComponent::BeginPlay()
{
    Super::BeginPlay();

    const auto IndicatorsComp = GetOwner()->FindComponentByClass<UVehicleIndicatorsComponent>();
    if (IndicatorsComp)
    {
        IndicatorsComp->OnItemValueChanged.AddUObject(this, &UWorkshopComponent::OnItemValueChanged);
    }
}

void UWorkshopComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UWorkshopComponent, Coins);
    DOREPLIFETIME(UWorkshopComponent, WorkshopTasks);
    DOREPLIFETIME(UWorkshopComponent, VehicleConsumables);
}

void UWorkshopComponent::OnItemValueChanged(EItemPropertyType Type, float Value, float MaxValue)
{
    if (GetOwnerRole() == ENetRole::ROLE_Authority)
    {
        const auto ConsumableValue = FMath::CeilToInt(MaxValue - Value);
        const auto ConsumablesPtr = VehicleConsumables.FindByPredicate([&](const FInteractionData& Data) { return Data.Type == Type; });
        if (!ConsumablesPtr) return;

        ConsumablesPtr->Amount = ConsumableValue;
        OnUpdateCost_Client(Type, ConsumableValue);
    }
}

void UWorkshopComponent::OnUpdateCost_Client_Implementation(EItemPropertyType Type, int32 Value)
{
    OnUpdateCost.Broadcast(Type, Value);
}

void UWorkshopComponent::OnRep_Coins()
{
    OnCoinsChanged.Broadcast(Coins);
}

void UWorkshopComponent::OnRep_WorkshopTasks()
{
    OnTasksUpdated.Broadcast(WorkshopTasks);
}
