// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OW/ActorComponents/Ability/AbilityComponent.h"
#include "GenjiDeflectComponent.generated.h"


class IOWPlayerTraceInterface;
class UBoxComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UGenjiDeflectComponent : public UAbilityComponent
{
	GENERATED_BODY()

public:
	UGenjiDeflectComponent();

	virtual void InitializeComponent() override;

protected:
	virtual void BeginPlay() override;

private:
	virtual void AbilityStart() override;
	virtual void AbilityEnd() override;

	virtual void OtherAbilityStart(EAbilityType OtherAbilityType) override;

	void DeflectStartSetting();
	void DeflectEndSetting();

	void GetDeflectDirection(const FVector& InStartLocation, FVector& InDirectionVector);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Deflect, meta = (AllowPrivateAccess = "true"))
	float DurationTime;

	FTimerHandle DurationTimerHandle;

	void DurationStart();
	void DurationEnd();
	
	UPROPERTY()
	uint8 DeflectMontageSectionIndex;

	UPROPERTY()
	TMap<uint8, FName> DeflectMontageSection;

// Caching
	UPROPERTY()
	TScriptInterface<IOWPlayerTraceInterface> PlayerTraceInterface;


// Overlap Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Deflect, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> DeflectBoxComponent;

	UFUNCTION()
	void OnDeflectBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
