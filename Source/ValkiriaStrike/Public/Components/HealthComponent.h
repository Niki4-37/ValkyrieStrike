// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCoreTypes.h"
#include "HealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VALKIRIASTRIKE_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

    FOnDeathSignature OnDeath;
    /* used in widget */
    FOnHealthChangedSignature OnHealthChanged;

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    void SetHealth(float NewHealth);

    float GetHealth() const { return Health; };
    bool IsDead() const { return Health <= 0.f; };
    int32 GetNeededHealth() const { return static_cast<int32>(MaxHealth - Health); }

private:
    UPROPERTY(Replicated)
    float Health{0.f};
    UPROPERTY(Replicated)
    float MaxHealth{100.f};

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION(Client, unreliable)
    void OnHealthChanged_OnClient(float Value, float MaxValue);
};
