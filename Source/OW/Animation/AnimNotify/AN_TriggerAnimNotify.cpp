// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_TriggerAnimNotify.h"

#include "OW/Interface/OWTriggerAnimNotifyInterface.h"

void UAN_TriggerAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(IOWTriggerAnimNotifyInterface* OWTriggerAnimNotifyInterface = Cast<IOWTriggerAnimNotifyInterface>(MeshComp->GetOwner()))
	{
		OWTriggerAnimNotifyInterface->TriggerAnimNotify();
	}
}
