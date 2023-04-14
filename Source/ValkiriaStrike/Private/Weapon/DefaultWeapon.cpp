// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Weapon/DefaultWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Weapon/DefaultProjectile.h"

ADefaultWeapon::ADefaultWeapon()
{
    PrimaryActorTick.bCanEverTick = false;
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    WeaponMesh->SetupAttachment(RootComponent);
}

void ADefaultWeapon::StartFire(bool bIsPressed, const FVector& AimPosition)
{
    if (bIsPressed)
    {
        FTimerDelegate Delegate;
        Delegate.BindUFunction(this, "MakeShot", AimPosition);

        GetWorldTimerManager().SetTimer(FiringTimer, Delegate, 0.5f, true);
    }
    else
    {
        GetWorldTimerManager().ClearTimer(FiringTimer);
    }
}

void ADefaultWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(GetWorld());
    check(WeaponMesh);
}

void ADefaultWeapon::MakeShot(const FVector& AimPosition)
{
    if (!WeaponMesh || !GetWorld()) return;
    const FRotator MuzzleRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
    const FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);

    const FVector Direction = (AimPosition - MuzzleLocation).GetSafeNormal();

    FTransform SpawnTransform(MuzzleRotation, MuzzleLocation);
    auto Bullet = GetWorld()->SpawnActor<ADefaultProjectile>(ProjectileClass, SpawnTransform);
    if (Bullet)
    {
        Bullet->SetShootDirection(Direction);
        Bullet->SetLifeSpan(2.f);
    }
}
