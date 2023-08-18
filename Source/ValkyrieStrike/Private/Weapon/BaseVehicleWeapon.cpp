// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Weapon/BaseVehicleWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Engine.h"

ABaseVehicleWeapon::ABaseVehicleWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    Platform = CreateDefaultSubobject<UStaticMeshComponent>("Platform");
    SetRootComponent(Platform);
    Platform->SetMobility(EComponentMobility::Movable);
    Platform->SetIsReplicated(true);
    Platform->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Gun = CreateDefaultSubobject<UStaticMeshComponent>("Gun");
    Gun->SetMobility(EComponentMobility::Movable);
    Gun->SetIsReplicated(true);
    Gun->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseVehicleWeapon::SetStaticMesh(EUnitComponentType Type, UStaticMesh* NewMesh, FName SocketName)
{
    if (!NewMesh) return;
    switch (Type)
    {
        case EUnitComponentType::Platform: Platform->SetStaticMesh(NewMesh); break;
        case EUnitComponentType::Gun:
        {
            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
            Gun->AttachToComponent(Platform, AttachmentRules, SocketName);
            Gun->SetStaticMesh(NewMesh);
            break;
        }
        case EUnitComponentType::Other:
        {
            auto ExtraSMComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
            if (!ExtraSMComponent) return;
            ExtraSMComponent->SetMobility(EComponentMobility::Movable);
            ExtraSMComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            ExtraSMComponent->SetIsReplicated(true);
            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
            ExtraSMComponent->RegisterComponent();
            ExtraSMComponent->AttachToComponent(Gun, AttachmentRules, SocketName);
            ExtraSMComponent->SetStaticMesh(NewMesh);
            ExtraSMComponent->SetRelativeScale3D(FVector(1.f));
        }
    }
}

void ABaseVehicleWeapon::DefineSidePosition(float DotProduct)
{
    SidePositionModifier = DotProduct > 0.f ? 1.f : -1.f;
}

void ABaseVehicleWeapon::DefineSideMode(float InAcos)
{
    bIsSideMode = !FMath::IsNearlyZero(InAcos);
}

void ABaseVehicleWeapon::SetWeaponMovable(bool bEnable)
{
    bEnable ? GetWorld()->GetTimerManager().SetTimer(RotationTimer, this, &ABaseVehicleWeapon::RotateToTarget, 0.05f, true) : GetWorld()->GetTimerManager().ClearTimer(RotationTimer);
}

void ABaseVehicleWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseVehicleWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseVehicleWeapon, bIsSideMode);
    DOREPLIFETIME(ABaseVehicleWeapon, SidePositionModifier);
}

void ABaseVehicleWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABaseVehicleWeapon::RotateToTarget_Implementation()
{
    // if (!bPlatformCanMove) return;

    const auto AimLocation = FVector::ZeroVector;
    const auto TurretLocatiom = Platform->GetComponentLocation();
    const FRotator Direction = FRotationMatrix::MakeFromXZ(AimLocation, TurretLocatiom).Rotator();
    const FRotator FromRotation = Platform->GetComponentRotation();

    FRotator Delta = (Direction - FromRotation);

    const auto DeltaValue = bIsSideMode  //
                                ?
                                FMath::Clamp(Delta.Pitch, -15.f, 15.f)  //
                                :
                                FMath::Clamp(Delta.Yaw, -15.f, 15.f);

    const auto ValueToSet = bIsSideMode  //
                                ?
                                Platform->GetRelativeRotation().Yaw - SidePositionModifier * DeltaValue  //
                                :
                                Platform->GetRelativeRotation().Yaw + DeltaValue;

    Platform->SetRelativeRotation(FRotator(0.f, ValueToSet, 0.f));
}
