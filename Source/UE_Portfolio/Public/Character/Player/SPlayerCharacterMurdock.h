// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Player/SPlayerCharacterBase.h"
#include "SPlayerCharacterMurdock.generated.h"

/**
 * 
 */
UCLASS()
class UE_PORTFOLIO_API ASPlayerCharacterMurdock : public ASPlayerCharacterBase
{
	GENERATED_BODY()

public:
	ASPlayerCharacterMurdock();

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
