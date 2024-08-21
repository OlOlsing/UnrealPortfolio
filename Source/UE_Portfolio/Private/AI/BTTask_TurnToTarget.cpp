#include "AI/BTTask_TurnToTarget.h"
#include "Controller/SAIController.h"
#include "Character/SCharacter.h"
//#include "Character/SNonPlayerCharacter.h"
//#include "Character/SPlayerCharacter.h"
#include "Character/Player/SPlayerCharacterBase.h"
#include "Character/Monster/SMonsterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("TurnToTargetActor");
}

//EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//{
//	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
//
//	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
//	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));
//
//	ASNonPlayerCharacter* NPC = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
//	checkf(IsValid(NPC) == true, TEXT("Invalid NPC."));
//
//	if (ASPlayerCharacter* TargetPC = Cast<ASPlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AIController->TargetActorKey)))
//	{
//		FVector LookVector = TargetPC->GetActorLocation() - NPC->GetActorLocation();
//		LookVector.Z = 0.f;
//		FRotator TargetRotation = FRotationMatrix::MakeFromX(LookVector).Rotator();
//		NPC->SetActorRotation(FMath::RInterpTo(NPC->GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 2.f));
//
//		return Result = EBTNodeResult::Succeeded;
//	}
//
//	return Result = EBTNodeResult::Failed;
//}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

	ASMonsterCharacter* Monster = Cast<ASMonsterCharacter>(AIController->GetPawn());
	checkf(IsValid(Monster) == true, TEXT("Invalid Monster."));

	if (ASPlayerCharacterBase* TargetPC = Cast<ASPlayerCharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AIController->TargetActorKey)))
	{
		FVector LookVector = TargetPC->GetActorLocation() - Monster->GetActorLocation();
		LookVector.Z = 0.f;
		FRotator TargetRotation = FRotationMatrix::MakeFromX(LookVector).Rotator();
		Monster->SetActorRotation(FMath::RInterpTo(Monster->GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 2.f));
		return Result = EBTNodeResult::Succeeded;
	}

	return Result = EBTNodeResult::Failed;
}