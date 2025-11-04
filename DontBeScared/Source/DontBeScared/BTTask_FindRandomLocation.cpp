// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"

#include "Monster_AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	// Set name of node when dragged into behavior tree
	NodeName = "Find Random Location In NavMesh";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the controller
	if (auto* const cont = Cast<AMonster_AIController>(OwnerComp.GetAIOwner()))
	{
		// Get the pawn from the controller
		if (APawn* const creature = cont->GetPawn())
		{
			// Get the location and then ensure navigation works
			auto const Origin = creature->GetActorLocation();

			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				// Create a holder for our navigation to run on
				FNavLocation Loc;

				// Load a random point into our navigation data
				if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc))
				{
					// Update the location for the blackboard value
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
				}

				// Finish and return that we're done!
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	// Somewhere along the long line, we failed. Add logging later if not in game jam mode
	return EBTNodeResult::Failed;
}
