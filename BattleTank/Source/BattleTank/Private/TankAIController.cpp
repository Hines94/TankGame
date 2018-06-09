// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!ensure(AimingComponent && PlayerTank )) { return; }
	else
	{
		
		//move towards player
		MoveToActor(PlayerTank, AcceptanceRadius);

		//Aim at Player
		AimingComponent->AimAt(PlayerTank->GetActorLocation());

		//Fire at player
		AimingComponent->Fire();
	}
}

