#include "UI/UW_TitleLayout.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

UUW_TitleLayout::UUW_TitleLayout(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_TitleLayout::NativeConstruct()
{
	PlayButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnPlayButtonClicked);
	ExitButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnExitButtonClicked);
}

void UUW_TitleLayout::OnPlayButtonClicked()
{
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Example"));
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Loading")), true, FString(TEXT("NextLevel=Lobby")));
	//UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Lobby")), true, FString(TEXT("NextLevel=Example")));
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"));
	// NextLevel�� Key, Example�� Value��. �׷� Loading ���������� NextLevel�� �Ľ��ؼ� Example ���� ���� ��.
}

void UUW_TitleLayout::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}