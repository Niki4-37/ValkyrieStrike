// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BattleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBattleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VALKYRIESTRIKE_API IBattleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AttackEnemy(AActor* Target);
};
