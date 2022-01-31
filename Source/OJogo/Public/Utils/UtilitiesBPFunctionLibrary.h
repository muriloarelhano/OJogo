// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilitiesBPFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API UUtilitiesBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Raycast Functions
	static FVector CalculateRaycastEnd(AController* PlayerController, float Distance,
	                                   FVector& PlayerViewPointLocation,
	                                   FRotator& PlayerViewPointRotation);
};
