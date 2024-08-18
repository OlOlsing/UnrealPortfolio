// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/SPlayerCharacterMurdock.h"

ASPlayerCharacterMurdock::ASPlayerCharacterMurdock()
{
	
}

void ASPlayerCharacterMurdock::BeginPlay()
{
	Super::BeginPlay();
}

void ASPlayerCharacterMurdock::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ASPlayerCharacterMurdock::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASPlayerCharacterMurdock::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
