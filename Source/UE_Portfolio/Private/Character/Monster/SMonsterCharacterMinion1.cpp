// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/SMonsterCharacterMinion1.h"

ASMonsterCharacterMinion1::ASMonsterCharacterMinion1()
{
	MeleeAttackRange = 50.f;
	MeleeAttackRadius = 20.f;
}

void ASMonsterCharacterMinion1::BeginAttack()
{
	Super::BeginAttack();
}

void ASMonsterCharacterMinion1::EndAttack(UAnimMontage* InMontage, bool bInterruped)
{
	Super::EndAttack(InMontage, bInterruped);
}

float ASMonsterCharacterMinion1::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ASMonsterCharacterMinion1::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASMonsterCharacterMinion1::SetWidget(UStudyWidget* InStudyWidget)
{
	Super::SetWidget(InStudyWidget);
}
