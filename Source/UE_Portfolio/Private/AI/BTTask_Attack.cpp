#include "AI/BTTask_Attack.h"
#include "Controller/SAIController.h"
//#include "Character/SNonPlayerCharacter.h"
#include "Character/Monster/SMonsterCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

//void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
//
//	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
//	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));
//
//	ASNonPlayerCharacter* NPC = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
//	checkf(IsValid(NPC) == true, TEXT("Invalid NPC."));
//
//	if (NPC->bIsNowAttacking == false)
//	{
//		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
//	}
//}
//
//EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//{
//	Super::ExecuteTask(OwnerComp, NodeMemory);
//
//	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
//	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));
//
//	ASNonPlayerCharacter* NPC = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
//	checkf(IsValid(NPC) == true, TEXT("Invalid NPC."));
//
//	NPC->BeginAttack();
//
//	return EBTNodeResult::InProgress;
//}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

	ASMonsterCharacter* Monster = Cast<ASMonsterCharacter>(AIController->GetPawn());
	checkf(IsValid(Monster) == true, TEXT("Invalid Monster."));

	if (Monster->bIsNowAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

	ASMonsterCharacter* Monster = Cast<ASMonsterCharacter>(AIController->GetPawn());
	checkf(IsValid(Monster) == true, TEXT("Invalid Monster."));

	Monster->BeginAttack();

	return EBTNodeResult::InProgress;
}