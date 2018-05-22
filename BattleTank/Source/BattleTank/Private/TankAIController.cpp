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


ATank * ATankAIController::GetControlledAI() const
{
	return Cast<ATank>(GetPawn());
}


