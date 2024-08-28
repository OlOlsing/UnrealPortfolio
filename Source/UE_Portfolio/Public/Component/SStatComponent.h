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

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
    UFUNCTION(NetMulticast, Reliable)
    void OnCurrentHPChanged_NetMulticast(float InOldCurrentHP, float InNewCurrentHP);

public:
    FOnOutOfCurrentHPDelegate OnOutOfCurrentHPDelegate;

    FOnCurrentHPChangeDelegate OnCurrentHPChangedDelegate;

    FOnMaxHPChangeDelegate OnMaxHPChangedDelegate;

private:
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
    TObjectPtr<class USGameInstance> GameInstance;


    UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
    float MaxHP;

    UPROPERTY(Replicated, Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
    float CurrentHP;


    //UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
    //float MaxHP;

    //UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
    //float CurrentHP;                                                                                                       
           
    // Transient - 이 키워드를 갖고 있는 속성은 툴에서 수정해도 무조건 CDO에 있는 값이 기본값
    // 휘발성이라 메모리 절약, 시리얼라이션 제외 등 이점,
    // CurHp는 객체마다 다 다르니 굳이 저장할 필요가 없으니 키워드 사용

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
    int32 MaxBulletCnt = 1000; // 임시 

    UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
    int32 CurBulletCnt = 1000; // 임시
};