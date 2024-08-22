// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/SMonsterCharacterMinion2.h"

ASMonsterCharacterMinion2::ASMonsterCharacterMinion2()
{
	MeleeAttackRange = 50.f;
	MeleeAttackRadius = 20.f;
}

void ASMonsterCharacterMinion2::BeginAttack()
{
	Super::BeginAttack();
}

void ASMonsterCharacterMinion2::EndAttack(UAnimMontage* InMontage, bool bInterruped)
{
	Super::EndAttack(InMontage, bInterruped);
}

float ASMonsterCharacterMinion2::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ASMonsterCharacterMinion2::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASMonsterCharacterMinion2::SetWidget(UStudyWidget* InStudyWidget)
{
	Super::SetWidget(InStudyWidget);
}