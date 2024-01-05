// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCoreTypes.h"
#include "HealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VALKYRIESTRIKE_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

    FOnDeathSignature OnDeath;
    /* used in widget */
    FOnItemValueChangedSignature OnItemValueChanged;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> ExplosionCameraShake;

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void Killed();

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    void AddHealth(float Value);

    bool IsDead() const { return Health <= 0.f; };
    virtual void UpdateWidgetsInfo();

private:
    UPROPERTY(Replicated)
    float Health{0.f};
    UPROPERTY(Replicated)
    float MaxHealth{100.f};

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION(NetMulticast, unreliable)
    void OnHealthChanged_Multicast(float Value, float MaxValue);

    UFUNCTION(Client, unreliable)
    void ShakeCamera_OnClient();
};
