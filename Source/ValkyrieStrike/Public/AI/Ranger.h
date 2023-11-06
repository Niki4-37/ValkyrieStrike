// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacter.h"
#include "Ranger.generated.h"

UCLASS()
class VALKYRIESTRIKE_API ARanger : public AAICharacter
{
    GENERATED_BODY()

public:
    ARanger();

    virtual void AttackEnemy(AActor* AimActor) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ADefaultWeapon> WeaponClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1", ClampMax = "10.0"))
    float FireRate{1.0f};

    virtual void BeginPlay() override;

    virtual void OnDeath() override;

private:
    UPROPERTY()
    ADefaultWeapon* OwnedWeapon;

    void SpawnAndAttachWeapon();
};
