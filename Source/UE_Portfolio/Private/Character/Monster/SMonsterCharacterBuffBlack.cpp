// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/SMonsterCharacterBuffBlack.h"

void ASMonsterCharacterBuffBlack::BeginAttack()
{
	Super::BeginAttack();
}

void ASMonsterCharacterBuffBlack::EndAttack(UAnimMontage* InMontage, bool bInterruped)
{
	Super::EndAttack(InMontage, bInterruped);
}

float ASMonsterCharacterBuffBlack::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ASMonsterCharacterBuffBlack::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASMonsterCharacterBuffBlack::SetWidget(UStudyWidget* InStudyWidget)
{
	Super::SetWidget(InStudyWidget);
}
