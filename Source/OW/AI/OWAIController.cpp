// Fill out your copyright notice in the Description page of Project Settings.


#include "OWAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"

AOWAIController::AOWAIController()
{

}

void AOWAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	
}

void AOWAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if(BTComponent)
	{
		BTComponent->StopTree();
	}
}

void AOWAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
