// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"
class UStaticMeshComponent;
class USceneComponent;
UCLASS()
class SOLOPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	
	 void PullTrigger();
	UPROPERTY(BlueprintReadWrite)
	float Ammo;
	UPROPERTY(BlueprintReadWrite)
	float  MaxAmmo = 30.0f;
	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();
	UPROPERTY(EditAnywhere)
	float FireRate = 0.2f;

	float WeaponFury = 0.0f;
private:
	UPROPERTY(EditAnywhere)
	UParticleSystem *MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem *ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase *ImpactSound;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	float MaxRange = 1000.0f;
	UPROPERTY(EditAnywhere)
	float Damage = 20.0f;
	//bool bReloading = false; 
	//FTimerHandle ReloadingTimer;
	//void ReloadingFinished();



	bool GunTrace(FHitResult &Hit, FVector &ShotDirection);
	AController* GetOwnerController();





//Fury System
};
