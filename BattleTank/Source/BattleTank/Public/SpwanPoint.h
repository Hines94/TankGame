// Copyright Jack Hines

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpwanPoint.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API USpwanPoint : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpwanPoint();	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		TSubclassOf<AActor> SpawnClass;

		
	
};
