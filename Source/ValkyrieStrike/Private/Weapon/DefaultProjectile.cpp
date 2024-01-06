// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Weapon/DefaultProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/WeaponFXComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ADefaultProjectile::ADefaultProjectile()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->SetSphereRadius(16.f);
    SetRootComponent(CollisionComponent);
    CollisionComponent->bReturnMaterialOnMove = true;      // hit Event returns PhysMaterial
    CollisionComponent->SetCanEverAffectNavigation(true);  // collision not affect navigation
    // CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    // CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    DefaultProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    DefaultProjectileMesh->SetupAttachment(CollisionComponent);
    DefaultProjectileMesh->SetCanEverAffectNavigation(false);
    DefaultProjectileMesh->SetMobility(EComponentMobility::Movable);
    DefaultProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DefaultProjectileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("UDefaultProjectileMovementComponent");
    ProjectileMovementComponent->UpdatedComponent = RootComponent;  //???
    ProjectileMovementComponent->InitialSpeed = 1000.f;
    ProjectileMovementComponent->MaxSpeed = 1000.f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->ProjectileGravityScale = 0.f;

    ProjectileMovementComponent->SetIsReplicated(true);
    ProjectileMovementComponent->bInterpMovement = true;

    WeaponFXComponent = CreateDefaultSubobject<UWeaponFXComponent>("WeaponFXComponent");
}

void ADefaultProjectile::BeginPlay()
{
    Super::BeginPlay();

    check(CollisionComponent);

    CollisionComponent->OnComponentHit.AddDynamic(this, &ADefaultProjectile::ProjectileCollisionComponentHit);
    // CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADefaultProjectile::ComponentBeginOverlap);
}

void ADefaultProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ADefaultProjectile, DamageAmount);
    DOREPLIFETIME(ADefaultProjectile, LifeSeconds);
}

void ADefaultProjectile::ProjectileCollisionComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UGameplayStatics::ApplyPointDamage(Hit.GetActor(),             //
                                       DamageAmount,               //
                                       Hit.TraceStart,             //
                                       Hit,                        //
                                       GetInstigatorController(),  //
                                       GetOwner(),                 //
                                       nullptr);

    if (Hit.bBlockingHit /*&& Hit.GetActor()*/)
    {
        // UE_LOG(LogTemp, Display, TEXT("%s"), *Hit.GetActor()->GetName());
        WeaponFXComponent->PlayImpactFX(Hit);
        Destroy();
    }
}

void ADefaultProjectile::SetShootDirection(const FVector& Direction)
{
    ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}

void ADefaultProjectile::ComponentBeginOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // if (!OtherActor) return;
    // UGameplayStatics::ApplyPointDamage(OtherActor,                 //
    //                                    DamageAmount,               //
    //                                    SweepResult.TraceStart,     //
    //                                    SweepResult,                //
    //                                    GetInstigatorController(),  //
    //                                    GetOwner(),                 //
    //                                    nullptr);

    // Destroy();
}
