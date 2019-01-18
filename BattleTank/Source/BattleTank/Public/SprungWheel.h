// Copyright Jack Hines

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SprungWheel.generated.h"

class UPhysicsConstraintComponent;
class USphereComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API ASprungWheel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASprungWheel();	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	//Set from tracks
	AActor* TankOwner = nullptr;
	TArray <AActor*> OtherWheels;
	int32 LineTraceOffset = 5;

	bool CheckIfGroundContact();

	void ApplyForce(float Force);
	void ApplyDownForce(float Force);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	void SetupSpringConstraint();
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* Axle = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* Wheel = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhysicsConstraintComponent* MassAxleConstraint = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhysicsConstraintComponent* AxleWheelConstraint = nullptr;
	//UFUNCTION()
	//void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	//float TotalForceMagnitudeThisFrame = 0.f;
};
