// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthActorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OJOGO_API UHealthActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthActorComponent();

	// Attr
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float DefaultHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, Category="Health")
	float Health;


protected:
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	void Die();


	// Called when the game starts
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
