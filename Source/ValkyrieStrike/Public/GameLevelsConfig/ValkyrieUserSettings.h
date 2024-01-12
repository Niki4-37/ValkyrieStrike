// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "ValkyrieUserSettings.generated.h"

UCLASS()
class VALKYRIESTRIKE_API UValkyrieUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UValkyrieUserSettings(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetMusicSoundVolume(float Value);

	UFUNCTION(BlueprintCallable)
	void SetFXSoundVolume(float Value);

	UFUNCTION(BlueprintPure)
	float GetMusicSoundVolume() const { return MusicSoundVolume; };

	UFUNCTION(BlueprintPure)
	float GetFXSoundVolume() const { return FXSoundVolume; };

	UFUNCTION(BlueprintCallable)
	static UValkyrieUserSettings* GetValkyrieUserSettings();

protected:
	UPROPERTY(Config)
	float MusicSoundVolume;

	UPROPERTY(Config)
	float FXSoundVolume;
	
};
