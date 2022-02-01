// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseWaepon.h"

#include "DrawDebugHelpers.h"
#include "Characters/MainPlayer.h"
#include "Components/HealthActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/UtilitiesBPFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"

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
		this->SetOwner(Player);
		this->IsDropped = false;
		Player->GetInventory()->SetCurrentWeapon(this);
	}
}

void ABaseWaepon::Drop()
{
	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SkeletalMesh->SetSimulatePhysics(true);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ABaseWaepon::Shoot()
{
	AMainPlayer* PlayerOwner = Cast<AMainPlayer>(GetOwner());
	if (!PlayerOwner) { return; }

	FHitResult Hit;
	FVector EndLocation = UUtilitiesBPFunctionLibrary::CalculateRaycastEnd(
		PlayerOwner->GetController(), Range, PlayerOwner->PlayerViewPointLocation,
		PlayerOwner->PlayerViewPointRotation);
	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, this);

	FTransform Weapon = SkeletalMesh->GetSocketTransform(
		FName("Muzzle"));
	DrawDebugLine(GetWorld(), PlayerOwner->PlayerViewPointLocation, EndLocation, FColor::Red, false, 2, 0, 1);

	if (GetWorld())
	{
		bool actorHit = GetWorld()->LineTraceSingleByChannel(Hit, PlayerOwner->PlayerViewPointLocation, EndLocation,
		                                                     ECC_WorldDynamic, QueryParams, FCollisionResponseParams());
		DrawDebugLine(GetWorld(), PlayerOwner->PlayerViewPointLocation, EndLocation, FColor::Red, false, 1.f, 0, 1.f);
		if (actorHit)
		{
			if (ImpactParticle)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(), ImpactParticle,
					Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

			TSet<UActorComponent*> actorComponents = Hit.GetActor()->GetComponents();
			if (!actorComponents.IsEmpty())
			{
				for (auto Component : actorComponents)
				{
					UHealthActorComponent* ActorHealthComponent = Cast<UHealthActorComponent>(Component);
					if (ActorHealthComponent)
					{
						ActorHealthComponent->GetOwner()->TakeDamage(Damage,
						                                             FDamageEvent(),
						                                             GetOwner()->GetInstigatorController(), this);
					}
				}
			}
		}
	}

	if (MuzzleParticle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), MuzzleParticle, Weapon.GetLocation(),
			Weapon.Rotator());
	}

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound,
		                                      Weapon.GetLocation());
	}
}


void ABaseWaepon::LookAt()
{
	UE_LOG(LogTemp, Warning, TEXT("Uhuuuu"));
}
