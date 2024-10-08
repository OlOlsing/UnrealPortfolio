#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerTeam : uint8
{
	None,
	Black,
	White,
	End
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentKillCountChangedDelegate, int32, InOldCurrentKillCount, int32, InNewCurrentKillCount);

UCLASS()
class UE_PORTFOLIO_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASPlayerState();

	void InitPlayerState();

	int32 GetMaxKillCount() const { return MaxKillCount; }

	void SetMaxKillCount(int32 InMaxKillCount) { MaxKillCount = InMaxKillCount; }

	int32 GetCurrentKillCount() const { return CurrentKillCount; }

	void AddCurrentKillCount(int32 InCurrentKillCount);

	EPlayerTeam GetPlayerTeam() const { return PlayerTeam; }

public:
	FOnCurrentKillCountChangedDelegate OnCurrentKillCountChangedDelegate;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 CurrentKillCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 MaxKillCount = 99;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	EPlayerTeam PlayerTeam = EPlayerTeam::None;

};