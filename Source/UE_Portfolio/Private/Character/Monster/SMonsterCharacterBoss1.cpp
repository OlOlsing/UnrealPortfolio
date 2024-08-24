// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/SMonsterCharacterBoss1.h"
#include "Animation/SAnimInstance.h"
#include "Static/SBulletBase.h"
#include "GameFramework/CharacterMovementComponent.h"

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

void ASMonsterCharacterBoss1::BeginRangedAttack()
{
	Super::BeginRangedAttack();

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

}

void ASMonsterCharacterBoss1::RangedAttack1()
{
	UWorld* World = GetWorld();
	if (true == IsValid(World))
	{
		if (GetMesh()->DoesSocketExist(TEXT("CATRigHub001Bone005")))
		{
			FVector Location = GetMesh()->GetSocketLocation(TEXT("CATRigHub001Bone005")); //소켓 따로 추가 or Offset변수로 조정
			FRotator Rotation = GetControlRotation();
			Location.Z += 3000.f; // Offset 조정 필요
			BulletInstance = World->SpawnActor<ASBulletBase>(BulletClass, Location, Rotation);
			//BulletInstance->SetScale
			if (BulletInstance)
			{
				BulletInstance->SetActorLocation(Location);
				BulletInstance->SetActorRotation(GetActorRotation());
				FVector Direction = World->GetFirstPlayerController()->GetPawn()->GetActorLocation() - Location;
				Direction.Normalize();
				BulletInstance->Fire(Direction);
			}
		}
	}
}

void ASMonsterCharacterBoss1::RangedAttack2()
{
}

void ASMonsterCharacterBoss1::MeleeAttack1()
{
}

void ASMonsterCharacterBoss1::MeleeAttack2()
{
}
