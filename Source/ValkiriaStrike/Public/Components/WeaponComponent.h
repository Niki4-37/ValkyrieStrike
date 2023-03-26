// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class ATurret;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VALKIRIASTRIKE_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponComponent();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ATurret> TurretClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName TurretSocketName{"TurretSocket"};

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY(Replicated)
    ATurret* VehicleTurret{nullptr};

    UFUNCTION(Server, reliable)
    void InitWeapons_OnServer();
};
