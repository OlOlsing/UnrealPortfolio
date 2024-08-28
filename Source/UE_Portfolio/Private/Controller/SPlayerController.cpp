// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SPlayerController.h"
#include "UI/SHUD.h"
#include "Game/SPlayerState.h"
#include "Component/SStatComponent.h"
#include "Character/SCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Game/SGameMode.h"
#include "UI/SGameResultWidget.h"
#include "Components/TextBlock.h"

ASPlayerController::ASPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    // 컨트롤러 액터의 Tick이 돌아야 입력도 처리할 수 있음.
}

void ASPlayerController::BeginPlay()
{
    Super::BeginPlay();

    FInputModeGameOnly InputModeGameOnly;
    SetInputMode(InputModeGameOnly);

    if (true == HasAuthority())
    {
        return;     //내 UI는 내 화면, 내 클라에서만 생성되어야 하니 서버라면 return
    }

    if (IsValid(HUDWidgetClass) == true)
    {
        HUDWidget = CreateWidget<USHUD>(this, HUDWidgetClass); // 만들고
        if (IsValid(HUDWidget) == true)
        {
            HUDWidget->AddToViewport(); // 이거까지 넣어야 화면에 나옴

            ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
            if (IsValid(SPlayerState) == true)
            {
                HUDWidget->BindPlayerState(SPlayerState);
            }

            ASCharacter* PC = GetPawn<ASCharacter>();
            if (IsValid(PC) == true)
            {
                USStatComponent* StatComponent = PC->GetStatComponent();
                if (IsValid(StatComponent) == true)
                {
                    HUDWidget->BindStatComponent(StatComponent);
                }
            }
        }
    }

    if (IsValid(InGameMenuClass) == true)
    {
        InGameMenuInstance = CreateWidget<UUserWidget>(this, InGameMenuClass);
        if (IsValid(InGameMenuInstance) == true)
        {
            InGameMenuInstance->AddToViewport(3); // 상위에 띄움.

            InGameMenuInstance->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    if (IsValid(CrosshairUIClass) == true)
    {
        UUserWidget* CrosshairUIInstance = CreateWidget<UUserWidget>(this, CrosshairUIClass);
        if (IsValid(CrosshairUIInstance) == true)
        {
            CrosshairUIInstance->AddToViewport(1);

            CrosshairUIInstance->SetVisibility(ESlateVisibility::Visible);
        }
    }

    if (IsValid(NotificationTextUIInstance) == true)
    {
        UUserWidget* NotificationTextUI = CreateWidget<UUserWidget>(this, NotificationTextUIInstance);
        if (IsValid(NotificationTextUI) == true)
        {
            NotificationTextUI->AddToViewport(1);

            NotificationTextUI->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void ASPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAxis(TEXT("LeftRight"), this, &ThisClass::LeftRight);
}

void ASPlayerController::LeftRight(float InAxisValue)
{
   // UE_LOG(LogTemp, Error, TEXT("ASPlayerController::LeftRight(%.3f)"), InAxisValue);
}

void ASPlayerController::ToggleInGameMenu()
{
    checkf(IsValid(InGameMenuInstance) == true, TEXT("Invalid InGameMenuInstance"));

    if (false == bIsInGameMenuOn)
    {
        InGameMenuInstance->SetVisibility(ESlateVisibility::Visible);

        FInputModeUIOnly Mode;
        Mode.SetWidgetToFocus(InGameMenuInstance->GetCachedWidget());
        SetInputMode(Mode);

        // SetPause(true); 만약 게임 일시 정지를 원한다면.
        //					그리고 InputAction 애셋의 TriggerWhenPaused 속성을 true로 지정해야 Pause 상태에서도 해당 입력 액션이 동작함.
        bShowMouseCursor = true;
    }
    else
    {
        InGameMenuInstance->SetVisibility(ESlateVisibility::Collapsed);
        FInputModeGameOnly InputModeGameOnly;
        SetInputMode(InputModeGameOnly);

        // SetPause(false); 만약 게임 일시 정지를 원한다면 이 코드가 필요함.
        //					그리고 InputAction 애셋의 TriggerWhenPaused 속성을 true로 지정해야 Pause 상태에서도 해당 입력 액션이 동작함.
        bShowMouseCursor = false;
    }

    bIsInGameMenuOn = !bIsInGameMenuOn;
}

void ASPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, NotificationText);
}

void ASPlayerController::OnOwningCharacterDead()
{
    ASGameMode* GameMode = Cast<ASGameMode>(UGameplayStatics::GetGameMode(this));
    if (true == HasAuthority() && true == IsValid(GameMode))
    {
        GameMode->OnControllerDead(this);
    }
}

void ASPlayerController::ShowWinnerUI_Implementation()
{
    if (HasAuthority() == false)
    {
        if (IsValid(WinnerUIClass) == true)
        {
            USGameResultWidget* WinnerUI = CreateWidget<USGameResultWidget>(this, WinnerUIClass);
            if (IsValid(WinnerUI) == true)
            {
                WinnerUI->AddToViewport(3);
                WinnerUI->RankingText->SetText(FText::FromString(TEXT("#01")));

                FInputModeUIOnly Mode;
                Mode.SetWidgetToFocus(WinnerUI->GetCachedWidget());
                SetInputMode(Mode);

                bShowMouseCursor = true;
            }
        }
    }
}

void ASPlayerController::ShowLooserUI_Implementation(int32 InRanking)
{
    if (HasAuthority() == false)
    {
        if (IsValid(LooserUIClass) == true)
        {
            USGameResultWidget* LooserUI = CreateWidget<USGameResultWidget>(this, LooserUIClass);
            if (IsValid(LooserUI) == true)
            {
                LooserUI->AddToViewport(3);
                FString RankingString = FString::Printf(TEXT("#%02d"), InRanking);
                LooserUI->RankingText->SetText(FText::FromString(RankingString));

                FInputModeUIOnly Mode;
                Mode.SetWidgetToFocus(LooserUI->GetCachedWidget());
                SetInputMode(Mode);

                bShowMouseCursor = true;
            }
        }
    }
}

void ASPlayerController::ReturnToLobby_Implementation()
{
    if (false == HasAuthority())
    { // 서버의 레벨이 변경되는걸 원치 않음. 클라가 이동해야하므로 if() 처리 필수.
        UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Loading")), true, FString(TEXT("NextLevel=Lobby?Saved=false")));
    }
}