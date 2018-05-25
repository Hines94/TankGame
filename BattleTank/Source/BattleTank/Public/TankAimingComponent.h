// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"
//Forward Declaration
class UTankBarrel; 
class UTurret;

//Holds Barrel's Properties and Elevate Method

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTankAimingComponent();
	void SetTurretReference(UTurret*TurretToSet);
	void SetBarrelReference(UTankBarrel*BarrelToSet);
	void AimAt(FVector HitLocation,float LaunchSpeed);

private:
	UTurret * Turret = nullptr;
	UTankBarrel * Barrel = nullptr;
	void MoveBarrelTowards(FVector AimDirection) const;
	void MoveTurretTowards(FVector AimDirection) const;
};
