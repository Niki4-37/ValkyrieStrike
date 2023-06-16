// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/WheelManagerComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UWheelManagerComponent::UWheelManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

bool UWheelManagerComponent::GetVelocityAverage(FVector& VelocityAverage) const
{
    FVector VelocityCache{FVector::ZeroVector};
    int32 TotalWheels{0};
    for (const auto& WheelGroup : WheelsGroups)
    {
        for (const auto& Wheel : WheelGroup.WheelsArray)
        {
            VelocityCache += Wheel.WheelVelocity;
            ++TotalWheels;
        }
    }
    VelocityAverage = TotalWheels ? (VelocityCache / TotalWheels) : FVector::ZeroVector;

    return true;
}

bool UWheelManagerComponent::GetWheelsGroups(TArray<FWheelsGroup>& OutWheelsGroup) const
{
    OutWheelsGroup = WheelsGroups;
    return true;
}

void UWheelManagerComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UWheelManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    ManageWheels();
}

bool UWheelManagerComponent::WheelSphereTrace(const FWheelData& Wheel, FHitResult& Hit)
{
    if (!GetWorld() || !Wheel.WheelRoot) return false;

    const auto Start = Wheel.WheelRoot->GetComponentLocation();
    const auto End = Start - Wheel.WheelRoot->GetUpVector() * (WheelRadius + WheelMaxContactDistance);

    UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),                    //
                                                      Start,                         //
                                                      End,                           //
                                                      WheelRadius,                   //
                                                      TreceForObjectTypes,           //
                                                      false,                         //
                                                      {GetOwner()},                  //
                                                      EDrawDebugTrace::ForOneFrame,  //
                                                      Hit,                           //
                                                      true);

    return Hit.bBlockingHit;
}

void UWheelManagerComponent::ManageWheels()
{
    bIsWheelContact = false;
    // use for each loop
    // for (int WheelsGroupIndex = 0; WheelsGroupIndex < WheelsGroups.Num(); ++WheelsGroupIndex)
    if (!WheelsGroups.Num()) return;
    for (auto& WheelGroup : WheelsGroups)
    {
        ComputeWeelGroup(WheelGroup);
        // ComputeWeelGroup(WheelsGroupIndex, WheelsGroups[WheelsGroupIndex]);
    }
}

void UWheelManagerComponent::ComputeWeelGroup(FWheelsGroup& WheelGroup)
{
    for (auto& CurrentWheel : WheelGroup.WheelsArray)
    {
        FHitResult Hit;
        FVector WheelVelocity{FVector::ZeroVector};
        bIsWheelContact = WheelSphereTrace(CurrentWheel, Hit);
        if (!bIsWheelContact || !CurrentWheel.WheelRoot || !WheelGroup.RootSimulatedComponent)
        {
            /* reset wheel velocity */
            CurrentWheel.WheelVelocity = WheelVelocity;
            continue;
        }

        const float ContactDistance = FMath::Clamp(Hit.Distance, WheelMinContactDistance, WheelMaxContactDistance);

        const FVector Point = CurrentWheel.WheelRoot->GetComponentLocation();
        const FVector LinearVelocityAtPoint = WheelGroup.RootSimulatedComponent->GetPhysicsLinearVelocityAtPoint(Point, WheelGroup.RootSimulatedBoneName);
        const FTransform CurrentWheelInverseTransform = CurrentWheel.WheelRoot->GetComponentTransform().Inverse();

        WheelVelocity = CurrentWheelInverseTransform.TransformVectorNoScale(LinearVelocityAtPoint);

        const auto Force                                                                                                                  //
            = GetSpringForce(WheelVelocity.Z, WheelGroup.SuspensionDumping, WheelGroup.SuspensionStiffness, ContactDistance, Hit.Normal)  //
              + GetSideFriction(WheelVelocity.Y, CurrentWheel.WheelRoot->GetRightVector())                                                //
              + GetMoveForce(WheelVelocity.X, Hit.Normal.Z, CurrentWheel.WheelRoot->GetForwardVector());
        WheelGroup.RootSimulatedComponent->AddForceAtLocation(Force, CurrentWheel.WheelRoot->GetComponentLocation(), WheelGroup.RootSimulatedBoneName);

        CurrentWheel.WheelVelocity = WheelVelocity;
        CurrentWheel.LastContactDistance = ContactDistance;
    }
}

FVector UWheelManagerComponent::GetSpringForce(float WheelVelocityZValue, float Dumping, float Stiffness, float ContactDistance, const FVector& ContactNormalPoint)
{
    return ((WheelVelocityZValue * Dumping * -0.8f + (WheelDefaultDistance - ContactDistance) * Stiffness * 15.f)) * ContactNormalPoint;
}

FVector UWheelManagerComponent::GetSideFriction(float WheelVelocityYValue, const FVector& RightVector)
{
    return FMath::Clamp(WheelVelocityYValue, -80.f, 80.f) * -5000.f * WheelFrictionMultipler * RightVector;
}

FVector UWheelManagerComponent::GetMoveForce(float WheelVelocityXValue, float ContactNormalPointZValue, const FVector& ForwardVector)
{
    const float ValueWitoutBrake = FMath::Clamp((ControlInput * -1.f * WheelMaxSpeed + WheelVelocityXValue), -800.f, 800.f);
    const float ValueWithBrake = FMath::Clamp(WheelVelocityXValue, -160.f, 160.f) * 10;

    const float ResultValue = ContactNormalPointZValue * WheelMoveForceMultipler * -100.f * (bIsBreake ? ValueWithBrake : ValueWitoutBrake);
    return ForwardVector * ResultValue;
}
