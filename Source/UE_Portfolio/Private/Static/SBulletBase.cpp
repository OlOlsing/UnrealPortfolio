// Fill out your copyright notice in the Description page of Project Settings.


#include "Static/SBulletBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASBulletBase::ASBulletBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));*/
	//ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	//ProjectileMovementComponent->InitialSpeed = InitSpeed;
	//ProjectileMovementComponent->MaxSpeed = MaxSpeed;
	///** 속도에 따른 로테이션 변화 X */
	//ProjectileMovementComponent->bRotationFollowsVelocity = false;
	///** 바운스 X */
	//ProjectileMovementComponent->bShouldBounce = false;

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

	FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * InitSpeed * DeltaTime);
	SetActorLocation(NewLocation);

}

void ASBulletBase::Fire(const FVector& Driection) const
{
	//ProjectileMovementComponent->Velocity = Driection * ProjectileMovementComponent->InitialSpeed;
}

void ASBulletBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
}

void ASBulletBase::NotifyActorEndOverlap(AActor* OtherActor)
{

}

