// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/Ranger.h"
#include "Weapon/DefaultWeapon.h"

ARanger::ARanger() {}

void ARanger::AttackEnemy(AActor* AimActor)
{
    if (!OwnedWeapon) return;
    const bool bCanFire = AimActor ? true : false;

    FVector AimPosition = FVector::ZeroVector;
    if (AimActor)
    {
        AimActor->GetVelocity();
        AimPosition = AimActor->GetActorLocation() + AimActor->GetVelocity();
    }

    OwnedWeapon->StartFire(bCanFire, AimPosition);
}

void ARanger::BeginPlay()
{
    Super::BeginPlay();
    SpawnAndAttachWeapon();
}

void ARanger::OnDeath()
{
    Super::OnDeath();
    OwnedWeapon->StartFire(false, FVector::ZeroVector);
}

void ARanger::SpawnAndAttachWeapon()
{
    if (OwnedWeapon) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Owner = GetOwner();
    OwnedWeapon = GetWorld()->SpawnActor<ADefaultWeapon>(WeaponClass, SpawnParams);

    if (!OwnedWeapon) return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    OwnedWeapon->AttachToComponent(GetMesh(), AttachmentRules, WeaponSocketName);
    OwnedWeapon->SetFireRate(FireRate);
}
