// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameLevelsConfig/ValkiriaPlayerState.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(ValkiriaPlayerState_LOG, All, All);

void AValkiriaPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AValkiriaPlayerState, Lives);
    DOREPLIFETIME(AValkiriaPlayerState, TurretClass);
    DOREPLIFETIME(AValkiriaPlayerState, SecondWeaponClass);
}

void AValkiriaPlayerState::SaveMountedItem(UClass* Class, EVehicleItemType Type)
{
    if (!Class) return;

    switch (Type)
    {
        case EVehicleItemType::Turret: TurretClass = Class; break;
        case EVehicleItemType::SecondWeapon: SecondWeaponClass = Class; break;
        default: return;
    }

    UE_LOG(ValkiriaPlayerState_LOG, Display, TEXT("SaveMountedItem %s"), *Class->GetName());
}

void AValkiriaPlayerState::CopyProperties(APlayerState* PlayerState)
{
    Super::CopyProperties(PlayerState);

    if (auto ValkiriaPlayerState = Cast<AValkiriaPlayerState>(PlayerState))
    {
        ValkiriaPlayerState->SetTurretClass(TurretClass);
        ValkiriaPlayerState->SetSecondWeaponClass(SecondWeaponClass);
    }
}

void AValkiriaPlayerState::OverrideWith(APlayerState* PlayerState)
{
    Super::OverrideWith(PlayerState);

     if (auto ValkiriaPlayerState = Cast<AValkiriaPlayerState>(PlayerState))
    {
         SetTurretClass(ValkiriaPlayerState->GetTurretClass());
         SetSecondWeaponClass(ValkiriaPlayerState->GetSecondWeaponClass());
     }
}
