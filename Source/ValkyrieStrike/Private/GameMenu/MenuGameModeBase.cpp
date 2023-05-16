// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/MenuGameModeBase.h"
#include "GameMenu/UI/MenuHUD.h"
#include "GameMenu/MenuPlayerController.h"
#include "GameLevelsConfig/ValkyriePlayerState.h"

AMenuGameModeBase::AMenuGameModeBase()
{
    HUDClass = AMenuHUD::StaticClass();
    PlayerControllerClass = AMenuPlayerController::StaticClass();
    PlayerStateClass = AValkyriePlayerState::StaticClass();

    bUseSeamlessTravel = true;
}
