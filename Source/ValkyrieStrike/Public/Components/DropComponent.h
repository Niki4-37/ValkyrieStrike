// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCoreTypes.h"
#include "DropComponent.generated.h"

class APickupActor;
class UStaticMesh;
class UMaterialInterface;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VALKYRIESTRIKE_API UDropComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDropComponent();

    void DropItem();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<APickupActor> PickupActorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDropComponentElement> DropElements;

    virtual void BeginPlay() override;

private:
    float SumOfWeights{0};

    const FDropComponentElement& Roll();
};
