// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/AICharacter.h"
#include "AI/AIEnemyController.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/DefaultWeapon.h"

AAICharacter::AAICharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AAIEnemyController::StaticClass();

    bUseControllerRotationYaw = false;

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
}

void AAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAICharacter::StartFire(bool bIsPressed)
{
    if (!OwnedWeapon) return;
    OwnedWeapon->StartFire(bIsPressed);
}

void AAICharacter::BeginPlay()
{
    Super::BeginPlay();
    check(HealthComponent) HealthComponent->OnDeath.AddUObject(this, &AAICharacter::OnDeath);
    Tags.Add("Enemy");
    SpawnAndAttachWeapon();
}

void AAICharacter::OnDeath()
{
    // GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    PlayAnimMontage(DeathMontage);
}

void AAICharacter::SpawnAndAttachWeapon()
{
    if (OwnedWeapon) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Owner = GetOwner();
    OwnedWeapon = GetWorld()->SpawnActor<ADefaultWeapon>(WeaponClass, SpawnParams);

    if (!OwnedWeapon) return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    OwnedWeapon->AttachToComponent(GetMesh(), AttachmentRules, WeaponSocketName);
}
