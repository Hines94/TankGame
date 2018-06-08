// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class ATank;

/**
 *Controls move towards player and fires
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

protected:
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

private:
		//TODO Make variable
		float AcceptanceRadius = 3;
};
