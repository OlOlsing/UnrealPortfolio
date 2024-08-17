
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class USHUD;
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

protected:
	virtual void SetupInputComponent() override;

private:
	void LeftRight(float InAxisValue);

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
