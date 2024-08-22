// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/SMonsterCharacter.h"
#include "Controller/SAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/SAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/SPlayerCharacter.h"
#include "Component/SStatComponent.h"
#include "UI/SW_HPBar.h"
#include "Component/SWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Static/SBulletBase.h"
#include "Game/SPlayerState.h"

ASMonsterCharacter::ASMonsterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;

	AIControllerClass = ASAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	// 레벨에 배치되거나 새롭게 생성되면 SAIController의 빙의가 자동으로 진행됨.
	GetCharacterMovement()->MaxWalkSpeed = 250.f;

	WidgetComponent = CreateDefaultSubobject<USWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	// WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	// Billboard 방식으로 보이나, 주인공 캐릭터를 가리게됨. 또한 UI와 멀어져도 동일한 크기가 유지됨.
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
	if (true == ::IsValid(AnimInstance))
	{
		AnimInstance->OnCheckMonsterDeath.AddDynamic(this, &ThisClass::OnCheckMonsterDeath);
	}

}

void ASMonsterCharacter::BeginRangedAttack()
{
	USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
	checkf(IsValid(AnimInstance) == true, TEXT("Invalid AnimInstance"));

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	bIsNowAttacking = true;
	AnimInstance->PlayAnimMontage(RangedAttackMontage);

	if (OnRangedAttackMontageEndedDelegate.IsBound() == false)
	{
		OnRangedAttackMontageEndedDelegate.BindUObject(this, &ThisClass::EndAttack);
		AnimInstance->Montage_SetEndDelegate(OnRangedAttackMontageEndedDelegate, RangedAttackMontage);
	}
	UWorld* World = GetWorld();
	if (true == IsValid(World))
	{
		
		//FVector Location;
		//if (GetMesh()->DoesSocketExist(TEXT("CATRigHub001Bone005")))
		//{
		//	Location = GetMesh()->GetSocketLocation(TEXT("CATRigHub001Bone005")); //총구 이름, 에셋 바뀌면 이 이름도 바뀔 듯, Blast는 쌍권총.. TryFire 재정의 필요
		//}

		//else
		//{
		//	Location = GetMesh()->GetSocketLocation(TEXT("HEAD")); //총구 이름, 에셋 바뀌면 이 이름도 바뀔 듯, Blast는 쌍권총.. TryFire 재정의 필요
		//}
		
		
		//FVector Location = GetMesh()->GetSocketLocation(TEXT("CATRigHub001Bone005")); //총구 이름, 에셋 바뀌면 이 이름도 바뀔 듯, Blast는 쌍권총.. TryFire 재정의 필요
		FRotator Rotation = GetControlRotation();
		BulletInstance = World->SpawnActor<ASBulletBase>(BulletClass, FVector::ZeroVector, FRotator::ZeroRotator);

		if (BulletInstance)
		{
			Rotation = GetControlRotation();
		}
	}
}

void ASMonsterCharacter::BeginAttack()
{
	USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
	checkf(IsValid(AnimInstance) == true, TEXT("Invalid AnimInstance"));

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	bIsNowAttacking = true;
	AnimInstance->PlayAnimMontage(MeleeAttackMontage);

	if (OnMeleeAttackMontageEndedDelegate.IsBound() == false)
	{
		OnMeleeAttackMontageEndedDelegate.BindUObject(this, &ThisClass::EndAttack);
		AnimInstance->Montage_SetEndDelegate(OnMeleeAttackMontageEndedDelegate, MeleeAttackMontage);
	}
}

void ASMonsterCharacter::EndAttack(UAnimMontage* InMontage, bool bInterruped)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bIsNowAttacking = false;

	if (MeleeAttackMontage == InMontage)
	{
		if (true == OnMeleeAttackMontageEndedDelegate.IsBound())
		{
			OnMeleeAttackMontageEndedDelegate.Unbind();
		}
	}

	else if (RangedAttackMontage == InMontage)
	{
		if (true == OnRangedAttackMontageEndedDelegate.IsBound())
		{
			OnRangedAttackMontageEndedDelegate.Unbind();
		}
	}

}

float ASMonsterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// Super::TakeDamage()에서 나머지는 처리 했기 때문.
	if (GetStatComponent()->GetCurrentHP() < KINDA_SMALL_NUMBER)
	{
		ASAIController* AIController = Cast<ASAIController>(GetController());
		if (true == ::IsValid(AIController))
		{
			AIController->EndAI();
		}

		ASPlayerCharacter* DamageCauserCharacter = Cast<ASPlayerCharacter>(DamageCauser);
		if (true == ::IsValid(DamageCauserCharacter))
		{
			ASPlayerState* SPlayerState = Cast<ASPlayerState>(DamageCauserCharacter->GetPlayerState());
			if (true == IsValid(SPlayerState))
			{
				SPlayerState->AddCurrentKillCount(1);
			}
			//DamageCauserCharacter->AddCurrentKillCount(1);
		}

		//Destroy();
	}

	return FinalDamageAmount;
}

void ASMonsterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsValid(WidgetComponent) == true)
	{
		FVector WidgetComponentLocation = WidgetComponent->GetComponentLocation();
		FVector LocalPlayerCameraLocation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation(); // 내 컴에선 내가 0
		WidgetComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(WidgetComponentLocation, LocalPlayerCameraLocation)); // 위젯에서 카메라 바라보는 방향벡터를 통해 로테이션
	}
}

void ASMonsterCharacter::SetWidget(UStudyWidget* InStudyWidget)
{
	Super::SetWidget(InStudyWidget);

	USW_HPBar* HPBarWidget = Cast<USW_HPBar>(InStudyWidget);
	if (IsValid(HPBarWidget) == true)
	{
		HPBarWidget->SetMaxHP(StatComponent->GetMaxHP());
		HPBarWidget->InitializeHPBarWidget(StatComponent);
		StatComponent->OnCurrentHPChangedDelegate.AddDynamic(HPBarWidget, &USW_HPBar::OnCurrentHPChange);
	}
}

void ASMonsterCharacter::OnCheckMonsterDeath()
{
	Destroy();
}
