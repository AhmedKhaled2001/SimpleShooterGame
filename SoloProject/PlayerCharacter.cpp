// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "Weapon.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnemyCharacter.h"
#include "DrawDebugHelpers.h"





//include the gun and make the gun->pulltrigger
// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FuryMode = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Fury Mode go prr Mesh"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("asd"));
	Health = MaxHealth;
	Magic = MaxMagic;
	
	
	//Weapon = GetWorld()->SpawnActor<AWeapon>(M4A1Class);
			
	//Weapon->AttachToSocketSelection(TEXT("WeaponSocket"), GetMesh());
		
			

}

bool APlayerCharacter::IsDead() const
{
	return Health <= 0;
}

bool APlayerCharacter::IsGrabable() 
{
	
	
	FHitResult Locked;
	FVector HitDirection;
	if	(PickUpTrace(Locked, HitDirection))
	{
         if (Locked.Actor->IsA<AWeapon>())
         {
            UE_LOG(LogTemp, Warning, TEXT("ASDASD"));
			HitWeapon =Cast<AWeapon> (Locked.GetActor());
			return true;
		 }
	}
	return false;
}

bool APlayerCharacter::IsAttachedToPlayer() 
{
	if (HitWeapon->IsAttachedTo(this))
	{
			return true;
		
	}
	else
	{
		return false;
	}
	
}



// Called every frame

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed,this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("PickUpWeapon"),IE_Pressed,this , &APlayerCharacter::PickUp);
	PlayerInputComponent->BindAction(TEXT("HoldWeapon"),IE_Pressed,this , &APlayerCharacter::HoldWeapon);
	PlayerInputComponent->BindAction(TEXT("PutWeaponOnBack"),IE_Pressed,this , &APlayerCharacter::PutWeaponOnBack);
	PlayerInputComponent->BindAction(TEXT("Reload"),IE_Pressed,this , &APlayerCharacter::ReloadCurrent);
	PlayerInputComponent->BindAction(TEXT("StartFire"),IE_Pressed,this , &APlayerCharacter::StartFire);
	PlayerInputComponent->BindAction(TEXT("StopFire"),IE_Released,this , &APlayerCharacter::StopFire);
	PlayerInputComponent->BindAction(TEXT("SlowMo"),IE_Pressed,this , &APlayerCharacter::SlowMotion);
	PlayerInputComponent->BindAction(TEXT("SlowMo"),IE_Released,this , &APlayerCharacter::StopSlowMotion);
	PlayerInputComponent->BindAction(TEXT("Fury"),IE_Pressed,this , &APlayerCharacter::EnterFury);
}

float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) 
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);
	if (IsDead())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageToApply;

}

void APlayerCharacter::StartFire() 
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	
				if (bisReloading)
				{
					return;
				}
				
	
			if (HitWeapon== nullptr)
			{
				return;
			}
	
		if (IsAttachedToHand)
		{
		
				Fury = HitWeapon->WeaponFury;
				GunAction();
				GetWorldTimerManager().SetTimer(IsFiring, this, &APlayerCharacter::StartFire, HitWeapon->FireRate);
				UE_LOG(LogTemp,Warning, TEXT("FURY!!! %f"), Fury);
				Health = FMath::Clamp(Health + 1.0f, 0.0f, MaxHealth);

				
			
		
			
		}
		
	
	
}

float APlayerCharacter::FuryPercentage() 
{
	return Fury/MaxFury;
}


void APlayerCharacter::PutWeaponOnBack() 
{

	if (bisReloading)
	{
		return;
	}
				
	if (HitWeapon == nullptr)
	{
		return;
	}
	
	if (IsAttachedToHand)
	{
		HitWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("BackWeapon"));
	}
		IsAttachedToBack = true;
		IsAttachedToHand = false;
	
}

void APlayerCharacter::MoveForward(float Value) 
{
	AddMovementInput(GetActorForwardVector(), Value);
	
}

void APlayerCharacter::MoveRight(float Value) 
{
	AddMovementInput(GetActorRightVector(), Value);
}

void APlayerCharacter::PickUp() 
{

	if (IsAttachedToBack || IsAttachedToHand)
	{
		return;
	}
	
	if (IsGrabable())
	{
		
		HitWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("BackWeapon"));
		HitWeapon->SetOwner(this);
		IsAttachedToBack = true;
		IsAttachedToHand = false;
	}
}

float APlayerCharacter::HealthPercentage() 
{
	return Health/MaxHealth;
}

bool APlayerCharacter::PickUpTrace(FHitResult &Locked, FVector &HitDirection) 
{
	APlayerController* PlayerContr = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    FVector Location;
	FRotator Rotation;
	PlayerContr->GetPlayerViewPoint(Location, Rotation);
	HitDirection = -Rotation.Vector();
	FVector End = Location + Rotation.Vector()* MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	return GetWorld()->LineTraceSingleByChannel(Locked, Location, End, ECC_GameTraceChannel2, Params);
	
	
    
}

void APlayerCharacter::ReloadCurrent() 
{
	if (IsAttachedToHand)
	{
		bisReloading = true;
		PlayAnimMontage(ReloadMontage, 1.0f);
		HitWeapon->ReloadWeapon();
		GetWorldTimerManager().SetTimer( ReloadDone, this, &APlayerCharacter::DoneReloading, 3.27f);
	}
	
}

void APlayerCharacter::DoneReloading() 
{
	bisReloading = false; 
}

void APlayerCharacter::StopFire() 
{
	GetWorldTimerManager().ClearTimer(IsFiring);
}

void APlayerCharacter::GunAction() 
{

	HitWeapon->PullTrigger();
	if (HitWeapon->Ammo>0.0f)
	{
		PlayAnimMontage(Recoil,1.0f);
	}
	

}

void APlayerCharacter::SlowMotion() 
{	
	if (Magic>0.0f)
	{
		bIsInSlowMotion = true; 
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
	}
	else
	{
		StopSlowMotion();
	}
	
	
	
}


void APlayerCharacter::StopSlowMotion() 
{
	bIsInSlowMotion = false; 
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void APlayerCharacter::EnterFury() 
{
	if (Fury == 100.0f)	
	{
		HitWeapon->WeaponFury = 0.0f;
		Fury = 0.0f;
		PlayAnimMontage(FuryEntery,1.0f);
		UGameplayStatics::SpawnEmitterAttached(FuryEffect, GetMesh(), TEXT("FuryEffect"));
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController == nullptr)
		{
			return;
		}
			
	TArray<FHitResult> OutHits;
	FVector SweepStart = GetActorLocation();
	FVector SweepEnd = GetActorLocation();

	// create a collision sphere
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(500.0f);

	// draw collision sphere
	DrawDebugSphere(GetWorld(), GetActorLocation(), MyColSphere.GetSphereRadius(), 50, FColor::Purple, true, 999);
	
	// check if something got hit in the sweep
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, MyColSphere);

	if (isHit)
	{
		// loop through TArray
		for (auto& Hitterz : OutHits)
		{
			if (GEngine) 
			{
				// screen log information on what was hit

				if (Hitterz.GetActor()->IsA<AEnemyCharacter>())
				{
					Hitterz.GetActor()->TakeDamage(100.0f,FDamageEvent() , PlayerController,this);
					
				}
				
				// uncommnet to see more info on sweeped actor
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("All Hit Information: %s"), *Hit.ToString()));
			}						
		}
	}





			
		
	}






	
}

bool APlayerCharacter::FuryAbilityHit() 
{
	TArray<FHitResult> OutHits;
	FVector SweepStart = GetActorLocation();
	FVector SweepEnd = GetActorLocation();

	// create a collision sphere
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(500.0f);

	// draw collision sphere
	DrawDebugSphere(GetWorld(), GetActorLocation(), MyColSphere.GetSphereRadius(), 50, FColor::Purple, true, 999);
	
	// check if something got hit in the sweep
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, MyColSphere);

	if (isHit)
	{
		// loop through TArray
		for (auto& Hitterz : OutHits)
		{
			if (GEngine) 
			{
				// screen log information on what was hit

				if (Hitterz.GetActor()->IsA<AEnemyCharacter>())
				{
					PlayerCharacterHitFury = Hitterz.GetActor();
					return true;
				}
				
				// uncommnet to see more info on sweeped actor
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("All Hit Information: %s"), *Hit.ToString()));
			}						
		}
	}
return false;
}









void APlayerCharacter::HoldWeapon() 
{	

	if (HitWeapon == nullptr)
	{
		return;
	}
		
	if (IsAttachedToPlayer())
	{
		HitWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));	
		HitWeapon->SetOwner(this);

		IsAttachedToHand = true;
		IsAttachedToBack = false;
	}
}


