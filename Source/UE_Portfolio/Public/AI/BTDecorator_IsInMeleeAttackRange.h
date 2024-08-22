// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInMeleeAttackRange.generated.h"

/**
 *
 */
UCLASS()
class UE_PORTFOLIO_API UBTDecorator_IsInMeleeAttackRange : public UBTDecorator
{
    GENERATED_BODY()

public:
    UBTDecorator_IsInMeleeAttackRange();

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
    //static const float AttackRange;

};
