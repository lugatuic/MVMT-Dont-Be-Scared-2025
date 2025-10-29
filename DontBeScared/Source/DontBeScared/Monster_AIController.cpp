// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_AIController.h"

#include "Monster.h"

void AMonster_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AMonster* const monster = Cast<AMonster>(InPawn))
	{
		if (UBehaviorTree* const tree = monster->GetBehaviorTree())
		{
			UBlackboardComponent *b;
			
			UseBlackboard(tree->BlackboardAsset, b);

			Blackboard = b;
			RunBehaviorTree(tree);
		}
	}
}
