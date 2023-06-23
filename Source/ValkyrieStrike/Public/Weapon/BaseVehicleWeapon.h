// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCoreTypes.h"
#include "BaseVehicleWeapon.generated.h"

UCLASS()
class VALKYRIESTRIKE_API ABaseVehicleWeapon : public AActor
{
    GENERATED_BODY()

public:
    ABaseVehicleWeapon();

    void SetStaticMesh(EUnitComponentType Type, UStaticMesh* NewMesh, FName SocketName);

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* Platform;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* Gun;

    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};
