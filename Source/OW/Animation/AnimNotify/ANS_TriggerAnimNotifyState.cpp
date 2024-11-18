// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_TriggerAnimNotifyState.h"

#include "OW/Interface/OWTriggerAnimNotifyInterface.h"

void UANS_TriggerAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if(IOWTriggerAnimNotifyInterface* OWTriggerAnimNotifyInterface = Cast<IOWTriggerAnimNotifyInterface>(MeshComp->GetOwner()))
	{
		OWTriggerAnimNotifyInterface->TriggerAnimNotifyState(FrameDeltaTime);
	}
}

void UANS_TriggerAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(IOWTriggerAnimNotifyInterface* OWTriggerAnimNotifyInterface = Cast<IOWTriggerAnimNotifyInterface>(MeshComp->GetOwner()))
	{
		OWTriggerAnimNotifyInterface->TriggerAnimNotifyEnd();
	}
}