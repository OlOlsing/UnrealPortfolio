#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectTargetPlayer.generated.h"

/**
 *
 */
UCLASS()
class UE_PORTFOLIO_API UBTService_DetectTargetPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_DetectTargetPlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};