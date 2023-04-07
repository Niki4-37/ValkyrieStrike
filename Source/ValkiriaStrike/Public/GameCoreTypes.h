#pragma once

#include "GameCoreTypes.generated.h"

class UTexture2D;

//clang-format off
UENUM(BlueprintType)
enum class EMenuState : uint8
{
    MainMenu UMETA(DispayName = "Show Menu"),
    GameSettings UMETA(DispayName = "Show Setings"),
    GameConfig UMETA(DispayName = "Show Game Config")
};
//clang-format on

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMenuStateChangedSignature, EMenuState);

USTRUCT(BlueprintType)
struct FDataUnit
{
    GENERATED_USTRUCT_BODY()

    // UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    // UTexture2D* Tumb;
};

USTRUCT(BlueprintType)
struct FLevelData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelDisplayName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    UTexture2D* LevelThumb;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData&);