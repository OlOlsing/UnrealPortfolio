#include "AI/BTService_DetectTargetPlayer.h"
#include "Controller/SAIController.h"
//#include "Character/SNonPlayerCharacter.h"
#include "Character/Monster/SMonsterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/SCharacter.h"

UBTService_DetectTargetPlayer::UBTService_DetectTargetPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = TEXT("DetectTargetPlayer");
	Interval = 1.f;
}

void UBTService_DetectTargetPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASAIController* AIC = Cast<ASAIController>(OwnerComp.GetAIOwner());
	if (IsValid(AIC) == true)
	{
		ASMonsterCharacter* Monster = Cast<ASMonsterCharacter>(AIC->GetPawn());
		if (IsValid(Monster) == true)
		{
			UWorld* World = Monster->GetWorld();
			if (IsValid(World) == true)
			{
				FVector CenterPosition = Monster->GetActorLocation();
				float DetectRadius = Monster->GetDetectRadius();
				TArray<FOverlapResult> OverlapResults;
				FCollisionQueryParams CollisionQueryParams(NAME_None, false, Monster);
				bool bResult = World->OverlapMultiByChannel(
					OverlapResults,
					CenterPosition,
					FQuat::Identity,
					ECollisionChannel::ECC_GameTraceChannel2, // ¡÷¿«
					FCollisionShape::MakeSphere(1000.f * DetectRadius), // Test
					CollisionQueryParams
				);

				if (bResult == true)
				{
					for (auto const& OverlapResult : OverlapResults)
					{
						ASCharacter* PC = Cast<ASCharacter>(OverlapResult.GetActor());
						if (IsValid(PC) == true)
						{
							if (PC->GetController()->IsPlayerController() == true)
							{
								OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASAIController::TargetPlayerActorKey, PC);

								if (ASAIController::ShowAIDebug == 1)
								{
									UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Detected!")));
									DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Red, false, 0.5f);
									DrawDebugPoint(World, PC->GetActorLocation(), 10.f, FColor::Red, false, 0.5f);
									DrawDebugLine(World, Monster->GetActorLocation(), PC->GetActorLocation(), FColor::Red, false, 0.5f, 0u, 3.f);
								}
							}
							else
							{
								OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASAIController::TargetPlayerActorKey, nullptr);

								if (ASAIController::ShowAIDebug == 1)
								{
									DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Green, false, 0.5f);
								}
							}
						}
					}
				}
				else
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASAIController::TargetPlayerActorKey, nullptr);
				}

				if (ASAIController::ShowAIDebug == 1)
				{
					DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Green, false, 0.5f);
				}
			}
		}
	}
}