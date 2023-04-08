// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameCoreTypes.h"
#include "ValkiriaGameInstance.generated.h"

UCLASS()
class VALKIRIASTRIKE_API UValkiriaGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    TArray<FLevelData> GetLevelsData() const { return LevelsData; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ToolTip = "Level names must be unique!"))
    TArray<FLevelData> LevelsData;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Settings")
    // UDataTable* VehicleItemsTable;

private:
    FLevelData StartupLevel;
};
