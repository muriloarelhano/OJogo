// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseWaepon.h"

#include "Characters/MainPlayer.h"

// Sets default values
ABaseWaepon::ABaseWaepon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->SetSimulatePhysics(true);
	
}

// Called when the game starts or when spawned
void ABaseWaepon::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ABaseWaepon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWaepon::InteractWith(AActor* Actor)
{
	if (AMainPlayer* Player = Cast<AMainPlayer>(Actor))
	{
		SkeletalMesh->SetSimulatePhysics(false);
		SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SkeletalMesh->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		                          FName("RightHandMiddleSocket"));
		
	}
}

void ABaseWaepon::LookAt()
{
	UE_LOG(LogTemp, Warning, TEXT("Uhuuuu"));
}
