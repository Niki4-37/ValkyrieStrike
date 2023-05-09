// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameLevelsConfig/ValkyriePlayerState.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(ValkyriePlayerState_LOG, All, All);

void AValkyriePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AValkyriePlayerState, Lives);
    DOREPLIFETIME(AValkyriePlayerState, VehicleItems);
    DOREPLIFETIME(AValkyriePlayerState, bNoLives);
}

void AValkyriePlayerState::UpdateWidgetsInfo()
{
    OnLivesChanged_OnClient(Lives);
}

void AValkyriePlayerState::SaveMountedItem(const FVehicleItemData& VehicleItemData)
{
    const auto ItemPtr = VehicleItems.FindByPredicate([&](FVehicleItemData Data) { return Data.ItemType == VehicleItemData.ItemType; });
    ItemPtr ? (*ItemPtr = VehicleItemData) : VehicleItems.Add_GetRef(VehicleItemData);
}

void AValkyriePlayerState::CopyProperties(APlayerState* PlayerState)
{
    Super::CopyProperties(PlayerState);

    if (auto ValkyriePlayerState = Cast<AValkyriePlayerState>(PlayerState))
    {
        ValkyriePlayerState->SetVehicleItems(VehicleItems);
    }
}

void AValkyriePlayerState::OverrideWith(APlayerState* PlayerState)
{
    Super::OverrideWith(PlayerState);

    if (auto ValkyriePlayerState = Cast<AValkyriePlayerState>(PlayerState))
    {
        SetVehicleItems(ValkyriePlayerState->GetVehicleItems());
    }
}

void AValkyriePlayerState::OnLivesChanged_OnClient_Implementation(int32 Amount)
{
    // if (GetLocalRole() == ENetRole::ROLE_Authority)
    //{
    OnLivesChanged.Broadcast(Amount);
    //}
}
