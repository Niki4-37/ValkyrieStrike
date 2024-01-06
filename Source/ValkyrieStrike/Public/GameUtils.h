#pragma once

#include "GameCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine.h"
#include "ValkyrieGameInstance.h"

class GameUtils
{
public:
    static void PlayMusic(const UObject* WorldContextObject, EMusicTheme Theme)
    {
        const auto ValkyrieGameInstance = Cast<UValkyrieGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
        if (!ValkyrieGameInstance) return;

        UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
        if (!World) return;

        const auto MusicToPlay = ValkyrieGameInstance->GetGameMusic(Theme);
        const auto AudioComponent = UGameplayStatics::CreateSound2D(World, MusicToPlay);
        if (AudioComponent && !AudioComponent->IsPlaying())
        {
            AudioComponent->Play();
        }
    }
};
