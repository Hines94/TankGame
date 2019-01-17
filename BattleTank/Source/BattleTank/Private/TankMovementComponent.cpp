// Copyright Jack Hines.

#include "TankMovementComponent.h"
#include "TankTracks.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UTankMovementComponent::UTankMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankMovementComponent::Initialise(UTankTracks*LeftTrackToSet, UTankTracks*RightTrackToSet)
{
	if (!ensure(LeftTrackToSet &&RightTrackToSet)) { return; }
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
	LeftTrack->MasterMovementComp = this;
	RightTrack->MasterMovementComp = this;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!ensure(LeftTrack &&RightTrack)) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);

}

void UTankMovementComponent::IntendMoveRight(float Throw)
{
	if (!ensure(LeftTrack &&RightTrack)) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);

}

//Getters for debug
float UTankMovementComponent::GetCurrentRPM()
{
	return CurrentRPM;
}
float UTankMovementComponent::GetCurrentTorque()
{
	return CurrentTorque;
}
float UTankMovementComponent::GetCurrentRPMBump()
{
	return CurrentRPMBump;
}
float UTankMovementComponent::GetCurrentVelocity()
{
	return CurrentVelocity;
}
float UTankMovementComponent::GetCurrentVelocityPunishment()
{
	return VelocityPunishment;
}
float UTankMovementComponent::GetCurrentAcceclerationPunishment()
{
	return AccelerationPunishment;
}


void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{
	//No need to call super as were replacing functionality
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();
	auto TankForwardDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto ForwardThrow = FVector::DotProduct(AIForwardIntention, TankForwardDirection);
	auto RightThrow = FVector::CrossProduct(AIForwardIntention, TankForwardDirection);
	IntendMoveForward(ForwardThrow);
	IntendMoveRight(RightThrow.Z);
}


void UTankMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float RPM = GetRPMFromAxle();
	//UE_LOG(LogTemp, Warning, TEXT("RPM: %f"), RPM);
	float CurrentAcceleration = 0;
	CurrentVelocity = CalculateTankOverallVeloctiy(CurrentAcceleration);
	//UE_LOG(LogTemp, Warning, TEXT("CurrentVelocity: %f"), CurrentVelocity);
	//for turning on spot - needs more force for some reason.

	RPM = BumpRPMIfNeccessary(RPM,DeltaTime,CurrentAcceleration);

	float Torque = GetEngineToruqe(RPM);
	Torque = GetGearToruqe(Torque);

	CurrentTorque = Torque;

	float FinalDriveForce = Torque * TempTorqueToForce;

	FinalDriveForce = ApplyPunisments(FinalDriveForce,CurrentAcceleration);
	//UE_LOG(LogTemp, Warning, TEXT("FinalForce: %f"), FinalDriveForce);

	if (CurrentVelocity < MaxVelocityForSpotTurnBump && ForwardInput == 0)
	{
		//Linear interp
		float AdditionalForceMulti = (MaxVelocityForSpotTurnBump - CurrentVelocity)*RotateOnSpotPowerMult;
		RightTrack->MasterDriveTrack(FinalDriveForce * AdditionalForceMulti);
		LeftTrack->MasterDriveTrack(FinalDriveForce * AdditionalForceMulti);
	}
	else
	{
		RightTrack->MasterDriveTrack(FinalDriveForce);
		LeftTrack->MasterDriveTrack(FinalDriveForce);
	}
}
float UTankMovementComponent::ApplyPunisments(float FinalPower, float CurrentAcceleration)
{
	//UE_LOG(LogTemp, Warning, TEXT("PowerIn: %f"), FinalPower);
	float FinalDriveForce = FinalPower;
	VelocityPunishment = 0;
	VelocityPunishment = 1 + ((FMath::Square(CurrentVelocity) / FMath::Square(MaxSpeedApplyForce)) *VelocityPunishmentRatio);
	FinalDriveForce = FinalDriveForce / VelocityPunishment;
	//UE_LOG(LogTemp, Warning, TEXT("Velocity Punishment: %f"),(1/VelPunishment));
	AccelerationPunishment = 1 + FMath::Square(CurrentAcceleration * AccelerationPunishmentRatio);
	FinalDriveForce = FinalDriveForce / AccelerationPunishment;
	//UE_LOG(LogTemp, Warning, TEXT("Accel Punishment: %f"), (1/ AccelerationPunishment));
	return FinalDriveForce;
}

//Bumps rpm if in first and going nowhere
float UTankMovementComponent::BumpRPMIfNeccessary(float RPMIn, float DeltaSecs,float CurrentAcceleration)
{
	//Get throttle
	float Throttle = FMath::Abs(LeftTrack->GetThrottle());
	Throttle += FMath::Abs(RightTrack->GetThrottle());
	Throttle = Throttle / 2;
	bool IsThrottlingHard = false;
	if ((Throttle > 0.45|| Throttle < -0.45))
	{
		IsThrottlingHard = true;
	}
	//if we are Throttling and not going anywhere
	if (CurrentAcceleration < RPMBumpAccelerationMax && IsThrottlingHard)
	{
		//not in first early return
		if (CurrentGear != 0) {return RPMIn + RPMBump;}
		if (RPMBump < EngineSetup.MaxRPM * RPMBumpMaxRatio)
		{
			RPMBump += RPMBumpIncreaseRate * DeltaSecs;
		}
	}
	else if (RPMBump > 0)
	{
		if (RPMBump - DeltaSecs * RPMBumpDecreaseRate > 0)
		{
			RPMBump -= DeltaSecs * RPMBumpDecreaseRate;
		}
		else
		{
			RPMBump = 0;
		}
		 
	}

	CurrentRPMBump = RPMBump;

	//UE_LOG(LogTemp, Warning, TEXT("ThrottlingHard: %s"),(IsThrottlingHard ? TEXT("True") : TEXT("False")) );
	//UE_LOG(LogTemp, Warning, TEXT("CurrentAccel: %f"), CurrentAcceleration);
	//UE_LOG(LogTemp, Warning, TEXT("RPM Bump: %f"), RPMBump);
	return RPMIn + RPMBump;
	
}

float UTankMovementComponent::CalculateTankOverallVeloctiy(float & OutAcceleration)
{
	FVector Dist = (GetOwner()->GetActorLocation() - PriorLoc);
	//cm to M
	Dist = Dist / 100;
	
	float Velocity = FMath::Abs(Dist.Size());
	float TimeDiff = ((UGameplayStatics::GetTimeSeconds(GetWorld()) - PriorSecs));
	Velocity = Velocity / TimeDiff;
	OutAcceleration = (Velocity / TimeDiff);
	PriorLoc = GetOwner()->GetActorLocation();
	PriorSecs = UGameplayStatics::GetTimeSeconds(GetWorld());
	
	return Velocity;
	
}

void UTankMovementComponent::DiffAdditionalForce(float Force, UTankTracks* TrackComp)
{
	if (!ensure(LeftTrack && RightTrack)) { return; }
	if (TrackComp == LeftTrack)
	{
		RightTrack->AdditionalForceFromDiff = Force * DiffTransferResolution;
	}
	else if (TrackComp == RightTrack)
	{
		LeftTrack->AdditionalForceFromDiff = Force * DiffTransferResolution;
	}
}

float UTankMovementComponent::GetRPMFromAxle()
{
	//Have to get axle angular first
	if (!ensure(LeftTrack && RightTrack)) { return 0; }
	float TrackVelocity = FMath::Abs(RightTrack->GetTrackVelocity());
	TrackVelocity += FMath::Abs(LeftTrack->GetTrackVelocity());
	//UE_LOG(LogTemp, Warning, TEXT("Vel: %f"), TrackVelocity);
	float GearRatio = 1;
	if (CurrentGear > 0)
	{
		GearRatio = TransmissionSetup.ForwardGears[CurrentGear].Ratio;
	}
	else if(CurrentGear < 0)
	{
		GearRatio = TransmissionSetup.ReverseGearRatio;
	}
	else
	{
		GearRatio = TransmissionSetup.NeutralGearUpRatio;
	}
	float RPM = GearRatio * TrackVelocity;
	RPM = RPM * TransmissionSetup.FinalRatio;
	RPM = ((RPM * 60)/2);
	RPM = RPM * PI;

	//Mitigate RPM to stop sudden changes from hills etc
	float ReturnRPM = 0;
	ReturnRPM = ((RPM - PriorRPM) * RPMChangeRatio);
	ReturnRPM += PriorRPM;
	PriorRPM = ReturnRPM;
	return RPM;
}

float UTankMovementComponent::GetEngineToruqe(float RPM)
{
	float ClampedRPM = FMath::Clamp(RPM, 0.f, EngineSetup.MaxRPM);
	CurrentRPM = ClampedRPM;
	return EngineSetup.TorqueCurve.EditorCurveData.Eval(ClampedRPM);
}

float UTankMovementComponent::GetGearToruqe(float EngineTorque)
{
	float GearRatio = 1;
	if (CurrentGear > 0)
	{
		GearRatio = TransmissionSetup.ForwardGears[CurrentGear].Ratio;
	}
	if (CurrentGear < 0)
	{
		GearRatio = -TransmissionSetup.ReverseGearRatio;
	}
	else
	{
		GearRatio = TransmissionSetup.NeutralGearUpRatio;
	}
	float Torque = GearRatio * EngineTorque;
	Torque = Torque * TransmissionSetup.FinalRatio;
	Torque = Torque * TransmissionSetup.TransmissionEfficiency;

	return Torque;
}