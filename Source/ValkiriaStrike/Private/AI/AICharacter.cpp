// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/AICharacter.h"
#include "Components/HealthComponent.h"

AAICharacter::AAICharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
}

void AAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAICharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent) HealthComponent->OnDeath.AddUObject(this, &AAICharacter::OnDeath);
}

void AAICharacter::OnDeath()
{
    PlayAnimMontage(DeathMontage);
}
