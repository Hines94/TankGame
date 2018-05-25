// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"

void UTurret::Rotate(float RelativeSpeed)
{
	auto Speed = FMath::Clamp<float>(RelativeSpeed, -1, 1);
	auto RotationChange = Speed * MaxDegreesPerSecond *GetWorld()->DeltaTimeSeconds;
	auto RawNewRotation = RelativeRotation.Yaw + RotationChange;
	SetRelativeRotation(FRotator(0, RawNewRotation, 0));
	//TDO Fix behind aiming
}

