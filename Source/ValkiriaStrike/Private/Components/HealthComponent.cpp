// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(HealthComponent_LOG, All, All);

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    SetHealth(MaxHealth);  // Server?

    const auto ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
        // ComponentOwner->OnTakePointDamage.AddDynamic(this, &UTDSHealthComponent::OnTakePointDamage);
        // ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &UTDSHealthComponent::OnTakeRadialDamage);
    }
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UHealthComponent, Health);
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::SetHealth(float NewHealth)
{
    float DeltaHealth = NewHealth - Health;
    Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);

    float HelthPercentage = MaxHealth > 0 ? Health / MaxHealth : 0.f;
    OnHealthChanged_OnClient(HelthPercentage);
    UE_LOG(HealthComponent_LOG, Display, TEXT("SetHealth: %f"), Health);
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage < 0.f || IsDead()) return;
    SetHealth(Health - Damage);

    if (IsDead())
    {
        OnDeath.Broadcast();
        UE_LOG(HealthComponent_LOG, Display, TEXT("DEAD!"));
    }
}

void UHealthComponent::OnHealthChanged_OnClient_Implementation(float HealthPercentage)
{
    OnHealthChanged.Broadcast(HealthPercentage);
}
