// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "ValkiriaStrikeGameMode.h"
#include "Player/DefaultWeeledVehicle.h"

AValkiriaStrikeGameMode::AValkiriaStrikeGameMode()
{
    DefaultPawnClass = ADefaultWeeledVehicle::StaticClass();
    // HUDClass = AValkiriaStrikeHud::StaticClass();
}
