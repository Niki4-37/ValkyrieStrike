// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecorationActor.generated.h"

UCLASS()
class VALKIRIASTRIKE_API ADecorationActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ADecorationActor();

    void SetupDecoration(UStaticMesh* Mesh);

protected:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* ActorRootComp;

    UPROPERTY(EditDefaultsOnly)
    UStaticMeshComponent* MeshComp;

    virtual void BeginPlay() override;
};
