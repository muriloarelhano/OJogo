// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseWaepon.h"
#include "Components/ActorComponent.h"
#include "PlayerInventoryActorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OJOGO_API UPlayerInventoryActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerInventoryActorComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	// Help Functions
	UFUNCTION(BlueprintCallable, Category="Inventory|Utilities")
	bool IsPlayerArmed();

	// Action Function
	UFUNCTION(BlueprintCallable, Category="Inventory|Actions")
	void SetCurrentWeapon(ABaseWaepon* Weapon);

	// Action Function
	UFUNCTION(BlueprintCallable, Category="Inventory|Actions")
	void DropCurrentWeapon();

	UFUNCTION(BlueprintCallable, Category="Inventory|Actions")
	void SwitchCurrentWeapon();
                                
                                protected:
                                	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory")
                                	ABaseWaepon* CurrentWeapon = nullptr;
                                	
                                	// Called when the game starts
                                	virtual void BeginPlay() override;
};
