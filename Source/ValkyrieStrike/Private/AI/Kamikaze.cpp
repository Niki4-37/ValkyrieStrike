// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/Kamikaze.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

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

    DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 12, FColor::Yellow, false, 5.f);

    OnDeath();
}