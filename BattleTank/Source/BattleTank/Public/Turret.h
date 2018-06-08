// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Turret.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTurret : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	//-1 max down speed, +1 max up speed
	void Rotate(float RelativeSpeed);
private:
	UPROPERTY(EditDefaultsOnly, category = "Setup")
		float MaxDegreesPerSecond = 25; //Sensible default.

};
