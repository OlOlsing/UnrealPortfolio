// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SCharacter.h"
#include "SMonsterCharacter.generated.h"

class USWidgetComponent;
class ASBulletBase;

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

	virtual void RangedAttack1();

	virtual void RangedAttack2();

	virtual void MeleeAttack1();

	virtual void MeleeAttack2();




	UFUNCTION()
	void OnCheckMonsterDeath();

	UFUNCTION()
	void OnCheckFireBullet();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> MeleeAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> RangedAttackMontage;
	// 당연한 이야기지만, BP_NPC > Details > MeleeAttackMontange에 AM_NPC_Fire 지정 필수.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USWidgetComponent> WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float MeleeAttackStartRange = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float DetectRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<ASBulletBase> BulletClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<ASBulletBase> BulletInstance;
};