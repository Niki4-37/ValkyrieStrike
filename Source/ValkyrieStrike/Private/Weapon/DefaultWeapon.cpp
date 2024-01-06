// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Weapon/DefaultWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Weapon/DefaultProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ADefaultWeapon::ADefaultWeapon()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
    WeaponRootComponent = CreateDefaultSubobject<USceneComponent>("WeaponRootComponent");
    SetRootComponent(WeaponRootComponent);

    WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponStaticlMesh");
    WeaponStaticMesh->SetupAttachment(RootComponent);
}

void ADefaultWeapon::StartFire(bool bIsPressed, const FVector& AimPosition)
{
    if (!bWeaponIsReady) return;
    MakeShot(AimPosition);
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
    check(WeaponStaticMesh);
}

void ADefaultWeapon::MakeShot(const FVector& AimPosition)
{
    if (!GetWorld()) return;
    bWeaponIsReady = false;
    GetWorldTimerManager().ClearTimer(FireRateTimer);

    const FRotator MuzzleRotation = WeaponStaticMesh->GetSocketRotation(MuzzleSocketName);
    const FVector MuzzleLocation = WeaponStaticMesh->GetSocketLocation(MuzzleSocketName);

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

        SpawnSound_Multicast(FireSound, WeaponStaticMesh, MuzzleSocketName);
    }

    GetWorldTimerManager().SetTimer(
        FireRateTimer, [&]() { bWeaponIsReady = true; }, FireRate, false);
}

void ADefaultWeapon::SpawnSound_Multicast_Implementation(USoundBase* Sound, USceneComponent* AttachToComponent, FName AttachPointName)
{
    UGameplayStatics::SpawnSoundAttached(Sound, AttachToComponent, AttachPointName);
}