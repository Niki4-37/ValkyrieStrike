// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameCoreTypes.h"
#include "LobbyHUD.generated.h"

UCLASS()
class VALKYRIESTRIKE_API ALobbyHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void PostInitializeComponents() override;
    virtual void Destroyed() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> LobbyHUDWidgetClass;

    virtual void BeginPlay() override;

private:
    UUserWidget* LobbyHUDWidget{nullptr};
};
