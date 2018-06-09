// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"
//Forward Declaration
class UTankBarrel; 
class UTurret;
class AProjectile;

UENUM()
enum class EFiringState : uint8
{
	Reloading,
	Aiming,
	Locked
};

//Holds Barrel's Properties and Elevate Method

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTurret*TurretToSet, UTankBarrel*BarrelToSet);

	UTankAimingComponent();
	void AimAt(FVector HitLocation);
	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire();

protected:
	UPROPERTY(BlueprintReadOnly,Category = "State")
	EFiringState FiringState = EFiringState::Reloading;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 70000; // Starting value of 7000 m/s

private:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void BeginPlay() override;
	bool IsBarrelMoving();
	FVector AimDirection;
	UTurret * Turret = nullptr;
	UTankBarrel * Barrel = nullptr;
	void MoveBarrelTowards(FVector AimDirection) const;
	void MoveTurretTowards(FVector AimDirection) const;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeSeconds = 3;
	double LastFireTime = 0;
	
};
