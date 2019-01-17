// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"

float ATank::GetHealthPercent() const
{
	return (float)CurrentHealth/(float)StartingHealth;
}

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	//Needed for blueprint begin play to run
	Super::BeginPlay();

	auto AimingComponent = this->FindComponentByClass<UTankAimingComponent>();
	
	CurrentHealth = StartingHealth;

}

UTankMovementComponent* ATank::GetTankMovementComponent()
{
	auto MovementComponent = this->FindComponentByClass<UTankMovementComponent>();
	return MovementComponent;
}


float ATank::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);
	CurrentHealth -= DamageToApply;

	if (CurrentHealth <= 0) 
	{
		OnDeath.Broadcast();
	}

	return DamageToApply;
}
