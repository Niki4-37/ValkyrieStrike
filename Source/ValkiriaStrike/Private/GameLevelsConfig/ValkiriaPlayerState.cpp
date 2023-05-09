// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameLevelsConfig/ValkiriaPlayerState.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(ValkiriaPlayerState_LOG, All, All);

void AValkiriaPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AValkiriaPlayerState, Lives);
    DOREPLIFETIME(AValkiriaPlayerState, VehicleItems);
    DOREPLIFETIME(AValkiriaPlayerState, bNoLives);
}

void AValkiriaPlayerState::UpdateWidgetsInfo()
{
    OnLivesChanged_OnClient(Lives);
}

void AValkiriaPlayerState::SaveMountedItem(const FVehicleItemData& VehicleItemData)
{
    const auto ItemPtr = VehicleItems.FindByPredicate([&](FVehicleItemData Data) { return Data.ItemType == VehicleItemData.ItemType; });
    ItemPtr ? (*ItemPtr = VehicleItemData) : VehicleItems.Add_GetRef(VehicleItemData);
}

void AValkiriaPlayerState::CopyProperties(APlayerState* PlayerState)
{
    Super::CopyProperties(PlayerState);

    if (auto ValkiriaPlayerState = Cast<AValkiriaPlayerState>(PlayerState))
    {
        ValkiriaPlayerState->SetVehicleItems(VehicleItems);
    }
}

void AValkiriaPlayerState::OverrideWith(APlayerState* PlayerState)
{
    Super::OverrideWith(PlayerState);

    if (auto ValkiriaPlayerState = Cast<AValkiriaPlayerState>(PlayerState))
    {
        SetVehicleItems(ValkiriaPlayerState->GetVehicleItems());
    }
}

void AValkiriaPlayerState::OnLivesChanged_OnClient_Implementation(int32 Amount)
{
    // if (GetLocalRole() == ENetRole::ROLE_Authority)
    //{
    OnLivesChanged.Broadcast(Amount);
    //}
}
