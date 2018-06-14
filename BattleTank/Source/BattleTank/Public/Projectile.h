// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

class UParticleSystemComponent;
class URadialForceComponent;

UCLASS()
class BATTLETANK_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	void LaunchProjectile(float Speed);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UProjectileMovementComponent* ProjectileMovement = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DestroyDelay = 10.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ProjectileDamage = 10.f;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* CollisionMesh = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent*LaunchBlast = nullptr; //TODO Protect
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent*ImpactBlast = nullptr; //TODO Protect
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent* ExplosionForce = nullptr;
	void OnTimerExpire();

};
