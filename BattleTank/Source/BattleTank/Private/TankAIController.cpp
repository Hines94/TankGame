// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto ControlledAI = Cast<ATank>(GetPawn());
	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());

	//TODO move towards player
	if (PlayerTank) {
		//Aim at Player
		ControlledAI->AimAt(PlayerTank->GetActorLocation());

		//Fire at player
		ControlledAI->Fire();
	}
}

