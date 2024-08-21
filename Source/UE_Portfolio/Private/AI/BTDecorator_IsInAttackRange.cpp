#include "AI/BTDecorator_IsInAttackRange.h"
#include "Controller/SAIController.h"
//#include "Character/SNonPlayerCharacter.h"
#include "Character/Monster/SMonsterCharacter.h"
#include "Character/SCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

const float UBTDecorator_IsInAttackRange::AttackRange(200.f);

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("IsInAttackRange");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	checkf(true == bResult, TEXT("Super::CalculateRawConditionValue() function has returned false."));

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	checkf(true == IsValid(AIController), TEXT("Invalid AIController."));

	ASMonsterCharacter* Monster = Cast<ASMonsterCharacter>(AIController->GetPawn());
	checkf(true == IsValid(Monster), TEXT("Invalid Monster."));

	ASCharacter* TargetPlayerCharacter = Cast<ASCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASAIController::TargetActorKey));
	if (true == IsValid(TargetPlayerCharacter) && true == TargetPlayerCharacter->IsPlayerControlled())
	{
		return Monster->GetDistanceTo(TargetPlayerCharacter) <= AttackRange;
	}

	return false;
}