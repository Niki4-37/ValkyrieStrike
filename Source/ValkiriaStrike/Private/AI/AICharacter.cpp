// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/AICharacter.h"
#include "AI/AIEnemyController.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DropComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"

AAICharacter::AAICharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
    DropComponent = CreateDefaultSubobject<UDropComponent>("DropComponent");

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AAIEnemyController::StaticClass();

    bUseControllerRotationYaw = false;

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
}

void AAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAICharacter::AttackEnemy(AActor* AimActor) {}

void AAICharacter::BeginPlay()
{
    Super::BeginPlay();
    check(HealthComponent);
    check(DropComponent);
    HealthComponent->OnDeath.AddUObject(this, &AAICharacter::OnDeath);
    Tags.Add("Enemy");
}

void AAICharacter::OnDeath()
{
    /** handled on server */
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    PlayAnimMontage_Multicast(DeathMontage);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    const auto STUController = Cast<AAIController>(Controller);
    if (STUController && STUController->BrainComponent)
    {
        STUController->BrainComponent->Cleanup();
    }
    DropComponent->DropItem();  // Server

    Tags.Empty();
}

void AAICharacter::PlayAnimMontage_Multicast_Implementation(UAnimMontage* Animation)
{
    PlayAnimMontage(Animation);
}
