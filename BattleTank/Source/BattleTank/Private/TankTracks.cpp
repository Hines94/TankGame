// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTracks.h"

void UTankTracks::SetThrottle(float Throttle)
{
	auto ThrottleClamp = FMath::Clamp<float>(Throttle,-1,1);
	auto ForceApplied = GetForwardVector()*ThrottleClamp*TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	
	TankRoot->AddForceAtLocation(ForceApplied,ForceLocation);
}

