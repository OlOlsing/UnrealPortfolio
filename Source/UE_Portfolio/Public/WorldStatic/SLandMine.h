
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SLandMine.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;

UCLASS()
class UE_PORTFOLIO_API ASLandMine : public AActor
{
	GENERATED_BODY()

public:
	ASLandMine();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnLandMineBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(NetMulticast, Unreliable)
	void SpawnEffect_NetMulticast();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_IsExploded();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASLandMine", Meta = (AllowPrivateAccess))
	TObjectPtr<UBoxComponent> BodyBoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASLandMine", Meta = (AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> BodyStaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASLandMine", meta = (AllowPrivateAccess))
	TObjectPtr<UParticleSystemComponent> ParticleSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASLandMine", meta = (AllowPrivateAccess))
	TObjectPtr<class UMaterial> ExplodedMaterial;

	UPROPERTY(ReplicatedUsing = OnRep_IsExploded, VisibleAnywhere, BlueprintReadOnly, Category = "ASLandMine", meta = (AllowPrivateAccess))
	uint8 bIsExploded : 1;
};
