// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AIAttack.h"

#include "AIController.h"
#include "OWAIBBKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "OW/Interface/OWAICharacterInterface.h"

UBTTask_AIAttack::UBTTask_AIAttack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_AIAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if(nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	IOWAICharacterInterface* AIPawn = Cast<IOWAICharacterInterface>(ControllingPawn);
	if(nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	float AttackRange = AIPawn->GetAIAttackRange();
	float Distance = ControllingPawn->GetDistanceTo(TargetPawn);

	if(Distance > AttackRange)
	{
		return EBTNodeResult::Failed;
	}

	FAICharacterAttackFinished OnAttachFinished;
	OnAttachFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	
	AIPawn->SetAIAttackDelegate(OnAttachFinished);
	AIPawn->AIAttack();

	return EBTNodeResult::InProgress;
}
