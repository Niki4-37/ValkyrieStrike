// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/Kamikaze.h"
#include "Components/SphereComponent.h"
#include "Components/HealthComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraShakeBase.h"
#include "Decorations/DecorationActor.h"
#include "Interactables/SpawningActor.h"
#include "Net/UnrealNetwork.h"

AKamikaze::AKamikaze()
{
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->SetSphereRadius(200.f);
    CollisionComponent->SetupAttachment(RootComponent);
    CollisionComponent->SetCanEverAffectNavigation(true);  // collision not affect navigation
}

void AKamikaze::AttackEnemy(AActor* Target)
{
    Super::AttackEnemy(Target);
    if (!Target) return;
    ThrowBomb(Target->GetActorLocation());
}

void AKamikaze::NotifyActorBeginOverlap(AActor* OtherActor)
{
    if (OtherActor->IsA<ASpawningActor>() && HasAuthority())
    {
        TakeBomb();
    }
}

void AKamikaze::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        ExplosiveBomb = nullptr;
        TakeBomb();
    }
}

void AKamikaze::OnDeath()
{
    Super::OnDeath();

    ThrowBomb(FVector::ZeroVector);
}

void AKamikaze::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AKamikaze, ExplosiveBomb);
}

void AKamikaze::TakeBomb()
{
    if (ExplosiveBomb) return;
    ExplosiveBomb = GetWorld()->SpawnActor<ADecorationActor>(BombClass, GetActorTransform());
    if (!ExplosiveBomb) return;
    ExplosiveBomb->SetOwner(this);
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    ExplosiveBomb->AttachToComponent(GetMesh(), AttachmentRules, BackpackSocketName);
}

void AKamikaze::ThrowBomb(const FVector& ToLocation)
{
    if (!ExplosiveBomb) return;
    FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, false);
    ExplosiveBomb->DetachFromActor(DetachmentRules);
    ExplosiveBomb->SetDestructionTimer(2.f);
    ToLocation.Equals(FVector::ZeroVector) ? ExplosiveBomb->Throw_Multicast() : ExplosiveBomb->ThrowTo_Multicast(ToLocation);
    ExplosiveBomb = nullptr;
}
