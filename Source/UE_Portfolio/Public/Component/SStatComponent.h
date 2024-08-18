#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutOfCurrentHPDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentHPChangeDelegate, float, InOldCurrentHP, float, InNewCurrentHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxHPChangeDelegate, float, InOldMaxHP, float, InNewMaxHP);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE_PORTFOLIO_API USStatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USStatComponent();

    virtual void BeginPlay() override;

    float GetMaxHP() const { return MaxHP; }

    void SetMaxHP(float InMaxHP);

    float GetCurrentHP() const { return CurrentHP; }

    void SetCurrentHP(float InCurrentHP);

    int32 GetMaxBulletCnt() const { return MaxBulletCnt; }

    void SetMaxBulletCnt(int32 InMaxBulletCnt) { MaxBulletCnt = InMaxBulletCnt; }

    int32 GetCurBulletCnt() const { return CurBulletCnt;}

    void SetCurBulletCnt(int32 InCurBulletCnt) { CurBulletCnt = InCurBulletCnt; }

public:
    FOnOutOfCurrentHPDelegate OnOutOfCurrentHPDelegate;

    FOnCurrentHPChangeDelegate OnCurrentHPChangedDelegate;

    FOnMaxHPChangeDelegate OnMaxHPChangedDelegate;

private:
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
    TObjectPtr<class USGameInstance> GameInstance;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
    float MaxHP;

    UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess)) // Transient - �� Ű���带 ���� �ִ� �Ӽ��� ������ �����ص� ������ CDO�� �ִ� ���� �⺻��
    float CurrentHP;                                                                                                        // �ֹ߼��̶� �޸� ����, �ø�����̼� ���� �� ����,
                                                                                                                            // CurHp�� ��ü���� �� �ٸ��� ���� ������ �ʿ䰡 ������ Ű���� ���

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
    int32 MaxBulletCnt = 1000; // �ӽ� 

    UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
    int32 CurBulletCnt = 1000; // �ӽ�
};