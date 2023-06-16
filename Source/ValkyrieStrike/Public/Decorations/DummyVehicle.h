// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Decorations/DecorationActor.h"
#include "GameCoreTypes.h"
#include "DummyVehicle.generated.h"

class UCameraComponent;

UCLASS()
class VALKYRIESTRIKE_API ADummyVehicle : public ADecorationActor
{
    GENERATED_BODY()

public:
    ADummyVehicle();

    UFUNCTION(Server, reliable)
    void MountVehicleItem_OnSever(const FVehicleItemData& VehicleItemData);

    UFUNCTION(Server, reliable)
    void MountVehiclePart_OnServer(const FVehicleConstructPart& VehiclePart);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UCameraComponent* CameraComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* ChassisMesh;

private:
    UPROPERTY()
    TMap<EVehicleItemType, AActor*> AttachedActorsMap;

    UFUNCTION(NetMulticast, reliable)
    void MountVehicleItem_Multicast(const FVehicleItemData& VehicleItemData);

    UFUNCTION(NetMulticast, reliable)
    void MountVehiclePart_Multicast(const FVehicleConstructPart& VehiclePart);
};
