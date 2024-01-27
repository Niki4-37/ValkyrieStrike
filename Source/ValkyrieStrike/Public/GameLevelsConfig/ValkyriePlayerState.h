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

    void SaveCoins(int32 Value) { Coins = Value; }
    int32 GetCoins() const { return Coins; }

    void UpdateWidgetsInfo();

    void SaveMountedUnit(const FVehicleUnitData& VehicleUnit);

    const TArray<FVehicleUnitData>& GetVehicleUnits() const
    {
        if (VehicleUnits.Num()) return VehicleUnits;
        /* Default settings */
        return DebugVehicleUnits;
        /* --------------- */
    };
    void SetVehicleUnits(const TArray<FVehicleUnitData>& Units) { VehicleUnits = Units; };

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

    /* Default settings */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FVehicleUnitData> DebugVehicleUnits;
    /* --------------- */

    virtual void CopyProperties(APlayerState* PlayerState) override;
    virtual void OverrideWith(APlayerState* PlayerState) override;

private:
    UPROPERTY(Transient, Replicated)
    TArray<FVehicleUnitData> VehicleUnits;

    bool bIsNotDiedYet{true};
    UPROPERTY(Replicated)
    bool bNoLives{false};

    UPROPERTY(Replicated)
    int32 Coins{0};

    FTransform RespawnTransform;

    UFUNCTION(Client, reliable)
    void OnLivesChanged_OnClient(int32 Amount);
};
