// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "ValkiriaStrikeWheelFront.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

UValkiriaStrikeWheelFront::UValkiriaStrikeWheelFront()
{
    ShapeRadius = 35.f;
    ShapeWidth = 10.0f;
    bAffectedByHandbrake = false;
    SteerAngle = 50.f;
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS
