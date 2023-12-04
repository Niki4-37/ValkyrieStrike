// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawningActor.generated.h"

UCLASS()
class VALKYRIESTRIKE_API ASpawningActor : public AActor
{
    GENERATED_BODY()

public:
    ASpawningActor();

    void SpawnEnemy(UClass* Class);

protected:
    virtual void BeginPlay() override;
};
