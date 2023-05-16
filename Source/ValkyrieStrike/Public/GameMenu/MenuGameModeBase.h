// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameCoreTypes.h"
#include "MenuGameModeBase.generated.h"

class AMenuVehicleActor;
class APlayerStart;
class AMenuPlayerController;

UCLASS()
class VALKYRIESTRIKE_API AMenuGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMenuGameModeBase();
};
