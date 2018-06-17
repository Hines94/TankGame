// Copyright Jack Hines

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Mortar.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMortarDelegate);


UCLASS()
class BATTLETANK_API AMortar : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMortar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent*AimingCompRef);

public:	
	// Called to bind functionality to input
	FMortarDelegate OnDeath; //TODO make death reciever.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UPROPERTY(EditDefaultsOnly, Category = "Startup")
		int32 StartingHealth = 100;
	UPROPERTY(EditDefaultsOnly, Category = "Startup")
		int32 CurrentHealth = StartingHealth;
	
};
