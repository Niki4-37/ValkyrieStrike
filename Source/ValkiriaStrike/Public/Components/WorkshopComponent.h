// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCoreTypes.h"
#include "WorkshopComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VALKIRIASTRIKE_API UWorkshopComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWorkshopComponent();

    FOnWorkshopTasksUpdatedSignature OnTasksUpdated;
    FOnUpdateCostSignature OnUpdateCost;
    FOnCoinsChangedSignature OnCoinsChanged;

    void AddCoins(int32 Value);

    void SetWorkshopTasks(const TArray<FInteractionData>& Tasks);

    int32 GetValueToAdd(EItemPropertyType Type);
    int32 GetTaskPrice(EItemPropertyType Type);

    void MakeMaintenance(EItemPropertyType Type);

    void UpdateWidgetsInfo();

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY(ReplicatedUsing = OnRep_WorkshopTasks)
    TArray<FInteractionData> WorkshopTasks;
    UPROPERTY(Replicated)
    TArray<FInteractionData> VehicleConsumables;
    UPROPERTY(ReplicatedUsing = OnRep_Coins)
    int32 Coins{0};

    void OnItemValueChanged(EItemPropertyType Type, float Value, float MaxValue);

    UFUNCTION(Client, reliable)
    void OnUpdateCost_Client(EItemPropertyType Type, int32 Value);

    UFUNCTION()
    virtual void OnRep_Coins();

    UFUNCTION()
    void OnRep_WorkshopTasks();
};
