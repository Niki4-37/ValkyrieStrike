// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameCoreTypes.h"
#include "VehiclePlayerController.generated.h"

UCLASS()
class VALKIRIASTRIKE_API AVehiclePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    UFUNCTION(Server, reliable)
    void MakeMaintenance_OnServer(EItemPropertyType Type);

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
};
