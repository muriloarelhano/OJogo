// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/UtilitiesBPFunctionLibrary.h"

FVector UUtilitiesBPFunctionLibrary::CalculateRaycastEnd(AController* PlayerController, float Distance,
                                                         FVector& PlayerViewPointLocation,
                                                         FRotator& PlayerViewPointRotation)
{
	PlayerController->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Distance;
}