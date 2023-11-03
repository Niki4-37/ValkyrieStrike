// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseVehicleWeapon.h"
#include "SecondWeapon.generated.h"

class UStaticMeshComponent;
class UNiagaraSystem;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString BeamWidthName = "BeamWidth";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget";

    virtual void BeginPlay() override;
    virtual void MakeShot() override;

    virtual void AlternativeShot() override;

private:
    UFUNCTION(NetMulticast, reliable)
    void SpawnTraceFX_Multicast(const FVector& TraceStart, const FVector& TraceEnd);

    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
};
