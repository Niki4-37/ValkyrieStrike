// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameLevelsConfig/ValkyriePlayerState.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(ValkyriePlayerState_LOG, All, All);

void AValkyriePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AValkyriePlayerState, Lives);
    DOREPLIFETIME(AValkyriePlayerState, VehicleUnits);
    DOREPLIFETIME(AValkyriePlayerState, bNoLives);
    DOREPLIFETIME(AValkyriePlayerState, Coins);
}

void AValkyriePlayerState::UpdateWidgetsInfo()
{
    OnLivesChanged_OnClient(Lives);
}

void AValkyriePlayerState::SaveMountedUnit(const FVehicleUnitData& VehicleUnit)
{
    const auto UnitPtr = VehicleUnits.FindByPredicate([&](FVehicleUnitData Data) { return Data.UnitType == VehicleUnit.UnitType; });
    UnitPtr ? (*UnitPtr = VehicleUnit) : VehicleUnits.Add_GetRef(VehicleUnit);
}

void AValkyriePlayerState::CopyProperties(APlayerState* PlayerState)
{
    Super::CopyProperties(PlayerState);

    if (auto ValkyriePlayerState = Cast<AValkyriePlayerState>(PlayerState))
    {
        ValkyriePlayerState->SetVehicleUnits(VehicleUnits);
    }
}

void AValkyriePlayerState::OverrideWith(APlayerState* PlayerState)
{
    Super::OverrideWith(PlayerState);

    if (auto ValkyriePlayerState = Cast<AValkyriePlayerState>(PlayerState))
    {
        SetVehicleUnits(ValkyriePlayerState->GetVehicleUnits());
    }
}

void AValkyriePlayerState::OnLivesChanged_OnClient_Implementation(int32 Amount)
{
    OnLivesChanged.Broadcast(Amount);
}
