// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Interfaces/GameInterface.h"

// Add default functionality here for any IGameInterface functions that are not pure virtual.

bool IGameInterface::AddAmount(const FInteractionData&)
{
    return false;
}

bool IGameInterface::MakeMaintenance(EItemPropertyType)
{
    return false;
}

bool IGameInterface::SetWorkshopTasksData(const TArray<FInteractionData>&)
{
    return false;
}

bool IGameInterface::ShootFromWeapon()
{
    return false;
}

bool IGameInterface::UseReverseGear(bool bIsUsing)
{
    return false;
}
