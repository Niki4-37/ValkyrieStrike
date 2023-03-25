// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "ValkiriaStrikeWheelRear.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

UValkiriaStrikeWheelRear::UValkiriaStrikeWheelRear()
{
    ShapeRadius = 35.f;
    ShapeWidth = 10.0f;
    bAffectedByHandbrake = true;
    SteerAngle = 0.f;
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS
