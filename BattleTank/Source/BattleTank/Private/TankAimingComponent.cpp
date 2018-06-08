// Copyright Jack Hines.

#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.h"
#include "Turret.h"
#include "Projectile.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankAimingComponent::Initialise(UTurret*TurretToSet, UTankBarrel*BarrelToSet)
{
	if (!ensure(TurretToSet && BarrelToSet)) { return; }
	Turret= TurretToSet;
	Barrel = BarrelToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)) { return; }
	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	//Calculate the out launch velocity
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
																			this,
																			OutLaunchVelocity,
																			StartLocation,
																			HitLocation,
																			LaunchSpeed,
																			false,
																			0,
																			0,
																			ESuggestProjVelocityTraceOption::DoNotTrace
																		);
		if(bHaveAimSolution)
		{
			auto AimDirection = OutLaunchVelocity.GetSafeNormal();
			MoveBarrelTowards(AimDirection);
			MoveTurretTowards(AimDirection);
		}

}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) const
{
	if (!ensure(Barrel && Turret)) { return; }
	//Get rotator for the varrel -diff between current rotation and aim direction
	auto BarrrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrrelRotator;
	//move the barrel
	Barrel->Elevate(DeltaRotator.Pitch);
}

void UTankAimingComponent::MoveTurretTowards(FVector AimDirection) const
{
	if (!ensure(Barrel && Turret)) { return; }
	//Get rotator for the varrel -diff between current rotation and aim direction
	auto TurretRotator = Turret->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - TurretRotator;
	//move the barrel
	Turret->Rotate(DeltaRotator.Yaw);
}

void UTankAimingComponent::Fire()
{
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeSeconds;
	if (!ensure(Barrel) || !ProjectileBlueprint) { return; }

	if (isReloaded)
	{
		//Spawn a projectile at the barrel reference
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint,
			Barrel->GetSocketLocation("Projectile"),
			Barrel->GetSocketRotation("Projectile")
			);
		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}