// Fill out your copyright notice in the Description page of Project Settings.


#include "OWAIController.h"

#include "OWAIBBKey.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


AOWAIController::AOWAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/OW/StandardBot/AI/BB_StandardBot_Fire.BB_StandardBot_Fire'"));
	if(BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/OW/StandardBot/AI/BT_StandardBot_Fire.BT_StandardBot_Fire'"));
	if(BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AOWAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if(UseBlackboard(BBAsset, BlackboardPtr))
	{
		BlackboardPtr->SetValueAsVector(BBKEY_STARTPOS, GetPawn()->GetActorLocation());
		BlackboardPtr->SetValueAsVector(BBKEY_STARTDIR, GetPawn()->GetActorForwardVector());
		
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AOWAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if(BTComponent)
	{
		BTComponent->StopTree();
	}
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if(UseBlackboard(BBAsset, BlackboardPtr))
	{
		const FVector StartPos = BlackboardPtr->GetValueAsVector(BBKEY_STARTPOS);
		BlackboardPtr->SetValueAsVector(BBKEY_PATROLPOS, StartPos);
		BlackboardPtr->SetValueAsObject(BBKEY_TARGET, nullptr);
	}
}

void AOWAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}