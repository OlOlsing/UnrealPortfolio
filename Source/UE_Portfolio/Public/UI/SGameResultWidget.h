// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGameResultWidget.generated.h"

class UTextBlock;
class UButton;
/**
 *
 */
UCLASS()
class UE_PORTFOLIO_API USGameResultWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void OnReturnToLobbyButtonClicked();

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USGameResultWidget", Meta = (BindWidget))
    TObjectPtr<UTextBlock> RankingText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USGameResultWidget", Meta = (BindWidget))
    TObjectPtr<UButton> ReturnToLobbyButton;

};
