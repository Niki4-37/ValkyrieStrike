// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/Kamikaze.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraShakeBase.h"

#include "DrawDebugHelpers.h"

AKamikaze::AKamikaze()
{
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->SetSphereRadius(200.f);
    CollisionComponent->SetupAttachment(RootComponent);
    CollisionComponent->SetCanEverAffectNavigation(true);  // collision not affect navigation

    BombMesh = CreateDefaultSubobject<UStaticMeshComponent>("BombMesh");
    BombMesh->SetupAttachment(GetMesh(), WeaponSocketName);
    BombMesh->SetCanEverAffectNavigation(false);
    BombMesh->SetMobility(EComponentMobility::Movable);
}

void AKamikaze::NotifyActorBeginOverlap(AActor* OtherActor)
{
    if (bIsExploded || !OtherActor || !OtherActor->ActorHasTag("Player")) return;

    bIsExploded = true;

    UGameplayStatics::ApplyRadialDamage(GetWorld(),                  //
                                        Damage,                      //
                                        GetActorLocation(),          //
                                        Radius,                      //
                                        UDamageType::StaticClass(),  //
                                        {});

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExpoleEffect, GetActorLocation(), FRotator::ZeroRotator);

    OnDeath();

    const auto PlayerController = OtherActor->GetInstigatorController<APlayerController>();
    if (!PlayerController || !PlayerController->PlayerCameraManager) return;
    checkf(CameraShake, TEXT("CameraShake not define!"));
    PlayerController->PlayerCameraManager->StartCameraShake(CameraShake);
}