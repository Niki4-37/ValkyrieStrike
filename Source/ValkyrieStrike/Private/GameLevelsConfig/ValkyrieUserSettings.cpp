// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameLevelsConfig/ValkyrieUserSettings.h"

UValkyrieUserSettings::UValkyrieUserSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    MusicSoundVolume = 0.8f;
    FXSoundVolume = 0.8f;
}

void UValkyrieUserSettings::SetMusicSoundVolume(float Value)
{
    MusicSoundVolume = FMath::Clamp(Value, 0.f, 1.f);
}

void UValkyrieUserSettings::SetFXSoundVolume(float Value)
{
    FXSoundVolume = FMath::Clamp(Value, 0.f, 1.f);
}

UValkyrieUserSettings* UValkyrieUserSettings::GetValkyrieUserSettings()
{
    return Cast<UValkyrieUserSettings>(UGameUserSettings::GetGameUserSettings());
}
