// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ValkiriaPlayerState.generated.h"

UCLASS()
class VALKIRIASTRIKE_API AValkiriaPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite)
    uint8 Lives;

    // UPROPERTY()

    virtual void CopyProperties(APlayerState* PlayerState) override;
    virtual void OverrideWith(APlayerState* PlayerState) override;
};
