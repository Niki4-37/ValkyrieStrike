// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Decorations/DecorationActor.h"
#include "GameCoreTypes.h"
#include "DummyVehicle.generated.h"

class UCameraComponent;

DECLARE_DELEGATE_OneParam(FOnOperationDelegateSignature, const FVehicleUnitData&)

    UCLASS() class VALKYRIESTRIKE_API ADummyVehicle : public ADecorationActor
{
    GENERATED_BODY()

public:
    ADummyVehicle();

    /* make RPC */
    void MountVehicleUnit_OnServer(const FVehicleUnitData& UnitData);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UCameraComponent* CameraComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* ChassisMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FVehicleUnitData TestBodyUnit;

private:
    UPROPERTY()
    // TMap<EVehicleUnitType, AActor*> AttachedActorsMap;
    TMap<FName, AActor*> AttachedActorsMap;

    TArray<FName> SocketNames;

    TMap<EVehicleUnitType, FOnOperationDelegateSignature> OperationMap;

    UFUNCTION()
    void MountArmor(const FVehicleUnitData& UnitData);
    UFUNCTION()
    void MountBody(const FVehicleUnitData& UnitData);
    UFUNCTION()
    void MountChassis(const FVehicleUnitData& UnitData);
    UFUNCTION()
    void MountWeapon(const FVehicleUnitData& UnitData);

    void ConstructVehicle(UStaticMeshComponent* VehicleMeshComponent, const FVehicleUnitData& UnitData);

    // UFUNCTION(NetMulticast, reliable)
    // void MountVehicleItem_Multicast(const FVehicleItemData& VehicleItemData);

    // UFUNCTION(NetMulticast, reliable)
    // void MountVehiclePart_Multicast(const FVehicleConstructPart& VehiclePart);
};
