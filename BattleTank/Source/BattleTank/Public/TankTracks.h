// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTracks.generated.h"

/**
Tank Track is used to set maximum driving force and apply forces to the tank.
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTracks : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	//Sets the throttle between -1 and +1
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetThrottle(float Throttle);

	float AdditionalForceFromDiff = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 WheelLinetraceOffset = 5;
		
	class UTankMovementComponent* MasterMovementComp = nullptr;

	float GetTrackVelocity();

	float GetThrottle();
	
	void MasterDriveTrack(float CurrentDriveForce);

protected:
	//Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

private:
	UTankTracks();
	TArray<class ASprungWheel*>GetWheels() const;
	virtual void BeginPlay() override;
	void DriveTrack(float CurrentThrottle, float Force);
	//how many wheels were on the floor last frame?
	int32 PreviousContacts = 0;
	float CurrentTrackThrottle = 0;
	FVector PriorLoc;
	float PriorSecs = 0;
};
