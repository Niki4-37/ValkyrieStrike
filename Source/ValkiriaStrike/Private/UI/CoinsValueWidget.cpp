// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/CoinsValueWidget.h"
#include "Components/TextBlock.h"
#include "Components/WorkshopComponent.h"

void UCoinsValueWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UCoinsValueWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }

    if (CoinsValue_Text)
    {
        CoinsValue_Text->SetText(FText::AsNumber(0));
    }
}

void UCoinsValueWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn) return;
    const auto WorkshopComponent = NewPawn->FindComponentByClass<UWorkshopComponent>();
    if (WorkshopComponent && !WorkshopComponent->OnCoinsChanged.IsBoundToObject(this))
    {
        WorkshopComponent->OnCoinsChanged.AddUObject(this, &UCoinsValueWidget::OnCoinsChanged);
    }
}

void UCoinsValueWidget::OnCoinsChanged(int32 Value)
{
    CoinsValue_Text->SetText(FText::AsNumber(Value));
}
