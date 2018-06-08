// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

//Sets the barrel properties

UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	//-1 max down speed, +1 max up speed
	void Elevate(float RelativeSpeed);
private:
	UPROPERTY(EditDefaultsOnly, category = "Setup")
		float MaxDegreesPerSecond = 10; //Sensible default.
	UPROPERTY(EditDefaultsOnly, category = "Setup")
		float MinElevationDegrees = 0;	//Sensible default.
	UPROPERTY(EditDefaultsOnly, category = "Setup")
		float MaxElevationDegrees = 40;	//Sensible default.
};

