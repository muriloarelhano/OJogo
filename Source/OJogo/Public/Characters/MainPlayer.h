// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainPlayer.generated.h"

UCLASS()
class OJOGO_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

protected:
	// Components
	//Spring Arm Component to follow the camera behind the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArmComp;

	//Player follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
	UCurveFloat* SprintVelocityCurve;

	UPROPERTY()
	FTimeline CurveTimeline;
	
	FOnTimelineFloat SprintTimelineCallback;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateSprintPlayerVelocity(float Value);

	// Movement Functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void BeginSprint();
	void EndSprint();
	void BeginCrouch();
	void EndCrouch();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
