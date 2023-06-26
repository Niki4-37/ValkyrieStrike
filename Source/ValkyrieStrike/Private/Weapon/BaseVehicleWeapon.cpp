// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Weapon/BaseVehicleWeapon.h"

ABaseVehicleWeapon::ABaseVehicleWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    Platform = CreateDefaultSubobject<UStaticMeshComponent>("Platform");
    SetRootComponent(Platform);
    Platform->SetMobility(EComponentMobility::Movable);
    Platform->SetIsReplicated(true);

    Gun = CreateDefaultSubobject<UStaticMeshComponent>("Gun");
    Gun->SetMobility(EComponentMobility::Movable);
    Gun->SetIsReplicated(true);
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
            ExtraSMComponent->SetIsReplicated(true);
            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
            ExtraSMComponent->RegisterComponent();
            ExtraSMComponent->AttachToComponent(Gun, AttachmentRules, SocketName);
            ExtraSMComponent->SetStaticMesh(NewMesh);
            ExtraSMComponent->SetRelativeScale3D(FVector(1.f));
        }
    }
}

void ABaseVehicleWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseVehicleWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
