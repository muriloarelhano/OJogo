// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerInventoryActorComponent.h"

// Sets default values for this component's properties
UPlayerInventoryActorComponent::UPlayerInventoryActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPlayerInventoryActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UPlayerInventoryActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UPlayerInventoryActorComponent::SwitchCurrentWeapon()
{
}

ABaseWaepon* UPlayerInventoryActorComponent::GetCurrentWeapon()
{
	return CurrentWeapon;
}

void UPlayerInventoryActorComponent::SetCurrentWeapon(ABaseWaepon* Weapon)
{
	CurrentWeapon = Weapon;
}

void UPlayerInventoryActorComponent::DropCurrentWeapon()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->Drop();
		CurrentWeapon = nullptr;
	}
}


bool UPlayerInventoryActorComponent::IsPlayerArmed()
{
	if (CurrentWeapon)
	{
		return true;
	}
	return false;
}
