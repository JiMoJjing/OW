// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HPBarWidgetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UHPBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UHPBarWidgetComponent();

protected:
	virtual void BeginPlay() override;

public:
	void SetNameText(FText InNameText);

};
