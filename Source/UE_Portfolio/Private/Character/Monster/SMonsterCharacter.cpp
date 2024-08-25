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
	// ������ ��ġ�ǰų� ���Ӱ� �����Ǹ� SAIController�� ���ǰ� �ڵ����� �����.
	GetCharacterMovement()->MaxWalkSpeed = 250.f;

	WidgetComponent = CreateDefaultSubobject<USWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	// WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	// Billboard ������� ���̳�, ���ΰ� ĳ���͸� �����Ե�. ���� UI�� �־����� ������ ũ�Ⱑ ������.
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
		AnimInstance->OnCheckFireBullet.AddDynamic(this, &ThisClass::OnCheckFireBullet);
	}

}

void ASMonsterCharacter::BeginRangedAttack()
{
	return;
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

	// Super::TakeDamage()���� �������� ó�� �߱� ����.
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
	if (true == HasAuthority())
	{
		return;     //������� return;�ε� �������� ������ ��
	}

	Super::Tick(DeltaSeconds);

	if (IsValid(WidgetComponent) == true)
	{
		FVector WidgetComponentLocation = WidgetComponent->GetComponentLocation();
		FVector LocalPlayerCameraLocation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation(); // �� �Ŀ��� ���� 0
		WidgetComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(WidgetComponentLocation, LocalPlayerCameraLocation)); // �������� ī�޶� �ٶ󺸴� ���⺤�͸� ���� �����̼�
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

void ASMonsterCharacter::RangedAttack1()
{
}

void ASMonsterCharacter::RangedAttack2()
{
}

void ASMonsterCharacter::MeleeAttack1()
{
}

void ASMonsterCharacter::MeleeAttack2()
{
}

void ASMonsterCharacter::OnCheckMonsterDeath()
{
	Destroy();
}

void ASMonsterCharacter::OnCheckFireBullet()
{
	RangedAttack1();
}
