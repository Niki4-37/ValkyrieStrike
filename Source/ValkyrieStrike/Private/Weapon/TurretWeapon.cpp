// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Weapon/TurretWeapon.h"
#include "Weapon/DefaultProjectile.h"

void ATurretWeapon::BeginPlay()
{
    Super::BeginPlay();

    checkf(DefaultProjectileClass, TEXT("DefaultProjectileClass not define!"));

    if (HasAuthority())
    {
        GetWorldTimerManager().ClearTimer(FireTimer);
        GetWorldTimerManager().SetTimer(FireTimer, this, &ATurretWeapon::MakeShot, FireRate, true);
    }
}

void ATurretWeapon::RotateToTarget(AActor* Target)
{
    Super::RotateToTarget(Target);

    const auto NoneAimLocation = GetOwner() ? (GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 1000.f) : FVector::ZeroVector;
    const auto AimLocation = Target ? Target->GetActorLocation() : NoneAimLocation;

    const FRotator Direction = FRotationMatrix::MakeFromX(AimLocation - GetActorLocation()).Rotator();

    const auto GunRotation = Gun->GetComponentRotation();
    auto Delta = Direction - GunRotation;
    Delta.Normalize();
    const auto GunDeltaValue = bIsSideMode  //
                                   ?
                                   FMath::Clamp(Delta.Yaw, -1.f, 1.f)  //
                                   :
                                   FMath::Clamp(Delta.Pitch, -1.f, 1.f);

    const auto GunValueToClamp = Gun->GetRelativeRotation().Pitch + (bIsSideMode ? SidePositionModifier : 1.f) * GunDeltaValue;
    const auto GunValueToSet = FMath::Clamp(GunValueToClamp, -5.f, 90.f);

    Gun->SetRelativeRotation(FRotator(GunValueToSet, 0.f, 0.f));
}

void ATurretWeapon::MakeShot()
{
    Super::MakeShot();

    if (!HasAim() || IsReloading() || IsEmpty()) return;

    const FRotator MuzzleRotation = Gun->GetSocketRotation(MuzzleSocketName);
    const FVector MuzzleLocation = Gun->GetSocketLocation(MuzzleSocketName);

    const auto BulletSpread = 5.f;
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(MuzzleRotation.Vector(), HalfRad);

    FTransform SpawnTransform(MuzzleRotation, MuzzleLocation);
    auto Bullet = GetWorld()->SpawnActor<ADefaultProjectile>(DefaultProjectileClass, SpawnTransform);
    if (Bullet)
    {
        Bullet->SetShootDirection(ShootDirection);
        Bullet->SetLifeSpan(2.f);
    }

    ChangeAmmo(-1);

    if (IsEmpty())
    {
        ReloadWeapon();
    }
}
