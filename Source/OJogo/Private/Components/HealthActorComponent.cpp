// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthActorComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthActorComponent::UHealthActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Health = DefaultHealth;
	// ...
}

// Called when the game starts
void UHealthActorComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthActorComponent::TakeDamage);
	}
}


// Called every frame
void UHealthActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UHealthActorComponent::TakeDamage(AActor* DamagedActor, float Damage,
                                       const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
		return;

	Health -= Damage;
}

void UHealthActorComponent::Die()
{
}
