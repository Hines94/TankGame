// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTracks.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"
#include "TankMovementComponent.h"
#include "Tank.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"



UTankTracks::UTankTracks()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UTankTracks::GetTrackVelocity()
{
	FVector Dist = (GetComponentLocation() - PriorLoc);
	Dist = Dist / 100;
	float Velocity = FMath::Abs(Dist.Size());
	Velocity = Velocity / ((UGameplayStatics::GetTimeSeconds(GetWorld()) - PriorSecs) );
	PriorLoc = GetComponentLocation();
	PriorSecs = UGameplayStatics::GetTimeSeconds(GetWorld());
	return Velocity;
}

float UTankTracks::GetThrottle()
{
	return CurrentTrackThrottle;
}

void UTankTracks::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTankTracks::MasterDriveTrack(float CurrentDriveForce)
{
	if (CurrentTrackThrottle != 0)
	{
		DriveTrack(CurrentTrackThrottle,CurrentDriveForce);
		CurrentTrackThrottle = 0;
		AdditionalForceFromDiff = 0;
	}
	else
	{
		if (MasterMovementComp != nullptr)
		{
			MasterMovementComp->DiffAdditionalForce(CurrentDriveForce, this);
		}
	}
}

TArray<ASprungWheel*> UTankTracks::GetWheels() const
{
	TArray<AActor*> OtherWheels;
	TArray<ASprungWheel*> ResultArray;
	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);
	for (USceneComponent* Child : Children)
	{
		auto SprungWheel = Cast<ASprungWheel>(Child->GetOwner());
		if (!SprungWheel) continue;

		//add owner for linetrace ignore
		if (Cast<ATank>(GetOwner()))
		{
			SprungWheel->TankOwner = GetOwner();
			SprungWheel->LineTraceOffset = WheelLinetraceOffset;
		}
		OtherWheels.Add(SprungWheel);
		ResultArray.Add(SprungWheel);
	}
	//Add other Wheels to each wheel
	for (ASprungWheel* SprungWheel : ResultArray)
	{
		SprungWheel->OtherWheels = OtherWheels;
	}
	return ResultArray;
}

void UTankTracks::BeginPlay()
{
	Super::BeginPlay();
}

void UTankTracks::SetThrottle(float Throttle)
{
	if (Throttle > 0.05 || Throttle < 0.05)
	{
		CurrentTrackThrottle += Throttle;
		CurrentTrackThrottle = FMath::Clamp<float>(CurrentTrackThrottle, -1, 1);
	}
	//CurrentTrackThrottle = FMath::Clamp<float>(Throttle, -1, 1);
}

void UTankTracks::DriveTrack(float CurrentThrottle,float Force)
{
	auto ForceApplied = CurrentThrottle * (Force + AdditionalForceFromDiff);
	auto Wheels = GetWheels();
	int32 WheelNum = Wheels.Num();
	auto ForcePerWheel = ForceApplied / WheelNum;
	float PreviousContactsRatio = (PreviousContacts);

	PreviousContacts = 0;
	UE_LOG(LogTemp, Warning, TEXT("Force: %f"), (ForcePerWheel / 10000));
	for (ASprungWheel* Wheel : Wheels)
	{
		if (Wheel->CheckIfGroundContact())
		{
			Wheel->ApplyForce(ForcePerWheel *(1 + (WheelNum -PreviousContactsRatio)) );
			PreviousContacts += 1;
		}
		
	}
}