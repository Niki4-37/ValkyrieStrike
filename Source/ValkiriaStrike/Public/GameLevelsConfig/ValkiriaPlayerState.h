// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameCoreTypes.h"
#include "ValkiriaPlayerState.generated.h"

UCLASS()
class VALKIRIASTRIKE_API AValkiriaPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void SaveMountedItem(UClass* Class, EVehicleItemType Type);

    UClass* GetTurretClass() const { return TurretClass; }
    void SetTurretClass(UClass* Class) { TurretClass = Class; }

    UClass* GetSecondWeaponClass() const { return SecondWeaponClass; }
    void SetSecondWeaponClass(UClass* Class) { SecondWeaponClass = Class; }

protected:
    UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite)
    uint8 Lives;

    virtual void CopyProperties(APlayerState* PlayerState) override;
    virtual void OverrideWith(APlayerState* PlayerState) override;

private:
    UPROPERTY(Transient, Replicated)
    UClass* TurretClass;
    UPROPERTY(Transient, Replicated)
    UClass* SecondWeaponClass;
};
