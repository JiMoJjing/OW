// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToStartDirection.h"

#include "AIController.h"
#include "OWAIBBKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "OW/Interface/OWAICharacterInterface.h"

UBTTask_TurnToStartDirection::UBTTask_TurnToStartDirection()
{
	NodeName = TEXT("TurnToStartDir");
}

EBTNodeResult::Type UBTTask_TurnToStartDirection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IOWAICharacterInterface* AIPawn = Cast<IOWAICharacterInterface>(ControllingPawn);
	if(nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	float TurnSpeed = AIPawn->GetAITrunSpeed();
	FVector StartDirectionVec = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_STARTDIR);
	StartDirectionVec.Z = 0.f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(StartDirectionVec).Rotator();

	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));
	
	return EBTNodeResult::Succeeded;
}
