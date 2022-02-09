// Copyright Epic Games, Inc. All Rights Reserved.

#include "AI_ProjectCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "AI_Project/Gameplay/DamageType/DamageType_Hook.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
//////////////////////////////////////////////////////////////////////////
// AAI_ProjectCharacter

void AAI_ProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsTargetLocked)
	{
		if(LockedOnTarget)
		{


			
			FVector LockedLocation = LockedOnTarget->GetActorLocation();
			LockedLocation.Z -= 50;
			const FRotator LockedRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedLocation);

			FRotator FinalControlRotation = FMath::RInterpTo(GetControlRotation(), LockedRotation,DeltaTime,7.f);
			FinalControlRotation.Roll = GetControlRotation().Roll;
			GetController()->SetControlRotation(FinalControlRotation);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Magenta, "Actor Name: " + GetName() + "  Health: " + FString::FromInt( Health));

}

AAI_ProjectCharacter::AAI_ProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	bIsTargetLocked = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	

	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAI_ProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent); 
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &AAI_ProjectCharacter::LightAttack);
	PlayerInputComponent->BindAction("TargetLock", IE_Pressed, this, &AAI_ProjectCharacter::TargetLock);
	PlayerInputComponent->BindAction("HeavyAttack", IE_Pressed, this, &AAI_ProjectCharacter::HeavyAttack);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAI_ProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAI_ProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAI_ProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAI_ProjectCharacter::LookUpAtRate);
	
}

void AAI_ProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	OnTakePointDamage.AddDynamic(this, &AAI_ProjectCharacter::TakePointDamage);
	OnTakeRadialDamage.AddDynamic(this, &AAI_ProjectCharacter::TakeRadialDamage);
}


bool AAI_ProjectCharacter::IsDead() const
{
	return Health <= 0;
}


void AAI_ProjectCharacter::HeavyAttack()
{
}


void AAI_ProjectCharacter::LightAttack()
{
	if (bIsAttacking)
	{
		bSaveAttack = true; /*Saves attack for combo*/
	}
	else
	{
		bIsAttacking = true;

		PlayLightAttack();

	}

}

void AAI_ProjectCharacter::PlayLightAttack()
{
	float AnimationLength = 0;
	if (GetCharacterMovement()->IsFalling())
	{
		if (JumpAttackMontage && !GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
		{

			AnimationLength = PlayAnimMontage(JumpAttackMontage);

		}
		else
		{
			AttackCount = 0;
			bSaveAttack = false;
			bIsAttacking = false;
		}

	}
	else
	{
		if (LightMeleeAttacks.Num() == 0 || AttackCount >= LightMeleeAttacks.Num())
		{
			return;
		}

		AnimationLength = PlayAnimMontage(LightMeleeAttacks[AttackCount]);

		FOnMontageEnded CompleteDelegate;
		CompleteDelegate.BindUObject(this, &AAI_ProjectCharacter::AttackMontageCompletedEvent);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(CompleteDelegate, LightMeleeAttacks[AttackCount]);



		AttackCount = (AttackCount + 1) % LightMeleeAttacks.Num();
	}



}


void AAI_ProjectCharacter::TargetLock()
{
	if(bIsTargetLocked) // Unlock the target
	{
		bIsTargetLocked = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else
	{
		//Logic for finding a target based on control rotation
		
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);

		FHitResult Hit;

		if (TargetObjectType.Num() == 0)
		{
			return;
		}

		FRotator ControlRotation = GetControlRotation();
		ControlRotation.Roll = 0;
		ControlRotation.Pitch = 0;
		const FVector CameraForward = ControlRotation.Vector();
		
		const bool IsHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), GetActorLocation(),
			GetActorLocation() + CameraForward * 1000,
			300, TargetObjectType, false, ActorsToIgnore
			, EDrawDebugTrace::None, Hit, true);
		if (IsHit)
		{
			LockedOnTarget = Hit.GetActor();
			bIsTargetLocked = true;
			bUseControllerRotationYaw = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
	}

		
	

	
}

void AAI_ProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAI_ProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}



void AAI_ProjectCharacter::MoveForward(float Value)
{
	if (FMath::IsNearlyEqual(Value, 0))
	{
		bIsMovingForward = false;
	}
	else
	{
		bIsMovingForward = true;
	}

	if(bIsTargetLocked)
	{
		if ((Controller != nullptr) && (Value != 0.0f))
		{
			const FVector Direction = CalculateMovementYaw(Value, true, true);
			AddMovementInput(Direction, Value);
		}
	}
	else
	{
		if (Value != 0.0f)
		{
			const FVector Direction = CalculateMovementYaw(Value, true, false);
			AddMovementInput(Direction.GetSafeNormal(), FMath::Abs(Value));
		}
	}
	


	
}

void AAI_ProjectCharacter::MoveRight(float Value)
{
	if (FMath::IsNearlyEqual(Value, 0))
	{
		bIsMovingRight = false;
	}
	else
	{
		bIsMovingRight = true;
	}

	
	if(bIsTargetLocked)
	{
		if ( (Controller != nullptr) && (Value != 0.0f) )
		{
			const FVector Direction = CalculateMovementYaw(Value, false, true);
			AddMovementInput(Direction, Value);
		}
	}
	else
	{
		if (Value)
		{
			const FVector Direction = CalculateMovementYaw(Value, false, false);
			AddMovementInput(Direction.GetSafeNormal(), FMath::Abs(Value));
		}
	}
}

FVector AAI_ProjectCharacter::CalculateMovementYaw(float AxisValue, bool IsMovingForward, bool IsLockedOnTarget)
{
	FVector Direction;
	if (IsLockedOnTarget)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);
		if (IsMovingForward)
		{
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X).GetSafeNormal();
		}
		else
		{
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y).GetSafeNormal();

		}

	}
	else
	{
		const FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);
		YawRotation.Yaw = AxisValue > 0 ? YawRotation.Yaw : YawRotation.Yaw - 180;
		FVector TargetDirection;
		if (IsMovingForward)
		{
			TargetDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X).GetSafeNormal();
		}
		else
		{
			TargetDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y).GetSafeNormal();
		}
		const FRotator PlayerRotation = GetActorRotation();
		const FRotator PlayerYawRotation = FRotator(0, PlayerRotation.Yaw, 0);
		const FVector CurrentDirection = FRotationMatrix(PlayerYawRotation).GetUnitAxis(EAxis::X).GetSafeNormal();
		
		Direction = FMath::VInterpTo(CurrentDirection, TargetDirection, GetWorld()->GetDeltaSeconds(), MovementLerpSpeed);

	}


	return Direction;
}

void AAI_ProjectCharacter::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{

	if(IsDead())
	{
		return;
	}

	//When the damage is done by getting hooked
	if (Cast<UDamageType_Hook>(DamageType) != nullptr)
	{
		if(DamagedActor && DamageCauser && GetController())
		{
			//If Damaged actor is jumping Hook attack is canceled.
			AAI_ProjectCharacter* DamagedCharacter = Cast<AAI_ProjectCharacter>(DamagedActor);
			if(DamagedCharacter)
			{
				if(DamagedCharacter->GetMovementComponent())
				{
					if (DamagedCharacter->GetMovementComponent()->IsFalling())
					{
						return;
					}
				}
			}

			//Hook particle effect on damaged character
			if (HookedEffectParticle)
			{
				if (GetMesh()->GetSocketByName(TEXT("Impact")))
				{
					
					HookedEffectParticleSystemComponent = UGameplayStatics::SpawnEmitterAttached(HookedEffectParticle, GetMesh(), TEXT("Impact"));
				}
			}

		
			//End location of the damaged character
			HookSimpleMoveToGoal = DamageCauser->GetActorLocation() + DamageCauser->GetActorForwardVector() * 200;
			HookPointDamage();
			
			//DamagedActor->SetActorLocation(DamageCauser->GetActorLocation() + DamageCauser->GetActorForwardVector() * 200);
		}
		
	}
	else //Normal AttackDamage
	{

		// Find the angle which hit is occurred
		HitLocation.Z = GetActorLocation().Z;
		const FVector Direction = (HitLocation - GetActorLocation()).GetSafeNormal();
		const FVector Delta = FVector::CrossProduct(GetActorForwardVector().GetSafeNormal(), Direction.GetSafeNormal());
		const int Neg = (Delta.Z < 0 ? 1 : -1);
		float Angle = UKismetMathLibrary::DegAcos(FVector::DotProduct(GetActorForwardVector().GetSafeNormal(), Direction));
		Angle *= Neg;

		//Play hit react animation based on the hit angle
		PlayHitReactAnim(Angle);

		//Launch character in the direction of hit
		FVector VelocityDirection = -1 * Direction;
		VelocityDirection.Z = 0;
		GetCharacterMovement()->Launch(VelocityDirection * 1000);

	}

	//Manage health

	Health -= Damage;
	if(IsDead())
	{
		
		if(DeathAnimMontage)
		{
			GetMesh()->GetAnimInstance()->StopAllMontages(0.1f);

			PlayAnimMontage(DeathAnimMontage);
		}
		DisableInput(Cast<APlayerController>(GetController()));
	}
}


void AAI_ProjectCharacter::PlayHitReactAnim(float Angle)
{
	if (HitReactFront && HitReactBack && HitReactLeft && HitReactRight)
	{
		if (!bIsAttacking) // if the character is  attacking do not play hit react montages
		{
			if (FMath::Abs(Angle) < 50)
			{
				//front
				PlayAnimMontage(HitReactFront);

			}
			else if (FMath::Abs(Angle) >= 50 && FMath::Abs(Angle) < 140)
			{
				if (Angle > 0)
				{
					//left
					PlayAnimMontage(HitReactLeft);
				}
				else
				{

					//right
					PlayAnimMontage(HitReactRight);

				}
			}
			else
			{
				//back
				PlayAnimMontage(HitReactBack);
			}
		}
	}

}


void AAI_ProjectCharacter::TakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Magenta, "Radial Damage Received");

}

void AAI_ProjectCharacter::DealDamage(bool bRightWeapon,float DamageAmount)
{
	FHitResult HitResult;
	if (bRightWeapon)
	{
		const bool IsHit = WeaponTrace(HitResult, TEXT("weapon_r_bottom"), TEXT("weapon_r_top"));
		if(IsHit)
		{
			ProcessHitAndApplyPointDamage(HitResult, DamageAmount);
		}
	}
	else
	{
		const bool IsHit = WeaponTrace(HitResult, TEXT("weapon_l_bottom"), TEXT("weapon_l_top"));
		if (IsHit)
		{
			ProcessHitAndApplyPointDamage(HitResult, DamageAmount);
		}
	}
	
}


bool AAI_ProjectCharacter::WeaponTrace(FHitResult& HitOut, const FName WeaponBottomSocket, const FName WeaponTopSocket, float Radius)
{

	if (!bIsAttackEnded)
	{
		return false;
	}

	//NOTE: if damage amount is set to zero then apply point damage won't be called.

	const float SphereRadius = Radius;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	const FVector WeaponBottomLocation = GetMesh()->GetSocketLocation(WeaponBottomSocket);
	const FVector WeaponTopLocation = GetMesh()->GetSocketLocation(WeaponTopSocket);

	const bool bWeaponHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), WeaponBottomLocation, WeaponTopLocation,
		SphereRadius, ETraceTypeQuery::TraceTypeQuery12, false, ActorsToIgnore
		, EDrawDebugTrace::ForDuration, HitOut, true);

	bIsAttackEnded = !bWeaponHit;

	return bWeaponHit;

}

void AAI_ProjectCharacter::ProcessHitAndApplyPointDamage(const FHitResult& Hit, float DamageAmount)
{

	if (ImpactEffectParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffectParticle, Hit.Location - Hit.ImpactNormal * 10, Hit.GetActor()->GetActorRotation());
	}

	const TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass();

	float i = UGameplayStatics::ApplyPointDamage(Hit.GetActor(), DamageAmount,
		(Hit.Normal).GetSafeNormal(),
		Hit, GetController(), this, DamageType);
}

void AAI_ProjectCharacter::DealRadialDamage()
{

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 10.f, GetActorLocation(),
		300, UDamageType::StaticClass(), ActorsToIgnore, this, GetController(),
		false, ECollisionChannel::ECC_Camera);
}




void AAI_ProjectCharacter::AttackMontageCompletedEvent(UAnimMontage* animMontage, bool bInterrupted)
{	
	if (bIsAttackingInterrupted)
	{
		bSaveAttack = false;
		bIsAttacking = false;
		bIsAttackEnded = true;
		AttackCount = 0;
	}
	else
	{
		bIsAttackingInterrupted = true;
	}
}

void AAI_ProjectCharacter::HookPointDamage()
{

	
	if(HookedAnimMontage)
	{
		PlayAnimMontage(HookedAnimMontage,3);	
	}
	
	
	HookSimpleMoveToGoalIsCompleted = false;
	GetCharacterMovement()->MaxAcceleration = 10000.f;
	GetCharacterMovement()->MaxWalkSpeed = 10000.f;

	/*Find New damaged character rotation*/
	FRotator NewPlayerRotation = (HookSimpleMoveToGoal - GetActorLocation()).Rotation();
	NewPlayerRotation.Roll = 0;
	NewPlayerRotation.Pitch = 0;
	SetActorRotation(NewPlayerRotation);

	/* Disable Player Movement */
	DisableInput(Cast<APlayerController>(GetController()));


	/* Timer for calling Hook End Event this is when damaged character stun time is over */
	GetWorld()->GetTimerManager().SetTimer(AttackEndedTimerHandle,
		FTimerDelegate::CreateLambda([this] {HookMoveToLocationCompletedEvent(); }),
		HookStunnedTime, false);


	/* Timer for playing damaged character stun animation
	 * resets global time speed
	 * Stops hooked particle on damaged character
	 */
	FTimerHandle StunTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StunTimerHandle,
		FTimerDelegate::CreateLambda([this] {
			if (HookStunnedAnimMontage)
			{
				GetMesh()->GetAnimInstance()->StopAllMontages(0.1f);
				PlayAnimMontage(HookStunnedAnimMontage);

			}
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);

			HookedEffectParticleSystemComponent->SetActive(false);
	}),
		0.5f, false);

	
	// Slow motion
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.4);
	
	// Move the damaged character to founded goal location
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), HookSimpleMoveToGoal);

}

void AAI_ProjectCharacter::HookMoveToLocationCompletedEvent()
{
	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(HookedAnimMontage))
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.1, HookedAnimMontage);
	}
	HookSimpleMoveToGoalIsCompleted = true;
	GetCharacterMovement()->MaxAcceleration = 2048.f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), GetActorLocation());

	EnableInput(Cast<APlayerController>(GetController()));

}



