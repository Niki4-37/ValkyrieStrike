// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

class UHealthComponent;
class UAnimMontage;

UCLASS()
class VALKIRIASTRIKE_API AAICharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AAICharacter();

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UHealthComponent* HealthComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UAnimMontage* DeathMontage;

    virtual void BeginPlay() override;

private:
    void OnDeath();
};
