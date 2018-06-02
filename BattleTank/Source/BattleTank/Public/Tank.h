// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UTankMovementComponent;
class UTankAimingComponent;
class UTankBarrel; //Forward Declaration
class UTurret;
class AProjectile;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UTankAimingComponent* TankAimingComponent = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UTankMovementComponent* TankMovementComponent = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Sets default values for this pawn's properties
	ATank();
	void AimAt(FVector HitLocation);
	UFUNCTION(BlueprintCallable,Category = Setup)
	void SetTurretReference(UTurret*TurretToSet);
	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetBarrelReference(UTankBarrel*BarrelToSet);
	UFUNCTION(BlueprintCallable, Category = Firing)
	void Fire();
	
private:

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float LaunchSpeed = 70000; // Starting value of 1000 m/s
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AProjectile> ProjectileBlueprint;
	//Local Barrel ref for spawning projectile
	UTankBarrel*Barrel = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float ReloadTimeSeconds = 3;
	double LastFireTime = 0;
};
