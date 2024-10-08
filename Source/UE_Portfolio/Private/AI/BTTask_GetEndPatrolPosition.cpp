#include "AI/BTTask_GetEndPatrolPosition.h"
#include "Controller/SAIController.h"
//#include "Character/SNonPlayerCharacter.h"
#include "Character/Monster/SMonsterCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

//UBTTask_GetEndPatrolPosition::UBTTask_GetEndPatrolPosition()
//{
//	NodeName = TEXT("GetEndPatrolPosition"); // Behavior Tree에 보일 노드 이름.
//}
//
//EBTNodeResult::Type UBTTask_GetEndPatrolPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//{
//	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
//
//	if (EBTNodeResult::Failed == Result)
//	{
//		return Result;
//	}
//
//	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
//	checkf(IsValid(AIController) == true, TEXT("Invalid AIController"));
//
//	ASNonPlayerCharacter* NPC = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
//	checkf(IsValid(NPC) == true, TEXT("Invalid NPC"));
//
//	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(NPC->GetWorld());
//	checkf(IsValid(NavigationSystem) == true, TEXT("Invalid NavigationSystem"));
//
//	FVector StartPatrolPosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ASAIController::StartPatrolPositionKey);
//	FNavLocation EndPatrolLocation;
//	if (true == NavigationSystem->GetRandomPointInNavigableRadius(StartPatrolPosition, AIController->PatrolRadius, EndPatrolLocation))
//	{
//		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ASAIController::EndPatrolPositionKey, EndPatrolLocation.Location);
//		return Result = EBTNodeResult::Succeeded;
//	}
//
//	return Result;
//}

UBTTask_GetEndPatrolPosition::UBTTask_GetEndPatrolPosition()
{
	NodeName = TEXT("GetEndPatrolPosition"); // Behavior Tree에 보일 노드 이름.
}

EBTNodeResult::Type UBTTask_GetEndPatrolPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (EBTNodeResult::Failed == Result)
	{
		return Result;
	}

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController"));

	ASMonsterCharacter* Monster = Cast<ASMonsterCharacter>(AIController->GetPawn());
	checkf(IsValid(Monster) == true, TEXT("Invalid Monster"));

	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(Monster->GetWorld());
	checkf(IsValid(NavigationSystem) == true, TEXT("Invalid NavigationSystem"));

	FVector StartPatrolPosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ASAIController::StartPatrolPositionKey);
	FNavLocation EndPatrolLocation;
	if (true == NavigationSystem->GetRandomPointInNavigableRadius(StartPatrolPosition, AIController->PatrolRadius, EndPatrolLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ASAIController::EndPatrolPositionKey, EndPatrolLocation.Location);
		return Result = EBTNodeResult::Succeeded;
	}

	return Result;
}