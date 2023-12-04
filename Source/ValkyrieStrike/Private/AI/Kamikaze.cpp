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
#include "Net/UnrealNetwork.h"

#include "DrawDebugHelpers.h"
#include "Engine.h"

AKamikaze::AKamikaze()
{
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->SetSphereRadius(200.f);
    CollisionComponent->SetupAttachment(RootComponent);
    CollisionComponent->SetCanEverAffectNavigation(true);  // collision not affect navigation
}

void AKamikaze::NotifyActorBeginOverlap(AActor* OtherActor)
{
    if (!ExplosiveBomb || !OtherActor || !OtherActor->ActorHasTag("Player")) return;

    // const auto PlayerController = OtherActor->GetInstigatorController<APlayerController>();
    // if (!PlayerController || !PlayerController->PlayerCameraManager) return;
    // checkf(CameraShake, TEXT("CameraShake not define!"));
    // PlayerController->PlayerCameraManager->StartCameraShake(CameraShake);

    FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, false);
    ExplosiveBomb->DetachFromActor(DetachmentRules);
    ExplosiveBomb->SetDestructionTimer(2.f);
    ExplosiveBomb->Throw_Multicast();
    ExplosiveBomb = nullptr;
}

void AKamikaze::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        ExplosiveBomb = GetWorld()->SpawnActorDeferred<ADecorationActor>(ADecorationActor::StaticClass(), GetActorTransform());
        if (ExplosiveBomb)
        {
            ExplosiveBomb->SetupDecoration(BombMesh);
            ExplosiveBomb->IsDealsDamage(true, Damage, Radius);
            ExplosiveBomb->SetDestructionEffect(ExplodeEffect);
            ExplosiveBomb->SetOwner(this);
            ExplosiveBomb->FinishSpawning(GetActorTransform());
            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
            ExplosiveBomb->AttachToComponent(GetMesh(), AttachmentRules, BackpackSocketName);
        }
    }
}

void AKamikaze::OnDeath()
{
    Super::OnDeath();

    if (ExplosiveBomb)
    {
        FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, false);
        ExplosiveBomb->DetachFromActor(DetachmentRules);
        ExplosiveBomb->SetDestructionTimer(2.f);
        ExplosiveBomb->Throw_Multicast();
        ExplosiveBomb = nullptr;
    }
}

void AKamikaze::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AKamikaze, ExplosiveBomb);
}
