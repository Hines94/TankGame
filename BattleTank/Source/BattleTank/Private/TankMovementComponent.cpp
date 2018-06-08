// Copyright Jack Hines.

#include "TankMovementComponent.h"
#include "TankTracks.h"

void UTankMovementComponent::Initialise(UTankTracks*LeftTrackToSet, UTankTracks*RightTrackToSet)
{
	if (!ensure(LeftTrackToSet &&RightTrackToSet)) { return; }
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
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
