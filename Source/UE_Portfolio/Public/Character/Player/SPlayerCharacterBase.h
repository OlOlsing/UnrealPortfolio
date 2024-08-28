// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SCharacter.h"
#include "InputActionValue.h"
#include "Game/SPlayerState.h"
#include "SPlayerCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInputConfig;
class UInputMappingContext;
class ASWeaponActor;
class UTextRenderComponent;
struct FStreamableHandle;
class UCameraShakeBase;

UENUM(BlueprintType)
enum class EViewModePlayer : uint8
{
	None,
	BackView,
	QuarterView,
	TPSView,
	End
};

UCLASS()
class UE_PORTFOLIO_API ASPlayerCharacterBase : public ASCharacter
{
	GENERATED_BODY()

public:
	ASPlayerCharacterBase();

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	
	virtual void Tick(float DeltaSeconds) override;

	float GetForwardInputValue() const { return ForwardInputValue; }

	float GetRightInputValue() const { return RightInputValue; }

	float GetCurrentAimPitch() const { return CurrentAimPitch; }

	float GetCurrentAimYaw() const { return CurrentAimYaw; }

	bool GetArmed() const { return bIsArmed; }

	void SetViewMode();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void InputMove(const FInputActionValue& InValue);

	void InputLook(const FInputActionValue& InValue);

	//void InputChangeView(const FInputActionValue& InValue);

	void InputQuickSlot01(const FInputActionValue& InValue);

	void InputQuickSlot02(const FInputActionValue& InValue);

	void InputAttack(const FInputActionValue& InValue);

	void InputMenu(const FInputActionValue& InValue);

	void TryFire();

	void StartIronSight(const FInputActionValue& InValue);

	void EndIronSight(const FInputActionValue& InValue);

	void ToggleTrigger(const FInputActionValue& InValue);

	void StartFire(const FInputActionValue& InValue);

	void StopFire(const FInputActionValue& InValue);

	void SpawnLandMine(const FInputActionValue& InValue);

	UFUNCTION(Server, Reliable, WithValidation) //Server에서 실행, 무조건 수행되게(디폴트는 조금은 씹혀도 상관없다는 Unreliable), 위변조 검증용 함수를 정의해서 사용
	void SpawnLandMine_Server();

	UFUNCTION()
	void OnChangeArmStateEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnAttackEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnCheckPlayerDeath();

	UFUNCTION(Server, Unreliable) // 한 두번 정도는 씹혀도 되기 때문.
	void UpdateInputValue_Server(const float& InForwardInputValue, const float& InRightInputValue);

	UFUNCTION(Server, Unreliable)
	void UpdateAimValue_Server(const float& InAimPitchValue, const float& InAimYawValue);

	UFUNCTION(Server, Unreliable)
	void PlayAttackMontage_Server();

	UFUNCTION(NetMulticast, Unreliable)
	void PlayAttackMontage_NetMulticast();

	UFUNCTION(Server, Reliable) // 정밀하게
	void ApplyDamageAndDrawLine_Server(FHitResult HitResult);

	UFUNCTION(NetMulticast, Reliable)
	void DrawLine_NetMulticast(const FVector& InDrawStart, const FVector& InDrawEnd);

private:

public:
	//FOnMontageEnded OnChangeArmStateMontageEndedDelegate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USInputConfig> PlayerCharacterInputConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UInputMappingContext> PlayerCharacterInputMappingContext;

	EViewModePlayer CurrentViewMode = EViewModePlayer::None;

	FVector DirectionToMove = FVector::ZeroVector;

	float DestArmLength = 0.f;

	float ArmLengthChangeSpeed = 3.f;

	FRotator DestArmRotation = FRotator::ZeroRotator;

	float ArmRotationChangeSpeed = 10.f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float ForwardInputValue;

	float PreviousForwardInputValue = 0.f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float RightInputValue;

	float PreviousRightInputValue = 0.f;

	FSoftObjectPath CurrentPlayerCharacterMeshMaterialPath = FSoftObjectPath();

	TSharedPtr<FStreamableHandle> AssetStreamableHandle = nullptr;

	float TargetFOV = 70.f;

	float CurrentFOV = 70.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float FirePerMinute = 600; //분당 몇 발 발사

	bool bIsTriggerToggle = false;

	FTimerHandle BetweenShotsTimer; // 타이머

	float TimeBetweenFire;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float CurrentAimPitch = 0.f;

	float PreviousAimPitch = 0.f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float CurrentAimYaw = 0.f;

	float PreviousAimYaw = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ASPlayerCharacterBase, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> FireShake;

	FTimerHandle HittedRagdollRestoreTimer;

	FTimerDelegate HittedRagdollRestoreTimerDelegate;

	float TargetRagDollBlendWeight = 0.f;

	float CurrentRagDollBlendWeight = 0.f;

	bool bIsNowRagdollBlending = false;

	bool bIsArmed = true;

	bool bIsRight = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess, Units = cm)) // Meta에 단위를 넣어서 에디터 상에서 cm로 표시
	float MaxRange = 25000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPlayerCharacterBase|AnimLayer", meta = (AllowPrivateAccess))
	TSubclassOf<UAnimInstance> UnarmedCharacterAnimLayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPlayerCharacterBase|AnimLayer", meta = (AllowPrivateAccess))
	TSubclassOf<UAnimInstance> ArmedCharacterAnimLayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> ReloadAnimMontage; // 무장 -> 비무장

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> UnArmAnimMontage; // 무장 -> 비무장

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> ArmAnimMontage; // 비무장 -> 무장 

	//Shot
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> FireLeftAnimMontage; // Left는 아마 Twinblast만 사용할 수도

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> FireRightAnimMontage; // Shot(양쪽다 있긴한데 일단 Rt기준)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UParticleSystemComponent> ParticleSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ASPlayerCharacter, Meta = (AllowPrivateAccess = true))
	TSubclassOf<AActor> LandMineClass;

	TArray<TPair<float, UTextRenderComponent*>> DamageArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess)) // Meta에 단위를 넣어서 에디터 상에서 cm로 표시
	float AlphaTime = 1.f;
private:

};
