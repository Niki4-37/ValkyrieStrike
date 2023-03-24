// Copyright Epic Games, Inc. All Rights Reserved.

#include "ValkiriaStrikeGameMode.h"
#include "Player/DefaultWeeledVehicle.h"

AValkiriaStrikeGameMode::AValkiriaStrikeGameMode()
{
    DefaultPawnClass = ADefaultWeeledVehicle::StaticClass();
    //HUDClass = AValkiriaStrikeHud::StaticClass();
}
