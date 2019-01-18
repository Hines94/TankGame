// Copyright Jack Hines

#include "SprungWheel.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
//#include "Runtime/Engine/Classes/Engine/World.h"
#include "TankTracks.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	MassAxleConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("MassAxleConstraint"));
	SetRootComponent(MassAxleConstraint);	
	
	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	Axle->SetupAttachment(MassAxleConstraint);	
	
	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	Wheel->SetupAttachment(Axle);	

	AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleWheelConstraint"));
	AxleWheelConstraint->SetupAttachment(Axle);
	
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	Wheel->SetNotifyRigidBodyCollision(true);
	//Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit); //TODO fix on hit
	SetupSpringConstraint();
}

void ASprungWheel::SetupSpringConstraint()
{
	if (!GetAttachParentActor()) { return; }
	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());
	if (!BodyRoot) { return; }
	MassAxleConstraint->SetConstrainedComponents(BodyRoot, NAME_None, Axle, NAME_None);
	AxleWheelConstraint->SetConstrainedComponents(Axle, NAME_None, Wheel, NAME_None);
}

//void ASprungWheel::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
//{
//	ApplyForce();
//}

// Called every frame
//void ASprungWheel::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//	if (GetWorld() && //TrackOwner != nullptr)
//	{
//		GetWorld()
//	}
//	//if (GetWorld()->TickGroup == TG_PostPhysics)
//	//{
//	//	TotalForceMagnitudeThisFrame = 0;
//	//}
//}

bool ASprungWheel::CheckIfGroundContact()
{
	if (GetWorld() && TankOwner != nullptr)
	{
		FCollisionShape CollisionShape;
		float Radius = (Wheel->GetScaledSphereRadius() + 1);
		//UE_LOG(LogTemp, Warning, TEXT("Rad: %f"), Radius);
		CollisionShape.MakeSphere(Radius);
		FVector Start = Wheel->GetComponentLocation();
		FVector End = Start;
		Start += (GetActorUpVector() *(LineTraceOffset));
		End += (GetActorUpVector() * -(Radius + LineTraceOffset));
		FHitResult OutHit;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(TankOwner);
		QueryParams.AddIgnoredActors(OtherWheels);
		GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat(), Wheel->GetCollisionObjectType(), CollisionShape, QueryParams);

		if (OutHit.Actor != nullptr)
		{
			DrawDebugSphere(GetWorld(), End, Radius, 8, FColor::Green, false, 0.1, 0, 2);
			return true;
		}
		DrawDebugSphere(GetWorld(), End, Radius, 8, FColor::Red, false, 0.1, 0, 2);
	}
	return false;
}

//void ASprungWheel::AddDrivingForce(float ForceMagnitude)
//{
//	TotalForceMagnitudeThisFrame += ForceMagnitude;
//}

void ASprungWheel::ApplyForce(float Force) 
{
	//if (Force > 10000 || Force < -10000)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Force: %f"), (Force/1000));
	//}
	FVector ForceDir = (Axle->GetForwardVector());
	Wheel->AddForce((ForceDir)*Force);
	//apply slight down force to keep track stuck to ground - friction not woking hard enough
	//Wheel->AddForce((-Axle->GetUpVector()) *Force* 0.05);
}

void ASprungWheel::ApplyDownForce(float Force)
{
	FVector ForceDir = (Axle->GetUpVector());
	Axle->AddForce((ForceDir)*Force);
}