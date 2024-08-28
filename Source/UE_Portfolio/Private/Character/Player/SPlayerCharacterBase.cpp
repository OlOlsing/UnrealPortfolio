// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/SPlayerCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "WorldStatic/SLandMine.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/SInputConfig.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/SWeaponActor.h"
#include "Animation/SAnimInstance.h"
#include "Particles/ParticleSystemComponent.h"
#include "Component/SStatComponent.h"
#include "SPlayerCharacterSettings.h"
#include "Engine/AssetManager.h"
#include "Game/SPlayerState.h"
#include "Engine/StreamableManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/SPlayerController.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

ASPlayerCharacterBase::ASPlayerCharacterBase()
{

	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	//int Temp = GetCharacterMovement()->JumpZVelocity;
	//GetCharacterMovement()->JumpZVelocity = 8000.f;
	//ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	//ParticleSystemComponent->SetupAttachment(GetRootComponent());
	//ParticleSystemComponent->SetAutoActivate(false);

	//const USPlayerCharacterSettings* CDO = GetDefault<USPlayerCharacterSettings>();
	//if (0 < CDO->PlayerCharacterMeshMaterialPaths.Num())
	//{
	//	for (FSoftObjectPath PlayerCharacterMeshPath : CDO->PlayerCharacterMeshMaterialPaths)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Path: %s"), *(PlayerCharacterMeshPath.ToString()));
	//	}
	//}

	//TimeBetweenFire = 60.f / FirePerMinute;
}

void ASPlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (IsValid(PlayerController) == true)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (IsValid(Subsystem) == true)
		{
			Subsystem->AddMappingContext(PlayerCharacterInputMappingContext, 0);
		}
	}

	USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());

	if (true == ::IsValid(AnimInstance))
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnChangeArmStateEnd);
		AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnAttackEnd);
		AnimInstance->OnCheckPlayerDeath.AddDynamic(this, &ThisClass::OnCheckPlayerDeath);
	}

	SetViewMode();
}

void ASPlayerCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}

void ASPlayerCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaSeconds, 35.f);
	CameraComponent->SetFieldOfView(CurrentFOV);

	if (IsValid(GetController()) == true)
	{
		PreviousAimPitch = CurrentAimPitch;
		PreviousAimYaw = CurrentAimYaw;

		FRotator ControlRotation = GetController()->GetControlRotation();
		CurrentAimPitch = ControlRotation.Pitch;
		CurrentAimYaw = ControlRotation.Yaw;
	}

	if (PreviousAimPitch != CurrentAimPitch || PreviousAimYaw != CurrentAimYaw)
	{
		if (false == HasAuthority())
		{
			UpdateAimValue_Server(CurrentAimPitch, CurrentAimYaw);
		}
	}

	if (PreviousForwardInputValue != ForwardInputValue || PreviousRightInputValue != RightInputValue)
	{
		if (false == HasAuthority())
		{
			UpdateInputValue_Server(ForwardInputValue, RightInputValue);
		}
	}

	TArray <TPair<float, UTextRenderComponent*>> Temp;
	for (auto& iter : DamageArray)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector CurrentLocation = iter.Value->GetComponentLocation();
		FRotator NewRotation = FRotationMatrix::MakeFromX(CameraLocation - CurrentLocation).Rotator();
		iter.Value->SetWorldRotation(NewRotation);

		//FVector NewLocation = CurrentLocation + FVector(0.f, 0.f, 200.f * DeltaSeconds);
		//iter.Value->SetWorldLocation(NewLocation);

		iter.Key += DeltaSeconds;

		if (0.f <= iter.Key && 0.5f > iter.Key)
		{
			iter.Value->SetWorldSize(iter.Value->WorldSize - (AlphaTime * DeltaSeconds));
		}
		
		else if (0.5f <= iter.Key)
		{
			float Alpha = FMath::Clamp(1.0f - (iter.Key - 0.5f), 0.0f, 1.0f);
			FLinearColor TextColor = iter.Value->TextRenderColor;
			TextColor.A = Alpha;
			iter.Value->SetTextRenderColor(TextColor.ToFColor(true));

			if (0.f >= Alpha)
			{
				Temp.Add(iter);
			}
		}
	}

	for (auto& iter : Temp)
	{
		iter.Value->DestroyComponent();
		iter.Value = nullptr;
		DamageArray.Remove(iter);
	}
}

void ASPlayerCharacterBase::SetViewMode()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	SpringArmComponent->TargetArmLength = 400.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bInheritPitch = true;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bDoCollisionTest = true;
	SpringArmComponent->SetRelativeLocation(FVector(0.f, 50.f, 50.f));

	GetCharacterMovement()->RotationRate = FRotator(0.f, 1080.f, 0.f);
	GetCharacterMovement()->bOrientRotationToMovement = false; // 여기도 수정임
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void ASPlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (IsValid(EnhancedInputComponent) == true)
	{
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->Move, ETriggerEvent::Triggered, this, &ThisClass::InputMove);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::InputLook);
		//EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->ChangeView, ETriggerEvent::Started, this, &ThisClass::InputChangeView);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->QuickSlot01, ETriggerEvent::Started, this, &ThisClass::InputQuickSlot01);
		//EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->QuickSlot02, ETriggerEvent::Started, this, &ThisClass::InputQuickSlot02);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->Attack, ETriggerEvent::Started, this, &ThisClass::InputAttack);
		//EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->Menu, ETriggerEvent::Started, this, &ThisClass::InputMenu);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->IronSight, ETriggerEvent::Started, this, &ThisClass::StartIronSight);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->IronSight, ETriggerEvent::Completed, this, &ThisClass::EndIronSight);
		//EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->Trigger, ETriggerEvent::Started, this, &ThisClass::ToggleTrigger);
		//EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->Attack, ETriggerEvent::Started, this, &ThisClass::StartFire);
		//EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->Attack, ETriggerEvent::Completed, this, &ThisClass::StopFire);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->LandMine, ETriggerEvent::Started, this, &ThisClass::SpawnLandMine);
	}
}

void ASPlayerCharacterBase::InputMove(const FInputActionValue& InValue)
{
	if (GetCharacterMovement()->GetGroundMovementMode() == MOVE_None || StatComponent->GetCurrentHP() <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	FVector2D MovementVector = InValue.Get<FVector2D>();
	const FRotator ControlRotation = GetController()->GetControlRotation();
	const FRotator ControlRotationYaw(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardVector, MovementVector.X);
	AddMovementInput(RightVector, MovementVector.Y);

	ForwardInputValue = MovementVector.X;
	RightInputValue = MovementVector.Y;
}

void ASPlayerCharacterBase::InputLook(const FInputActionValue& InValue)
{
	if (GetCharacterMovement()->GetGroundMovementMode() == MOVE_None || StatComponent->GetCurrentHP() <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	FVector2D LookVector = InValue.Get<FVector2D>();
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);

}

void ASPlayerCharacterBase::InputQuickSlot01(const FInputActionValue& InValue)
{
	
	USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());

	if (true == bIsArmed)
	{
		if (true == IsValid(AnimInstance) && true == IsValid(UnArmAnimMontage))
		{
			AnimInstance->Montage_Play(UnArmAnimMontage);
		}
	}

	else// if (true == bIsArmed && true == IsValid(ArmedCharacterAnimLayer))
	{
		if (true == IsValid(AnimInstance) && true == IsValid(ArmAnimMontage))
		{
			AnimInstance->Montage_Play(ArmAnimMontage);
		}
	}

	//bIsArmed = !bIsArmed;
}

void ASPlayerCharacterBase::InputQuickSlot02(const FInputActionValue& InValue)
{

}

void ASPlayerCharacterBase::InputAttack(const FInputActionValue& InValue)
{
	int Cnt = StatComponent->GetCurBulletCnt();
	if (0 >= Cnt) // 총알 없으면 return
	{
		return;
	}

	StatComponent->SetCurBulletCnt(Cnt - 1);
	TryFire();
}

void ASPlayerCharacterBase::InputMenu(const FInputActionValue& InValue)
{
}

void ASPlayerCharacterBase::TryFire()
{
	APlayerController* PlayerController = GetController<APlayerController>();
	if (true == IsValid(PlayerController)) //Paragon Asset 기준 Weapon은 각자 들고있음
	{
#pragma region CaculateTargetTransform
		float FocalDistance = 400.f;
		FVector FocalLocation;
		FVector CameraLocation;
		FRotator CameraRotation;

		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation); // 카메라 정보(위치?)를 받아옴

		FVector AimDirectionFromCamera = CameraRotation.Vector().GetSafeNormal();  // 벡터를 노멀라이즈해서 받아줌 아마 카메라가 보고 있는 시선벡터의 노멀라이즈인듯
		FocalLocation = CameraLocation + (AimDirectionFromCamera * FocalDistance); // 초점 = 카메라 위치 + (스프링 암 길이 * 시선노말)?

		FVector WeaponMuzzleLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_01")); //총구 이름, 에셋 바뀌면 이 이름도 바뀔 듯, Blast는 쌍권총.. TryFire 재정의 필요
		FVector FinalFocalLocation = FocalLocation + (((WeaponMuzzleLocation - FocalLocation) | AimDirectionFromCamera) * AimDirectionFromCamera); // 총구 위치에서

		FTransform TargetTransform = FTransform(CameraRotation, FinalFocalLocation);

		if (ShowAttackDebug == 1)
		{
			DrawDebugSphere(GetWorld(), WeaponMuzzleLocation, 2.f, 16, FColor::Red, false, 60.f);

			DrawDebugSphere(GetWorld(), CameraLocation, 2.f, 16, FColor::Yellow, false, 60.f);

			DrawDebugSphere(GetWorld(), FinalFocalLocation, 2.f, 16, FColor::Magenta, false, 60.f);

			// (WeaponLoc - FocalLoc)
			DrawDebugLine(GetWorld(), FocalLocation, WeaponMuzzleLocation, FColor::Yellow, false, 60.f, 0, 2.f);

			// AimDir
			DrawDebugLine(GetWorld(), CameraLocation, FinalFocalLocation, FColor::Blue, false, 60.f, 0, 2.f);

			// Project Direction Line
			DrawDebugLine(GetWorld(), WeaponMuzzleLocation, FinalFocalLocation, FColor::Red, false, 60.f, 0, 2.f);
		}

#pragma endregion

#pragma region PerformLineTracing

		FVector BulletDirection = TargetTransform.GetUnitAxis(EAxis::X);
		FVector StartLocation = TargetTransform.GetLocation();
		FVector EndLocation = StartLocation + BulletDirection * MaxRange;

		FHitResult HitResult;
		FCollisionQueryParams TraceParams(NAME_None, false, this);
		//TraceParams.AddIgnoredActor(WeaponInstance); // 추후 살리기

		bool IsCollided = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_GameTraceChannel2, TraceParams); // 관통안해서 싱글, 관통할거면 멀티로 바꾸자
		if (IsCollided == false)
		{
			HitResult.TraceStart = StartLocation;
			HitResult.TraceEnd = EndLocation;
		}

		if (ShowAttackDebug == 2)
		{
			if (IsCollided == true)
			{
				DrawDebugSphere(GetWorld(), StartLocation, 2.f, 16, FColor::Red, false, 60.f);

				DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 2.f, 16, FColor::Green, false, 60.f);

				DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Blue, false, 60.f, 0, 2.f);
			}
			else
			{
				DrawDebugSphere(GetWorld(), StartLocation, 2.f, 16, FColor::Red, false, 60.f);

				DrawDebugSphere(GetWorld(), EndLocation, 2.f, 16, FColor::Green, false, 60.f);

				DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 60.f, 0, 2.f);
			}
		}

#pragma endregion

		//if (IsCollided == true)
		//{
		//	ASCharacter* HittedCharacter = Cast<ASCharacter>(HitResult.GetActor());
		//	if (IsValid(HittedCharacter) == true)
		//	{
		//		FDamageEvent DamageEvent;
		//		FString BoneNameString = HitResult.BoneName.ToString(); // HitResult로 BoneName도 알 수 있다.
		//		UKismetSystemLibrary::PrintString(this, BoneNameString);
		//		DrawDebugSphere(GetWorld(), HitResult.Location, 3.f, 16, FColor(255, 0, 0, 255), true, 20.f, 0U, 5.f);

		//		float DamageAmount = 0.f;
		//		if (true == BoneNameString.Equals(FString(TEXT("HEAD")), ESearchCase::IgnoreCase))
		//		{
		//			DamageAmount = 30.f;

		//		}
		//		else
		//		{
		//			DamageAmount = 10.f;
		//		}

		//		HittedCharacter->TakeDamage(DamageAmount, DamageEvent, GetController(), this);

		//		AActor* HitActor = HitResult.GetActor();
		//		UGameplayStatics::ApplyPointDamage(
		//			HitActor,
		//			DamageAmount,
		//			HitActor->GetActorForwardVector(),
		//			HitResult,
		//			GetController(),
		//			Cast<AActor>(this),
		//			UDamageType::StaticClass()
		//		);

		//		UTextRenderComponent* DamageText = NewObject<UTextRenderComponent>(HitActor);
		//		DamageText->SetText(FText::FromString(FString::Printf(TEXT("% d"), FMath::RoundToInt(DamageAmount))));
		//		DamageText->SetWorldSize(150.f);
		//		DamageText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
		//		DamageText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);

		//		DamageText->SetWorldLocation(HitResult.Location);
		//		if (30.f > DamageAmount)
		//		{
		//			DamageText->SetTextRenderColor(FColor::White);
		//		}

		//		else
		//		{
		//			DamageText->SetTextRenderColor(FColor::Red);
		//		}
		//		//DamageText->Set
		//		DamageText->RegisterComponent();
		//		DamageArray.Add({ 0.f, DamageText });
		//	}
		//}

		ApplyDamageAndDrawLine_Server(HitResult);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (true == IsValid(AnimInstance))
		{
			if (false == AnimInstance->Montage_IsPlaying(FireLeftAnimMontage))
			{
				AnimInstance->Montage_Play(FireLeftAnimMontage);
			}

			else
			{
				AnimInstance->Montage_Play(FireRightAnimMontage);
			}

			PlayAttackMontage_Server();
			
		}

		//if (true == IsValid(FireShake) && GetOwner() == UGameplayStatics::GetPlayerController(this, 0)) // 내 화면에만 Shake
		//{
		//	PlayerController->ClientStartCameraShake(FireShake);
		//}

	}
}

void ASPlayerCharacterBase::StartIronSight(const FInputActionValue& InValue)
{
	TargetFOV = 45.f;
}

void ASPlayerCharacterBase::EndIronSight(const FInputActionValue& InValue)
{
	TargetFOV = 70.f;
}

void ASPlayerCharacterBase::ToggleTrigger(const FInputActionValue& InValue)
{
}

void ASPlayerCharacterBase::StartFire(const FInputActionValue& InValue)
{
}

void ASPlayerCharacterBase::StopFire(const FInputActionValue& InValue)
{
}

void ASPlayerCharacterBase::SpawnLandMine(const FInputActionValue& InValue)
{
	SpawnLandMine_Server();
}

bool ASPlayerCharacterBase::SpawnLandMine_Server_Validate() // 이 함수는 WithValidation키워드를 붙이지 않았으면 만들지 않아도 되지만
{
	return true;
}

void ASPlayerCharacterBase::SpawnLandMine_Server_Implementation() // RPC문법을 사용한다면 무조건 Implementation이 붙은 함수로 구현부를 만들어야 함
{
	if (true == IsValid(LandMineClass))
	{
		FVector SpawnedLocation = (GetActorLocation() + GetActorForwardVector() * 200.f) - FVector(0.f, 0.f, 90.f);
		ASLandMine* SpawnedLandMine = GetWorld()->SpawnActor<ASLandMine>(LandMineClass, SpawnedLocation, FRotator::ZeroRotator);
		SpawnedLandMine->SetOwner(GetController());
	}
}

void ASPlayerCharacterBase::OnChangeArmStateEnd(UAnimMontage* Montage, bool bInterrupted)
{

	if (true == bIsArmed)
	{
		if (true == Montage->GetName().Equals(TEXT("AM_TravelMode_Start"), ESearchCase::IgnoreCase))
		{
			if (true == IsValid(UnarmedCharacterAnimLayer))
			{
				GetMesh()->LinkAnimClassLayers(UnarmedCharacterAnimLayer);
			}
		}
	}

	else
	{
		if (true == Montage->GetName().Equals(TEXT("AM_TravelMode_End"), ESearchCase::IgnoreCase))
		{
			if (true == IsValid(ArmedCharacterAnimLayer))
			{
				GetMesh()->LinkAnimClassLayers(ArmedCharacterAnimLayer);
			}
		}
	}
	
	bIsArmed = !bIsArmed;
	
}

void ASPlayerCharacterBase::OnAttackEnd(UAnimMontage* Montage, bool bInterrupted)
{

	//if (true == bIsArmed)
	//{
	//	if (true == Montage->GetName().Equals(TEXT("AM_TravelMode_Start"), ESearchCase::IgnoreCase))
	//	{
	//		if (true == IsValid(UnarmedCharacterAnimLayer))
	//		{
	//			GetMesh()->LinkAnimClassLayers(UnarmedCharacterAnimLayer);
	//		}
	//	}
	//}

	//else
	//{
	//	if (true == Montage->GetName().Equals(TEXT("AM_TravelMode_End"), ESearchCase::IgnoreCase))
	//	{
	//		if (true == IsValid(ArmedCharacterAnimLayer))
	//		{
	//			GetMesh()->LinkAnimClassLayers(ArmedCharacterAnimLayer);
	//		}
	//	}
	//}

	//bIsArmed = !bIsArmed;

}

void ASPlayerCharacterBase::OnCheckPlayerDeath()
{

}

void ASPlayerCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ForwardInputValue);
	DOREPLIFETIME(ThisClass, RightInputValue);
	DOREPLIFETIME(ThisClass, CurrentAimPitch);
	DOREPLIFETIME(ThisClass, CurrentAimYaw);
}

void ASPlayerCharacterBase::UpdateInputValue_Server_Implementation(const float& InForwardInputValue,
	const float& InRightInputValue)
{
	ForwardInputValue = InForwardInputValue;
	RightInputValue = InRightInputValue;
}

void ASPlayerCharacterBase::UpdateAimValue_Server_Implementation(const float& InAimPitchValue, const float& InAimYawValue)
{
	CurrentAimPitch = InAimPitchValue;
	CurrentAimYaw = InAimYawValue;
}

void ASPlayerCharacterBase::PlayAttackMontage_Server_Implementation()
{
	PlayAttackMontage_NetMulticast();
}

void ASPlayerCharacterBase::PlayAttackMontage_NetMulticast_Implementation()
{
	if (false == HasAuthority() && GetOwner() != UGameplayStatics::GetPlayerController(this, 0))
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (true == IsValid(AnimInstance))
		{
			if (false == AnimInstance->Montage_IsPlaying(FireLeftAnimMontage))
			{
				AnimInstance->Montage_Play(FireLeftAnimMontage);
			}

			else
			{
				AnimInstance->Montage_Play(FireRightAnimMontage);
			}
		}
	}
}

void ASPlayerCharacterBase::ApplyDamageAndDrawLine_Server_Implementation(FHitResult HitResult)
{
	ASCharacter* HittedCharacter = Cast<ASCharacter>(HitResult.GetActor());
	if (IsValid(HittedCharacter) == true)
	{
		FDamageEvent DamageEvent;

		FString BoneNameString = HitResult.BoneName.ToString();

		float DamageAmount = 0.f;
		if (true == BoneNameString.Equals(FString(TEXT("HEAD")), ESearchCase::IgnoreCase))
		{
			DamageAmount = 30.f;

		}
		else
		{
			DamageAmount = 10.f;
		}

		HittedCharacter->TakeDamage(DamageAmount, DamageEvent, GetController(), this);

		AActor* HitActor = HitResult.GetActor();
		UGameplayStatics::ApplyPointDamage(
			HitActor,
			DamageAmount,
			HitActor->GetActorForwardVector(),
			HitResult,
			GetController(),
			Cast<AActor>(this),
			UDamageType::StaticClass()
		);

		UTextRenderComponent* DamageText = NewObject<UTextRenderComponent>(HitActor);
		DamageText->SetText(FText::FromString(FString::Printf(TEXT("% d"), FMath::RoundToInt(DamageAmount))));
		DamageText->SetWorldSize(150.f);
		DamageText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
		DamageText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);

		DamageText->SetWorldLocation(HitResult.Location);
		if (30.f > DamageAmount)
		{
			DamageText->SetTextRenderColor(FColor::White);
		}

		else
		{
			DamageText->SetTextRenderColor(FColor::Red);
		}

		DamageText->RegisterComponent();
		DamageArray.Add({ 0.f, DamageText });
	}

	DrawLine_NetMulticast(HitResult.TraceStart, HitResult.TraceEnd);
}

void ASPlayerCharacterBase::DrawLine_NetMulticast_Implementation(const FVector& InDrawStart, const FVector& InDrawEnd)
{
	if (false == HasAuthority())
	{
		DrawDebugLine(GetWorld(), GetMesh()->GetSocketLocation(TEXT("Muzzle_01")), InDrawEnd, FColor(255, 255, 255, 64), false, 0.1f, 0U, 0.5f);
	}
}
