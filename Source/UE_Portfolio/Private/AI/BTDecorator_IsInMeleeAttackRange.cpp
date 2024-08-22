#include "AI/BTDecorator_IsInMeleeAttackRange.h"
#include "Controller/SAIController.h"
#include "Character/Monster/SMonsterCharacter.h"
#include "Character/SCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"



UBTDecorator_IsInMeleeAttackRange::UBTDecorator_IsInMeleeAttackRange()
{
	NodeName = TEXT("IsInMeleeAttackRange");
}

bool UBTDecorator_IsInMeleeAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	checkf(true == bResult, TEXT("Super::CalculateRawConditionValue() function has returned false."));

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	checkf(true == IsValid(AIController), TEXT("Invalid AIController."));

	ASMonsterCharacter* Monster = Cast<ASMonsterCharacter>(AIController->GetPawn());
	checkf(true == IsValid(Monster), TEXT("Invalid Monster."));

	ASCharacter* TargetPlayerCharacter = Cast<ASCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASAIController::TargetPlayerActorKey));
	if (true == IsValid(TargetPlayerCharacter) && true == TargetPlayerCharacter->IsPlayerControlled())
	{
		return Monster->GetDistanceTo(TargetPlayerCharacter) <= Monster->GetMeleeAttackStartRange();
	}

	return false;
}