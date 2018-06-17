// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h" //so we can implement on death
#include "Mortar.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::SetPawn(APawn*InPawn) 
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PosessedTank = Cast<ATank>(InPawn);
		if (!PosessedTank) { return; }
		//Subscribe our local method to the tanks death event
		PosessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPosessedTankDeath);
	}
}
void ATankAIController::OnPosessedTankDeath() 
{
	if (!ensure(GetPawn())) { return; }
	GetPawn()->DetachFromControllerPendingDestroy();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!(AimingComponent && PlayerTank )) { return; }
	else
	{
		
		//move towards player
		MoveToActor(PlayerTank, AcceptanceRadius);

		//Aim at Player
		AimingComponent->AimAt(PlayerTank->GetActorLocation());

		//if aiming or locked - fire
		if (AimingComponent->GetFiringState() == EFiringState::Locked)
		{
			AimingComponent->Fire();
		}
		
	}
}

