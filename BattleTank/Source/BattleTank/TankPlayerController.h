// Copyright Jack Hines.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h" //Must be the last include

class UTankAimingComponent;
/**
 *Responsible for helping the player aim.
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent*AimingCompRef);

private:
	

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;


	//Start the tank moving barrel so that shot would hit where crosshair intersects world.
	void AimTowardsCrosshair(); 	
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;
	UPROPERTY(EditDefaultsOnly)
	float CrosshairXLocation = 0.5;
	UPROPERTY(EditDefaultsOnly)
	float CrosshairYLocation = 0.3333333333;	
	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 10000000;
	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;
};
