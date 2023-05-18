// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JoinSessionWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class VALKYRIESTRIKE_API UJoinSessionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void InitWidget(const FString& SessionID, int32 ConnectionNum, int32 MaxPlayers);

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* JoinSessionButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SessionInfo;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnJoinSession();
};
