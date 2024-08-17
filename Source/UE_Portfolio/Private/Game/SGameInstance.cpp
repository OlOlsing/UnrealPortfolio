#include "Game/SGameInstance.h"

void USGameInstance::Init()
{
	Super::Init();

	if (IsValid(CharacterStatDataTable) == false || CharacterStatDataTable->GetRowMap().Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Not enuough data in CharacterStatDataTable."));
	}
	else
	{
		for (int32 i = 1; i <= CharacterStatDataTable->GetRowMap().Num(); ++i)
		{
			check(nullptr != GetCharacterStatDataTableRow(i));
		}
	}
}

void USGameInstance::Shutdown()
{
    Super::Shutdown();
}

FSStatTableRow* USGameInstance::GetCharacterStatDataTableRow(int32 InLevel)
{
	if (IsValid(CharacterStatDataTable) == true)
	{
		return CharacterStatDataTable->FindRow<FSStatTableRow>(*FString::FromInt(InLevel), TEXT("")); // 이름을 Level별로 쓰고 있으니 변환위해서 FromInt
	}

	return nullptr;
}