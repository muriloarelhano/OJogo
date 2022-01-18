// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseProjectileActor.h"

// Sets default values
ABaseProjectileActor::ABaseProjectileActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollisionComponent->InitSphereRadius(20.f);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->UpdatedComponent = SphereCollisionComponent;
	ProjectileMovementComponent->InitialSpeed = 3000.f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;

	InitialLifeSpan = 3.f;
}

// Called when the game starts or when spawned
void ABaseProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectileActor::OnHit);
}

// Called every frame
void ABaseProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseProjectileActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
}
