// Fill out your copyright notice in the Description page of Project Settings.


#include "HitMarkerWidget.h"

void UHitMarkerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FWidgetAnimationDynamicEvent HitMarkerAnimationEvent;

	HitMarkerAnimationEvent.Clear();
	HitMarkerAnimationEvent.BindUFunction(this, TEXT("HitMarkerAnimationEnded"));

	PlayAnimation(HitMarkerWidgetAnimation, 0, 1);
}

void UHitMarkerWidget::HitMarkerAnimationEnded()
{
	RemoveFromParent();
	Destruct();
}
