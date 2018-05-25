// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class UTankBarrel; //Forward Declaration

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTankAimingComponent();
	void SetBarrelReference(UTankBarrel*BarrelToSet);
	void AimAt(FVector HitLocation,float LaunchSpeed);
	//TODO SetTurretReference

private:
	UTankBarrel * Barrel = nullptr;
	void MoveBarrelTowards(FVector AimDirection) const;
};
