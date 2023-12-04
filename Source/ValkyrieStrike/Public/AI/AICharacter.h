// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

class UHealthComponent;
class UAnimMontage;
class ADefaultWeapon;
class UBehaviorTree;
class UDropComponent;

UCLASS()
class VALKYRIESTRIKE_API AAICharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AAICharacter();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UBehaviorTree* BehaviorTreeAsset;

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void AttackEnemy(AActor* AimActor);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UHealthComponent* HealthComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDropComponent* DropComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    UAnimMontage* DeathMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    UAnimMontage* AttackMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName WeaponSocketName{"WeaponSocket"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName BackpackSocketName{"BackpackSocket"};

    bool bCanPlayMontage{true};

    virtual void BeginPlay() override;

    virtual void OnDeath();

private:
    UFUNCTION(NetMulticast, unreliable)
    void PlayAnimMontage_Multicast(UAnimMontage* Animation);

    UFUNCTION(NetMulticast, reliable)
    void SwitchOffCollision_Multicast();
};
