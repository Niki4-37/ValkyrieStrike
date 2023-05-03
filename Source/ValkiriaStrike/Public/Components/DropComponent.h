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
class VALKIRIASTRIKE_API UDropComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDropComponent();

    void DropItem();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<APickupActor> PickupActorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DropChance{0.5f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* PickupMesh{nullptr};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UMaterialInterface* Material;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FInteractionData DropData;

    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
