// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectileActor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractableInterface.h"
#include "Niagara/Classes/NiagaraSystem.h"
#include "BaseWaepon.generated.h"

UCLASS()
class OJOGO_API ABaseWaepon : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseWaepon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Attributes")
	TSubclassOf<ABaseProjectileActor> Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Attributes")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Attributes")
	UNiagaraSystem* ImpactParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Attributes")
	UNiagaraSystem* MuzzleParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Attributes")
	float FireRate = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Attributes")
	float Range = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon Attributes")
	bool IsDropped = true;

	void Drop();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void InteractWith(AActor* Actor) override;

	virtual void LookAt() override;
};
