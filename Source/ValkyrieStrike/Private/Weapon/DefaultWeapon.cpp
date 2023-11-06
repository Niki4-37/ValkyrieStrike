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

        GetWorldTimerManager().SetTimer(FiringTimer, Delegate, FireRate, true);
    }
    else
    {
        GetWorldTimerManager().ClearTimer(FiringTimer);
    }
}

void ADefaultWeapon::SetFireRate(float NewFireRate) 
{
    if (NewFireRate < 0.f) return;
    FireRate = NewFireRate;
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

    const auto BulletSpread = 7.f;
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(Direction, HalfRad);

    FTransform SpawnTransform(MuzzleRotation, MuzzleLocation);
    auto Bullet = GetWorld()->SpawnActor<ADefaultProjectile>(ProjectileClass, SpawnTransform);
    if (Bullet)
    {
        Bullet->SetShootDirection(ShootDirection);
        Bullet->SetLifeSpan(2.f);
    }
}
