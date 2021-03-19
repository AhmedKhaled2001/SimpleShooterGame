// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class SOLOPROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;
	UFUNCTION(BlueprintPure)
	bool IsDead();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(BlueprintReadOnly)
	bool bIsTakingDamage = false; 
	UFUNCTION(BlueprintPure)
	float GetHealthPer();
	
private:
UPROPERTY(EditDefaultsOnly)
float Health;
UPROPERTY(EditAnywhere)
float MaxHealth = 100.0f;
FTimerHandle DoneTakingDamage;
void MakeItFalse();

	UPROPERTY(EditAnywhere)
	UParticleSystem *DeathEffect;
	FTimerHandle DeathHandler;
void PostDeathSequence();
	
// UPROPERTY(EditAnywhere)
// float MaxRange = 100.0f;
// UPROPERTY(EditAnywhere)
// float Damage = 20.0f;

// bool TraceForHit(FHitResult &Hit, FVector &ShotDirection);
// AController* GetOwnerController();


};
