// Fill out your copyright notice in the Description page of Project Settings.


#include "GenjiDragonbladeComponent.h"

#include "OW/Character/OWCharacterPlayable.h"


UGenjiDragonbladeComponent::UGenjiDragonbladeComponent() : DragonbladeDuration(6.f)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UltimateMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Genji_NormaltoDragonblade.AM_Genji_NormaltoDragonblade'"));
	if(UltimateMontageRef.Object)
	{
		UltimateMontage = UltimateMontageRef.Object;
	}
}

void UGenjiDragonbladeComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UGenjiDragonbladeComponent::StartUltimate()
{
	Super::StartUltimate();
	CharacterPlayable->OnAnimNotifyEnd.AddUObject(this, &UGenjiDragonbladeComponent::EndUltimate);
	PlayUltimateMontage();
}

void UGenjiDragonbladeComponent::EndUltimate()
{
	Super::EndUltimate();
	CharacterPlayable->OnAnimNotifyEnd.RemoveAll(this);
	DragonbladeModeStart();
}

void UGenjiDragonbladeComponent::DragonbladeModeStart()
{
	DragonbladeDurationStart();
}

void UGenjiDragonbladeComponent::DragonbladeDurationStart()
{
	CharacterPlayable->GetWorldTimerManager().SetTimer(DragonbladeDurationTimerHandle, this, &UGenjiDragonbladeComponent::DragonbladeDurationEnd, DragonbladeDuration, false);
}

void UGenjiDragonbladeComponent::DragonbladeDurationEnd()
{
	CharacterPlayable->SetUltimateActive(false);
	UE_LOG(LogTemp, Warning, TEXT("Dragonblade End!"));
}
