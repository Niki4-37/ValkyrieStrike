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

void ADefaultWeapon::StartFire(bool bIsPressed)
{
    bIsPressed ?                                                                                   //
        GetWorldTimerManager().SetTimer(FiringTimer, this, &ADefaultWeapon::MakeShot, 0.5f, true)  //
        :                                                                                          //
        GetWorldTimerManager().ClearTimer(FiringTimer);
}

void ADefaultWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(GetWorld());
    check(WeaponMesh);

    GetWorldTimerManager().SetTimer(FiringTimer, this, &ADefaultWeapon::MakeShot, 0.5f, true);
}

void ADefaultWeapon::MakeShot()
{
    if (!WeaponMesh || !GetWorld()) return;
    const FRotator MuzzleRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
    const FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);

    FTransform SpawnTransform(MuzzleRotation, MuzzleLocation);
    auto Bullet = GetWorld()->SpawnActor<ADefaultProjectile>(ProjectileClass, SpawnTransform);
    if (Bullet)
    {
        Bullet->SetLifeSpan(2.f);
    }
}