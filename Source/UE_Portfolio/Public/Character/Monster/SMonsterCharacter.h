// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SCharacter.h"
#include "SMonsterCharacter.generated.h"

class USWidgetComponent;

UCLASS()
class UE_PORTFOLIO_API ASMonsterCharacter : public ASCharacter
{
	GENERATED_BODY()

	friend class UBTTask_Attack;
	friend class UBTTask_RangedAttack;

public:
	ASMonsterCharacter();

	float GetMeleeAttackStartRange() const { return MeleeAttackStartRange; }

	float GetDetectRadius() const { return DetectRadius; }

	
protected:
	virtual void BeginPlay() override;

	virtual void BeginAttack() override;

	virtual void EndAttack(UAnimMontage* InMontage, bool bInterruped) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetWidget(UStudyWidget* InStudyWidget) override;

	virtual void BeginRangedAttack();

	UFUNCTION()
	void OnCheckMonsterDeath();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> MeleeAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> RangedAttackMontage;
	// �翬�� �̾߱�����, BP_NPC > Details > MeleeAttackMontange�� AM_NPC_Fire ���� �ʼ�.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USWidgetComponent> WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float MeleeAttackStartRange = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float DetectRadius = 500.f;
};