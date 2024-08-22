#include "AI/BTTask_RangedAttack.h"
#include "Controller/SAIController.h"
//#include "Character/SNonPlayerCharacter.h"
#include "Character/Monster/SMonsterCharacter.h"

UBTTask_RangedAttack::UBTTask_RangedAttack()
{
	bNotifyTick = true;
}

void UBTTask_RangedAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

EBTNodeResult::Type UBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

	ASMonsterCharacter* Monster = Cast<ASMonsterCharacter>(AIController->GetPawn());
	checkf(IsValid(Monster) == true, TEXT("Invalid Monster."));

	Monster->BeginRangedAttack();

	return EBTNodeResult::InProgress;
}