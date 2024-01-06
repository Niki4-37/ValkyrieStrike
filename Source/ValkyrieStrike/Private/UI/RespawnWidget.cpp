// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/RespawnWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstance.h"
#include "Components/RespawnComponent.h"

void URespawnWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (const auto RespawnComponent = GetOwningPlayer()->FindComponentByClass<URespawnComponent>())
    {
        RespawnComponent->OnStartRespawn.AddUObject(this, &URespawnWidget::OnStartRespawn);
    }

    if (CountdownBarImage && CountdownBarImage->GetDynamicMaterial())
    {
        // CountdownBarImage->SetBrushSize(FVector2D(150.f));
        CountdownBarMaterial = CountdownBarImage->GetDynamicMaterial();
        CountdownBarMaterial->SetScalarParameterValue(SectionsParamName, 0);
        CountdownBarMaterial->SetVectorParameterValue(BaseColorParamName, FLinearColor(FColor::Red));
    }
}

void URespawnWidget::OnStartRespawn(float RespawnDelay)
{
    if (GetWorld()->GetTimerManager().IsTimerActive(RespawnTimer) || !CountdownBarMaterial || !RespawnDelay) return;

    GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &URespawnWidget::RespawnEnd, RespawnDelay);
    CountdownBarMaterial->SetScalarParameterValue(PercentParamName, 1.f);

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda(
        [&, RespawnDelay]()
        {
            if (!CountdownBarMaterial) return;
            const float RemainingReloadingTime = GetWorld()->GetTimerManager().GetTimerRemaining(RespawnTimer);
            CountdownBarMaterial->SetScalarParameterValue(PercentParamName, RemainingReloadingTime / RespawnDelay);
        });

    const float ProgressTimerRate = RespawnDelay / 100.f;
    GetWorld()->GetTimerManager().SetTimer(ProgressTimer, TimerDelegate, ProgressTimerRate, true);
}

void URespawnWidget::RespawnEnd()
{
    GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
    GetWorld()->GetTimerManager().ClearTimer(ProgressTimer);
}
