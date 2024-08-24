// Fill out your copyright notice in the Description page of Project Settings.


#include "Static/SBulletBase.h"
#include "Engine/DamageEvents.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Character/Player/SPlayerCharacterBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASBulletBase::ASBulletBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	//BoxComponent->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
//	BoxComponent->SetCollisionProfileName(FName("SBulletBase"));
	//SetRootComponent(StaticMeshComponent);
	SetRootComponent(SphereComponent);

	//StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASBulletBase::OnOverlapBegin);
	SphereComponent->SetCollisionProfileName(FName("SBulletBase"));

	StaticMeshComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 30.f;
	ProjectileMovementComponent->MaxSpeed = 30.f;
	/** 속도에 따른 로테이션 변화 X */
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	/** 바운스 X */
	ProjectileMovementComponent->bShouldBounce = false; 
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

}

// Called when the game starts or when spawned
void ASBulletBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASBulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * InitSpeed * DeltaTime);
	//SetActorLocation(NewLocation);

}

void ASBulletBase::Fire(const FVector& Direction) const
{
	//ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
	//ProjectileMovementComponent->Move
}

void ASBulletBase::NotifyActorBeginOverlap(AActor* OtherActor)
{

}

void ASBulletBase::NotifyActorEndOverlap(AActor* OtherActor)
{

}

void ASBulletBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ASPlayerCharacterBase* Player = Cast<ASPlayerCharacterBase>(OtherActor);
	if (true == IsValid(Player))
	{
		FDamageEvent DamageEvent;
		Player->TakeDamage(50.f, DamageEvent, nullptr, this); // 3번째 인자 nullptr Test
		StaticMeshComponent->SetHiddenInGame(true);
		SetActorEnableCollision(false);
		//원래는 이펙트 처리 이후
		Destroy();
	}


}

