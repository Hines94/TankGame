// Copyright Jack Hines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "Structs/TrackedVehicleEngineStructs.h"
#include "TankMovementComponent.generated.h"

class UTankTracks;


/**
 For movement using the WASD or sticks 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()

public:
	UTankMovementComponent();

	//Pulled from N Wheeled vehicle
	UPROPERTY(EditAnywhere, Category = Vehicle)
	int32 NumOfWheels;

	// Engine.
	UPROPERTY(EditAnywhere, Category = MechanicalSetup)
	FVehicleEngineTrackedData EngineSetup;

	// Differential.
	UPROPERTY(EditAnywhere, Category = MechanicalSetup)
	FVehicleDifferentialTrackedData DifferentialSetup;

	//UPROPERTY(EditAnywhere, Category = MechanicalSetup).
	FDrivenWheelData DrivenWheelSetup;

	// Transmission Data.
	UPROPERTY(EditAnywhere, Category = MechanicalSetup)
	FVehicleTransmissionTrackedData TransmissionSetup;

	// Maximum steering versus forward speed (Km/h).
	UPROPERTY(EditAnywhere, Category = SteeringSetup)
	FRuntimeFloatCurve SteeringCurve;

	//virtual void Serialize(FArchive & Ar) override;
	//virtual void ComputeConstants() override;


	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankTracks*LeftTrackToSet, UTankTracks*RightTrackToSet);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendMoveForward(float Throw);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendMoveRight(float Throw);

	//Getters for debug
	UFUNCTION(BlueprintCallable, Category = "VehicleInfo")
	float GetCurrentRPM();
	UFUNCTION(BlueprintCallable, Category = "VehicleInfo")
	float GetCurrentGear();
	UFUNCTION(BlueprintCallable, Category = "VehicleInfo")
	float GetCurrentRatio();
	UFUNCTION(BlueprintCallable, Category = "VehicleInfo")
	float GetCurrentTorque();
	UFUNCTION(BlueprintCallable, Category = "VehicleInfo")
	float GetCurrentRPMBump();
	UFUNCTION(BlueprintCallable, Category = "VehicleInfo")
	float GetCurrentVelocity();
	UFUNCTION(BlueprintCallable, Category = "VehicleInfo")
	float GetCurrentVelocityPunishment();
	UFUNCTION(BlueprintCallable, Category = "VehicleInfo")
	float GetCurrentAcceclerationPunishment();


	//Ammount of power a slipping track transfers to other side
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DiffTransferResolution = 0.5;
	
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction);

	void AddDownForce(float Force);



	float CalculateTankOverallVeloctiy(float & OutAcceleration);

	//If track is slipping then nadd more force to other - needed by track
	void DiffAdditionalForce(float Force, UTankTracks* TrackComp);

	//max velocity we can apply force to stop crazy accels
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxSpeedApplyForce = 50;
	//TODO Temporary torque to force converter
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TempTorqueToForce = 40000;
	//Punishment to give on square of acceleration to stop crazy accels
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AccelerationPunishmentRatio = 0.01;
	//Punishment to give on square of Velocity to max vel to stop crazy accels
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float VelocityPunishmentRatio = 0.1;
	//Change Per frame to smooth out RPM changes
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RPMChangeRatio = 0.3;
	//additional power for turning on spot - doesnt quite work as well
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TurningOnSpot)
	float RotateOnSpotPowerMult = 3;
	//max velocity we can be at to artificially bump force
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TurningOnSpot)
	float MaxVelocityForSpotTurnBump = 1;

	//If we are below this speed and hitting the throttle and going nowhere then bump our RPM
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = RPMBump)
	float RPMBumpAccelerationMax = 50;
	//In 1 sec if we need to bump the RPM by how much
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = RPMBump)
	float RPMBumpIncreaseRate = 2000;
	//In 1 sec if we need to bump the RPM by how much
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = RPMBump)
	float RPMBumpDecreaseRate = 4000;
	//can bump this percentage of max rpm
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = RPMBump)
	float RPMBumpMaxRatio = 0.6;

private:
	UTankTracks* LeftTrack = nullptr;
	UTankTracks* RightTrack = nullptr;

	//Called from the pathfinding logic by the AI controllers
	void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	float CurrentVelocity = 0;
	
	//Gearbox
	int32 CurrentGear = 0;
	void CheckChangeGear(float DeltaTime);
	bool bChangingUp = false;
	bool bChangingDown = false;
	float ChangingTimer = 0;
	float CurrentGearRatio = 0;
	bool bNeutralGearUp = false;

	//Prior location of tank for turn on spot
	FVector PriorLoc;
	float PriorSecs = 0;

	//Kicks in if we are in first and going nowhere
	float RPMBump = 0;

	//For smoothing of RPM Changes
	float PriorRPM = 0;
	
	float ForwardInput = 0;

	//Methods
	float GetRPMFromAxle();
	float GetEngineToruqe(float RPM);
	float GetGearToruqe(float EngineRPM);
	float BumpRPMIfNeccessary(float RPMIn, float DeltaSecs, float CurrentAcceleration);
	float ApplyPunisments(float FinalPower, float CurrentAcceleration);
	float GetCurrentRatioMethod();

	//for headsup / debug
	float CurrentRPM = 0;
	float CurrentTorque = 0;
	float CurrentRPMBump = 0;
	float VelocityPunishment = 0;
	float AccelerationPunishment = 0;

};