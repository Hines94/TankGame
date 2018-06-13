// Copyright Jack Hines.


#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	if (!ensure(AimingComponent)) { return; }
	FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime);
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn()) { return; } //Eg depossesed
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) {return;}
	FVector HitLocation; //OutPerameter
	if (GetSightRayHitLocation(HitLocation))
	{
		AimingComponent->AimAt(HitLocation);
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
		return GetLookVectorHitLocation(LookDirection,OutHitLocation);
	}
	//not hit anything - false
	return false;
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
