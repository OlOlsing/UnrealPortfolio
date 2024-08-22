// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Monster/SMonsterCharacter.h"
#include "SMonsterCharacterMinion2.generated.h"

/**
 * 
 */
UCLASS()
class UE_PORTFOLIO_API ASMonsterCharacterMinion2 : public ASMonsterCharacter
{
	GENERATED_BODY()
	
public:
	ASMonsterCharacterMinion2();
protected:
	virtual void BeginAttack() override;

	virtual void EndAttack(UAnimMontage* InMontage, bool bInterruped) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetWidget(UStudyWidget* InStudyWidget) override;
};
