#pragma once

#include "CoreMinimal.h"
#include "UI/StudyWidget.h"
#include "SW_Bar.generated.h"

class UProgressBar;

/**
 *
 */
UCLASS()
class UE_PORTFOLIO_API USW_Bar : public UStudyWidget
{
	GENERATED_BODY()

public:
	USW_Bar(const FObjectInitializer& ObjectInitializer);
	// 만약 Widget 클래스의 생성자에 로직을 작성한다면 꼭 이 생성자를 선언/정의해줘야함.

	void SetMaxFigure(float InMaxFigure);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USW_Bar")
	TObjectPtr<UProgressBar> Bar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USW_Bar")
	float MaxFigure;

};