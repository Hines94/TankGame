// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "TankPlayerController.h"
#include "Engine/World.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("Player Controller not posessing a tank"));
	}


	UE_LOG(LogTemp, Warning, TEXT("Controlled Tank is: %s"), *(ControlledTank->GetName()));

}

void ATankPlayerController::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime);
	AimTowardsCrosshair();
}

ATank * ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
	
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) {
		return;
	}
	FVector HitLocation; //OutPerameter
	if (GetSightRayHitLocation(HitLocation))
	{
		GetControlledTank()->AimAt(HitLocation);
	}
}
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	//Find Crosshair position
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(ViewportSizeX*CrosshairXLocation, ViewportSizeY*CrosshairYLocation);

	// De-project the screen postition of the crosshair to a world position
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		//Use Line trace to see what we hit 
		GetLookVectorHitLocation(LookDirection,OutHitLocation);
	}
	//Line Trace along that LookDirection to see where we hit
	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CamerWorldLocation; // To be discarded

	DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		CamerWorldLocation,
		LookDirection);

	return true;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection,FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + LookDirection * LineTraceRange;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation,EndLocation ,ECC_Visibility)) {
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0);
	return false;
}
