// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay(); 

	auto ControlledTank = GetControlledAI();
	if (!ControlledTank) {
		UE_LOG(LogTemp,Warning,TEXT("No AI controller"))
	}
	else {
		UE_LOG(LogTemp,Warning,TEXT("AI Tank is: %s"), *(ControlledTank->GetName()))
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetControlledAI()->AimAt(GetPlayerTank()->GetTargetLocation());

	//TODO move towards player
}

ATank * ATankAIController::GetControlledAI() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) { return nullptr; } // Note the !, very important
	return Cast<ATank>(PlayerPawn);
}


