// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_LobbyLayout.generated.h"


class UButton;
class UEditableText;
struct FStreamableHandle;
class UMaterialInstance;


UCLASS()
class UE_PORTFOLIO_API UUW_LobbyLayout : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void OnBlackTeamButtonClicked();

	UFUNCTION()
	void OnWhiteTeamButtonClicked();

	UFUNCTION()
	void OnSubmitButtonClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USLobbyLevelUI, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> BlackTeamButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USLobbyLevelUI, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> WhiteTeamButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USLobbyLevelUI, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UEditableText> EditPlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USLobbyLevelUI, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> SubmitButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USLobbyLevelUI, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UEditableText> EditServerIP;

	TWeakObjectPtr<USkeletalMeshComponent> CurrentSkeletalMeshComponent;

	TArray<TSharedPtr<FStreamableHandle>> StreamableHandles;

	TArray<TSoftObjectPtr<UMaterial>> LoadedMaterialInstanceAssets;

	uint8 SelectedTeam = 1;



};
