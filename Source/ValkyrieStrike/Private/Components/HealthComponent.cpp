// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameLevelsConfig/FirstLEvelGameModeBase.h"
#include "Weapon/ExplosionDamageType.h"

DEFINE_LOG_CATEGORY_STATIC(HealthComponent_LOG, All, All);

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    // checkf(ExplosionCameraShake, TEXT("CameraShake not define!"));

    if (GetOwnerRole() == ENetRole::ROLE_Authority)  //(GetOwner()->HasAuthority())
    {
        AddHealth(MaxHealth);
    }

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
    DOREPLIFETIME(UHealthComponent, MaxHealth);
}

void UHealthComponent::Killed()
{
    if (!GetWorld()) return;
    const auto FirstLevelGM = GetWorld()->GetAuthGameMode<AFirstLevelGameModeBase>();
    if (!FirstLevelGM || !GetOwner()) return;
    FirstLevelGM->Killed(GetOwner()->GetInstigatorController(), GetOwner()->GetActorTransform());
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::AddHealth(float Value)
{
    const float NewHealth = Health + Value;
    Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);

    OnHealthChanged_Multicast(Health, MaxHealth);
}

void UHealthComponent::UpdateWidgetsInfo()
{
    OnHealthChanged_Multicast(Health, MaxHealth);
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage < 0.f || IsDead()) return;
    AddHealth(-Damage);

    if (IsDead())
    {
        OnDeath.Broadcast();
        UE_LOG(HealthComponent_LOG, Display, TEXT("DEAD!"));
        Killed();
    }

    if (DamageType)
    {
        if (DamageType->IsA<UExplosionDamageType>())
        {
            ShakeCamera_OnClient();
        }
    }
}

void UHealthComponent::OnHealthChanged_Multicast_Implementation(float Value, float MaxValue)
{
    OnItemValueChanged.Broadcast(EItemPropertyType::Endurance, Value, MaxValue);
}

void UHealthComponent::ShakeCamera_OnClient_Implementation() 
{
    if (!GetOwner()) return;
    const auto PlayerController = GetOwner()->GetInstigatorController<APlayerController>();
    if (!PlayerController || !PlayerController->PlayerCameraManager) return;
    PlayerController->PlayerCameraManager->StartCameraShake(ExplosionCameraShake);
}