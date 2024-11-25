// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBarWidgetComponent.h"

#include "OW/Character/OWCharacterBase.h"
#include "OW/Status/HPComponent.h"
#include "OW/Widget/UserWidget/FloatingHPBarWidget.h"


UHPBarWidgetComponent::UHPBarWidgetComponent()
{
	SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassRef(TEXT("/Game/OW/Widget/WBP_FloatingHPBar.WBP_FloatingHPBar_C"));
	if(WidgetClassRef.Class)
	{
		SetWidgetClass(WidgetClassRef.Class);
	}
}

void UHPBarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	UFloatingHPBarWidget* HPBarWidget = Cast<UFloatingHPBarWidget>(GetUserWidgetObject());
	AOWCharacterBase* CharacterBase = Cast<AOWCharacterBase>(GetOwner());
	
	if(HPBarWidget && CharacterBase)
	{		
		UHPComponent* HPComponent = CharacterBase->GetHPComponent();
		if(HPComponent)
		{
			HPComponent->OnHPChanged.AddUObject(HPBarWidget, &UFloatingHPBarWidget::OnHPChanged);
			HPComponent->InitializeWidget();
		}
	}
}

void UHPBarWidgetComponent::SetNameText(FText InNameText)
{
	UFloatingHPBarWidget* HPBarWidget = Cast<UFloatingHPBarWidget>(GetUserWidgetObject());
	if(HPBarWidget)
	{
		HPBarWidget->SetNameText(InNameText);
	}
}
