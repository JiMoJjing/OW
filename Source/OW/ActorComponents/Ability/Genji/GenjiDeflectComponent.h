// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OW/ActorComponents/Ability/AbilityComponent.h"
#include "GenjiDeflectComponent.generated.h"


class AOWGenji;
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

	void DurationStart();
	void DurationEnd();

	UFUNCTION()
	void OnDeflectBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	TObjectPtr<AOWGenji> GenjiRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_Deflect, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> DeflectBoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_Deflect, meta = (AllowPrivateAccess = "true"))
	float DurationTime;

	UPROPERTY()
	TScriptInterface<IOWPlayerTraceInterface> PlayerTraceInterface;

	FTimerHandle DurationTimerHandle;

	TMap<uint8, FName> DeflectMontageSection;
	uint8 DeflectMontageSectionIndex;
};
