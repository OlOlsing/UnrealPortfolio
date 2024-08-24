// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckAttackInput);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckPlayerDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckMonsterDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckHaveToReload);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckFireBullet);


class UAnimMontage;

UENUM(BlueprintType)
enum class ELocomotionState : uint8
{
	None,
	Idle,
	Walk,
	End
};

UENUM(BlueprintType)
enum class EMovementDirection : uint8
{
	None,
	Fwd,
	Bwd,
	Left,
	Right,
	End
};

UCLASS()
class UE_PORTFOLIO_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAnimMontage(UAnimMontage* InAnimMontage);

	ELocomotionState GetLocomotionState() const { return LocomotionState; }

	EMovementDirection GetMovementDirection() const { return MovementDirection; }

protected:
	UFUNCTION()
	void AnimNotify_CheckHit();

	UFUNCTION()
	void AnimNotify_CheckAttackInput();

	UFUNCTION()
	void AnimNotify_CheckPlayerDeath();

	UFUNCTION()
	void AnimNotify_CheckMonsterDeath();

	UFUNCTION()
	void AnimNotify_CheckHaveToReload();

	UFUNCTION()
	void AnimNotify_CheckFireBullet();

public:
	FOnCheckHit OnCheckHit;

	FOnCheckAttackInput OnCheckAttackInput;

	FOnCheckPlayerDeath OnCheckPlayerDeath;

	FOnCheckMonsterDeath OnCheckMonsterDeath;

	FOnCheckHaveToReload OnCheckHaveToReload;

	FOnCheckFireBullet OnCheckFireBullet;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	uint8 bIsCrouching : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	uint8 bIsArmed : 1; // PlayerCharacterBase Àü¿ë

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	uint8 bIsDead : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	FVector Acceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ELocomotionState LocomotionState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EMovementDirection MovementDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = USTPSAnimInstance, meta = (AllowPrivateAccess = true))
	FRotator ControlRotation;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//float TestSpeed; // Test

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//EMovementDirection MovementDirection;

};
