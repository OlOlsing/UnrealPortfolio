// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

/**
 * 
 */
class ASPlayerController;
class ASGameState;

UCLASS()
class UE_PORTFOLIO_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASGameMode();

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	void OnControllerDead(ASPlayerController* InDeadController);

private:
	UFUNCTION()
	void OnMainTimerElapsed();

	void NotifyToAllPlayer(const FString& NotificationString);

public:
	FTimerHandle MainTimerHandle;

	FTimerHandle MainTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASGameMode")
	int32 WaitingTime = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASGameMode")
	int32 RemainWaitingTimeForPlaying = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASGameMode")
	int32 MinimumPlayerCountForPlaying = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASGameMode")
	int32 EndingTime = 5;

	int32 RemainWaitingTimeForEnding = 5;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASGameMode", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<ASPlayerController>> AlivePlayerControllers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASGameMode", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<ASPlayerController>> DeadPlayerControllers;
	
};
