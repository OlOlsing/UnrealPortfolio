// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameMode.h"
#include "Controller/SPlayerController.h"
#include "Game/SPlayerState.h"
#include "Game/SGameState.h"
#include "Kismet/GameplayStatics.h"

ASGameMode::ASGameMode()
{
	PlayerControllerClass = ASPlayerController::StaticClass();
}

void ASGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &ThisClass::OnMainTimerElapsed, 1.f, true);

	RemainWaitingTimeForPlaying = WaitingTime;
    RemainWaitingTimeForEnding = EndingTime;
}

void ASGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

    ASGameState* SGameState = GetGameState<ASGameState>();
    if (false == SGameState)
    {
        return;
    }

    if (SGameState->MatchState != EMatchState::Waiting)
    {
        NewPlayer->SetLifeSpan(0.1f);
        return;
    }

	ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();

	if (true == ::IsValid(PlayerState))
	{
		PlayerState->InitPlayerState();
	}

	ASPlayerController* NewPlayerController = Cast<ASPlayerController>(NewPlayer);
	if (true == ::IsValid(NewPlayerController))
	{
		AlivePlayerControllers.Add(NewPlayerController);

		NewPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
	}
}

void ASGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ASPlayerController* ExitingPlayerController = Cast<ASPlayerController>(Exiting);
	if (true == ::IsValid(ExitingPlayerController) && INDEX_NONE != AlivePlayerControllers.Find(ExitingPlayerController))
	{
		AlivePlayerControllers.Remove(ExitingPlayerController);
		DeadPlayerControllers.Add(ExitingPlayerController);
	}
}

void ASGameMode::OnMainTimerElapsed()
{
    FString NotificationString = FString::Printf(TEXT(""));
    ASGameState* SGameState = GetGameState<ASGameState>();
    if (false == ::IsValid(SGameState))
    {
        return;
    }

    switch (SGameState->MatchState)
    {
    case EMatchState::None:
        break;
    case EMatchState::Waiting:
    { 
        if (AlivePlayerControllers.Num() < MinimumPlayerCountForPlaying)
        {
            NotificationString = FString::Printf(TEXT("Wait another players for playing."));

            RemainWaitingTimeForPlaying = WaitingTime; // 최소인원이 안된다면 대기 시간 초기화.
        }
        else
        {
            NotificationString = FString::Printf(TEXT("Wait %d seconds for playing."), RemainWaitingTimeForPlaying);

            --RemainWaitingTimeForPlaying;
        }

        if (0 == RemainWaitingTimeForPlaying)
        {
            NotificationString = FString::Printf(TEXT(""));
            SGameState->MatchState = EMatchState::Playing;
        }

        NotifyToAllPlayer(NotificationString);

        break;
    }
    case EMatchState::Playing:

        if (true == IsValid(SGameState))
        {
            SGameState->AlivePlayerControllerCount = AlivePlayerControllers.Num();

            NotificationString = FString::Printf(TEXT("%d / %d"), SGameState->AlivePlayerControllerCount, SGameState->AlivePlayerControllerCount + DeadPlayerControllers.Num());

            NotifyToAllPlayer(NotificationString);

            if (SGameState->AlivePlayerControllerCount <= 1)
            {
                SGameState->MatchState = EMatchState::Ending;
                AlivePlayerControllers[0]->ShowWinnerUI();
            }
        }
        break;
    case EMatchState::Ending:
        NotificationString = FString::Printf(TEXT("Waiting %d for returning to lobby."), RemainWaitingTimeForEnding);

        NotifyToAllPlayer(NotificationString);

        --RemainWaitingTimeForEnding;

        if (0 >= RemainWaitingTimeForEnding)
        {
            for (auto AliveController : AlivePlayerControllers)
            {
                AliveController->ReturnToLobby();
            }
            for (auto DeadController : DeadPlayerControllers)
            {
                DeadController->ReturnToLobby();
            }

            MainTimerHandle.Invalidate();

            FName CurrentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
            UGameplayStatics::OpenLevel(this, CurrentLevelName, true, FString(TEXT("listen")));
            // 만약 데디 서버가 게임 세션 서비스들과 연동되어 있다면,
            // 이렇게 레벨을 다시 준비된 뒤 세션 서버한테 알려줌. "새로운 플레이어 들어올 수 있음."
            // 그럼 세션 서비스는 새로운 플레이어들에게 데디 서버의 IP 주소를 전달해줘서 접속 유도.

            return;
            return;


        }

        break;
        break;
    case EMatchState::End:
        break;
    default:
        break;
    }
}

void ASGameMode::NotifyToAllPlayer(const FString& NotificationString)
{
    for (auto AlivePlayerController : AlivePlayerControllers)
    {
        AlivePlayerController->NotificationText = FText::FromString(NotificationString);
    }

    for (auto DeadPlayerController : DeadPlayerControllers)
    {
        DeadPlayerController->NotificationText = FText::FromString(NotificationString);
    }
}

void ASGameMode::OnControllerDead(ASPlayerController* InDeadController)
{
    if (false == IsValid(InDeadController) || INDEX_NONE == AlivePlayerControllers.Find(InDeadController))
    {
        return;
    }

    InDeadController->ShowLooserUI(AlivePlayerControllers.Num());

    AlivePlayerControllers.Remove(InDeadController);
    DeadPlayerControllers.Add(InDeadController);
}