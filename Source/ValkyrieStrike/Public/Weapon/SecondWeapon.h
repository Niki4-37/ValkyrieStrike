// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseVehicleWeapon.h"
#include "SecondWeapon.generated.h"

class UStaticMeshComponent;

UCLASS()
class VALKYRIESTRIKE_API ASecondWeapon : public ABaseVehicleWeapon
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BeemRadius = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WeaponDamage = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Choose object type to handle damage"))
    TArray<TEnumAsByte<EObjectTypeQuery>> TreceForObjectTypes;

    virtual void BeginPlay() override;
    virtual void MakeShot() override;

    virtual void AlternativeShot() override;
};
