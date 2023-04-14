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

    void SaveMountedItem(const FVehicleItemData& VehicleItemData);

    const TArray<FVehicleItemData>& GetVehicleItems() const { return VehicleItems; };
    void SetVehicleItems(const TArray<FVehicleItemData>& Items) { VehicleItems = Items; };

protected:
    UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite)
    uint8 Lives;

    virtual void CopyProperties(APlayerState* PlayerState) override;
    virtual void OverrideWith(APlayerState* PlayerState) override;

private:
    UPROPERTY(Transient, Replicated)
    TArray<FVehicleItemData> VehicleItems;  // Use reserve, update EVehicleItemType add MAX
};
