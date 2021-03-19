// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"
class AWeapon;
class USkeletalMeshComponent;
class UUSerWidget;
UCLASS()
class SOLOPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintPure)
	float HealthPercentage();
	UFUNCTION(BlueprintPure)
	bool IsDead() const;
	
	
	UFUNCTION(BlueprintPure)
	bool IsGrabable();
	UFUNCTION(BlueprintPure)
	bool IsAttachedToPlayer();
	// Called every frame

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;

	void StartFire();
	

	UPROPERTY(BlueprintReadOnly)
	bool IsAttachedToBack;
	UPROPERTY(BlueprintReadOnly)
	bool IsAttachedToHand;
	UPROPERTY(BlueprintReadWrite)
	AWeapon* HitWeapon;
	UFUNCTION(BlueprintPure)
	float FuryPercentage();
private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void PutWeaponOnBack();
	void HoldWeapon();
	void PickUp();
	
	UPROPERTY(EditDefaultsOnly)
	float Health;
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.0f;
	bool PickUpTrace(FHitResult &Locked, FVector &HitDirection) ;
	float MaxRange = 2000.0f;
	void ReloadCurrent();
	UPROPERTY(EditAnywhere)
	UAnimMontage* ReloadMontage;
	bool bisReloading = false; 
	void DoneReloading();
	FTimerHandle ReloadDone;
	FTimerHandle IsFiring;
	void StopFire();
	void GunAction();
	
	UPROPERTY(EditDefaultsOnly)
	float Magic;
	UPROPERTY(EditAnywhere)
	float MaxMagic = 100.0f;






	void SlowMotion();
	FTimerHandle ManaLossRate;
	void ManaLoss();
	void StopSlowMotion();
	bool bIsInSlowMotion = false; 
	




	//Fury System
	UPROPERTY()
	float Fury = 0.0f;
	float MaxFury = 100.0f;

	UPROPERTY(EditAnywhere)
	UAnimMontage* FuryEntery;

	UPROPERTY(EditAnywhere)
	UParticleSystem *FuryEffect;
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* FuryMode;
	void EnterFury();
	TArray<FHitResult> FuryOutHits;
	bool FuryAbilityHit();

	UPROPERTY(EditAnywhere)
	UAnimMontage* Recoil;
	AActor* PlayerCharacterHitFury;
	
};
