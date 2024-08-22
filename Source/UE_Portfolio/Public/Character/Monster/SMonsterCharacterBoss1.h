// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Monster/SMonsterCharacter.h"
#include "SMonsterCharacterBoss1.generated.h"

/**
 * 
 */
UCLASS()
class UE_PORTFOLIO_API ASMonsterCharacterBoss1 : public ASMonsterCharacter
{
	GENERATED_BODY()
	
public:
	ASMonsterCharacterBoss1();
protected:
	virtual void BeginAttack() override;

	virtual void EndAttack(UAnimMontage* InMontage, bool bInterruped) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetWidget(UStudyWidget* InStudyWidget) override;
};
