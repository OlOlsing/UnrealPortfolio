
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class USHUD;
class USGameResultWidget;
/**
 * 
 */
UCLASS()
class UE_PORTFOLIO_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	ASPlayerController();

public:
	USHUD* GetHUDWidget() const { return HUDWidget; };

	virtual void BeginPlay() override;

	void ToggleInGameMenu();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void OnOwningCharacterDead();

	UFUNCTION(Client, Reliable)
	void ShowWinnerUI();

	UFUNCTION(Client, Reliable)
	void ShowLooserUI(int32 InRanking);

	UFUNCTION(Client, Reliable)
	void ReturnToLobby();

protected:
	virtual void SetupInputComponent() override;

private:
	void LeftRight(float InAxisValue);

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "ASPlayerController", Meta = (AllowPrivateAccess))
	FText NotificationText;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ASPlayerController, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> NotificationTextUIInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ASPlayerController, Meta = (AllowPrivateAccess))
	TSubclassOf<USGameResultWidget> WinnerUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ASPlayerController, Meta = (AllowPrivateAccess))
	TSubclassOf<USGameResultWidget> LooserUIClass;

private:
	UPROPERTY();
	TObjectPtr<USHUD> HUDWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess));
	TSubclassOf<USHUD> HUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ASPlayerController, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> InGameMenuClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASPlayerController, Meta = (AllowPrivateAccess))
	TObjectPtr<UUserWidget> InGameMenuInstance;

	bool bIsInGameMenuOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ASPlayerController, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> CrosshairUIClass;
};
