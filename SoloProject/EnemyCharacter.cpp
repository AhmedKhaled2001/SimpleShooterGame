// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) 
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	bIsTakingDamage = true;
	DamageApplied = FMath::Min(Health, DamageApplied);
	Health -= DamageApplied;
	UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);
	if (IsDead())
	{
	
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	GetWorldTimerManager().SetTimer(DoneTakingDamage, this, &AEnemyCharacter::MakeItFalse, 1.32f );
	return DamageApplied;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// if (IsDead())
	// {
	// 	Destroy();
	// }
	
	
	
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyCharacter::GetHealthPer() 
{
	return Health/MaxHealth;
}

void AEnemyCharacter::MakeItFalse() 
{
	bIsTakingDamage = false;
}


bool AEnemyCharacter::IsDead() 
{
	return Health <= 0;

}


// void AEnemyCharacter::HitEnemy() 
// {

	
	
// 	FHitResult Hit;
// 	FVector ShotDirection;
// 	bool bSuccess = TraceForHit(Hit, ShotDirection);
// 	if (bSuccess)
// 	{
		
// 		AActor* HitActor = Hit.GetActor();
// 		if (HitActor != nullptr)
// 		{
// 			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
// 			AController* OwnerController = GetOwnerController();
// 			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
// 		}
		
// 	}
	
// }

// bool AEnemyCharacter::TraceForHit(FHitResult &Hit, FVector &ShotDirection) 
// {

// 	AController* OwnerController = GetOwnerController();
// 	if (OwnerController == nullptr)
// 	{
// 		return false;
// 	}
// 	FVector Location;
// 	FRotator Rotation;
// 	OwnerController->GetPlayerViewPoint(Location, Rotation);
// 	ShotDirection = -Rotation.Vector();
// 	FVector End = Location + Rotation.Vector()* MaxRange;
// 	FCollisionQueryParams Params;
// 	Params.AddIgnoredActor(this);
// 	Params.AddIgnoredActor(GetOwner());
// 	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECC_GameTraceChannel1, Params);
// }

// AController* AEnemyCharacter::GetOwnerController() 
// {
	

// 	APawn*  OwnerPawn = Cast<APawn>(GetOwner());
// 	if (OwnerPawn == nullptr)
// 	{
// 		return nullptr;
// 	}
// 	return OwnerPawn->GetController();
// }


