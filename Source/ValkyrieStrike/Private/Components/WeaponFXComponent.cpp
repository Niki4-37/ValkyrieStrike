// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/WeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"

UWeaponFXComponent::UWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DefaultEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
}
