// Fill out your copyright notice in the Description page of Project Settings.


#include "GenjiAnimInstance.h"

#include "OW/ActorComponents/Ultimate/Genji/GenjiDragonbladeComponent.h"
#include "OW/Character/Genji/OWGenji.h"

UGenjiAnimInstance::UGenjiAnimInstance() : bDragonbladeActive(false), bIsClimbWall(false)
{
}

void UGenjiAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if(AOWGenji* Genji = Cast<AOWGenji>(GetOwningActor()))
	{
		GenjiRef = Genji;
	}
}

void UGenjiAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(GenjiRef)
	{
		bDragonbladeActive = GenjiRef->IsUltimateActive();
		bIsClimbWall = GenjiRef->GetIsClimbWall();
	}
}
