// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/SMonsterCharacterBoss1.h"

ASMonsterCharacterBoss1::ASMonsterCharacterBoss1()
{
	MeleeAttackRange = 50.f;
	MeleeAttackRadius = 20.f;
}

void ASMonsterCharacterBoss1::BeginAttack()
{
	Super::BeginAttack();
}

void ASMonsterCharacterBoss1::EndAttack(UAnimMontage* InMontage, bool bInterruped)
{
	Super::EndAttack(InMontage, bInterruped);
}

float ASMonsterCharacterBoss1::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ASMonsterCharacterBoss1::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASMonsterCharacterBoss1::SetWidget(UStudyWidget* InStudyWidget)
{
	Super::SetWidget(InStudyWidget);
}