// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/Decorators/BTDecorator_HasBomb.h"
#include "AIController.h"
#include "AI/Kamikaze.h"

bool UBTDecorator_HasBomb::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto AIController = OwnerComp.GetAIOwner();
    if (!AIController) return false;
    const auto Kamikaze = AIController->GetPawn<AKamikaze>();
    return Kamikaze ? Kamikaze->HasBomb() : false;
}
