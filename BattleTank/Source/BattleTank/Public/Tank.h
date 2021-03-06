// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UTankAimingComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);


UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Returns current health as a percentage of starting health betweeen 0 and 1
	UFUNCTION(BlueprintPure,Category = "Health")
	float GetHealthPercent() const;

public:	
	FTankDelegate OnDeath;
	// Called to bind functionality to input
	ATank();
	class UTankMovementComponent * GetTankMovementComponent();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UPROPERTY(EditDefaultsOnly, Category = "Startup")
	int32 StartingHealth = 100;
	UPROPERTY(EditDefaultsOnly, Category = "Startup")
	int32 CurrentHealth;
private:

};
