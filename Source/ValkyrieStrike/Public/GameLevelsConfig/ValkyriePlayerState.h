// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameCoreTypes.h"
#include "ValkyriePlayerState.generated.h"

UCLASS()
class VALKYRIESTRIKE_API AValkyriePlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    FOnCoinsChangedSignature OnLivesChanged;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    bool CanRespawn() const { return !bNoLives; }
    void ChangeLives(int32 Amount)
    {
        bNoLives = Lives == 0 && Amount < 0;
        Lives = FMath::Clamp(Lives + Amount, 0, 10);
        OnLivesChanged_OnClient(Lives);
    }

    void UpdateWidgetsInfo();

    void SaveMountedItem(const FVehicleItemData& VehicleItemData);

    const TArray<FVehicleItemData>& GetVehicleItems() const { return VehicleItems; };
    void SetVehicleItems(const TArray<FVehicleItemData>& Items) { VehicleItems = Items; };

    void SetRespawnTransform(const FTransform& NewRespawnTransform)
    {
        bIsNotDiedYet = false;
        RespawnTransform = NewRespawnTransform;
    }
    const FTransform& GetRespawnTransform() const { return RespawnTransform; }
    bool IsFirstDead() const { return bIsNotDiedYet; }

protected:
    UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite)
    int32 Lives{5};

    virtual void CopyProperties(APlayerState* PlayerState) override;
    virtual void OverrideWith(APlayerState* PlayerState) override;

private:
    UPROPERTY(Transient, Replicated)
    TArray<FVehicleItemData> VehicleItems;

    bool bIsNotDiedYet{true};
    UPROPERTY(Replicated)
    bool bNoLives{false};

    FTransform RespawnTransform;

    UFUNCTION(Client, reliable)
    void OnLivesChanged_OnClient(int32 Amount);
};
