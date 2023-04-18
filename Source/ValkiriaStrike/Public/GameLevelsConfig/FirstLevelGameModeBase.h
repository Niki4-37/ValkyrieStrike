// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FirstLevelGameModeBase.generated.h"

class APlayerStart;

UCLASS()
class VALKIRIASTRIKE_API AFirstLevelGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AFirstLevelGameModeBase();

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual void HandleSeamlessTravelPlayer(AController*& C) override;

    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
    UPROPERTY()
    TMap<AActor*, bool> PlayerStartMap;

    void FillPlayerStartMap();
};
