// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI_ProjectCharacter.generated.h"

UCLASS(config=Game)
class AAI_ProjectCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	AAI_ProjectCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	virtual void MoveForward(float Value);

	/** Called for side to side input */
	virtual void MoveRight(float Value);

	UFUNCTION(BlueprintCallable)
	virtual void TargetLock();

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	virtual void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	virtual void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UFUNCTION(BlueprintCallable)
		void PlayLightAttack();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE virtual  class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE virtual  class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


public:
	UFUNCTION(BlueprintPure)
	virtual  bool IsDead() const;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Attack")
	UAnimMontage* JumpAttackMontage;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		TArray<UAnimMontage*> LightMeleeAttacks;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		UAnimMontage* HitReactLeft;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		UAnimMontage* HitReactRight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		UAnimMontage* HitReactFront;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		UAnimMontage* HitReactBack;

	UPROPERTY(BlueprintReadWrite)
		bool bSaveAttack;
	UPROPERTY(BlueprintReadWrite)
		bool bIsAttacking;
	UPROPERTY(BlueprintReadWrite)
		bool bIsTargetLocked;
	UPROPERTY(BlueprintReadWrite)
		bool bIsAttackEnded;
	//this get true when animation is going to be played and false if save notify event happens.
	UPROPERTY(BlueprintReadWrite)
		bool bIsAttackingInterrupted = false;
	UPROPERTY(BlueprintReadWrite)
		int AttackCount;

	UPROPERTY()
		FTimerHandle AttackEndedTimerHandle;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TargetLock")
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectType;

	UPROPERTY(BlueprintReadWrite)
		AActor* LockedOnTarget;

	UPROPERTY(EditAnywhere, Category = "CustomMovement", meta = (AllowPrivateAccess = "true"))
		float MovementLerpSpeed = 5;

	UPROPERTY()
		bool bIsMovingForward;
	UPROPERTY()
		bool bIsMovingRight;

	UPROPERTY()
		bool HookSimpleMoveToGoalIsCompleted;
	UPROPERTY()
		FVector HookSimpleMoveToGoal;

	UPROPERTY(EditDefaultsOnly)
		float MaxHealth = 100;
	UPROPERTY(VisibleAnywhere)
		float Health = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hook")
		float HookStunnedTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hook")
		UAnimMontage* HookStunnedAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hook")
		UAnimMontage* HookedAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hook")
		UParticleSystem* HookedEffectParticle;
	UPROPERTY()
		UParticleSystemComponent* HookedEffectParticleSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		UParticleSystem* ImpactEffectParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death")
		UAnimMontage* DeathAnimMontage;

	UFUNCTION(BlueprintCallable)
		void LightAttack();

	UFUNCTION(BlueprintCallable)
		void HeavyAttack();

	
	UFUNCTION(BlueprintCallable)
		FVector CalculateMovementYaw(float AxisValue, bool IsMovingForward, bool IsLockedOnTarget);

	UFUNCTION()
		virtual void TakePointDamage(AActor* DamagedActor, float Damage, class AController*
			InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent,
			FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
		virtual void TakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType*
			DamageType, FVector Origin, FHitResult
			HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

	
	UFUNCTION(BlueprintCallable)
		 virtual void DealDamage(bool bRightWeapon, float DamageAmount);
	
	UFUNCTION(BlueprintCallable)
		virtual void DealRadialDamage();

	UFUNCTION()
		void PlayHitReactAnim(float Angle);
	
	UFUNCTION()
		bool WeaponTrace(FHitResult& HitOut, const FName WeaponBottomSocket, const FName WeaponTopSocket,float Radius = 15.f);

	UFUNCTION()
		void ProcessHitAndApplyPointDamage(const FHitResult& Hit, float DamageAmount);

	UFUNCTION()
		virtual void HookPointDamage();
	
	UFUNCTION()
		virtual void HookMoveToLocationCompletedEvent();

	UFUNCTION()
		void AttackMontageCompletedEvent(UAnimMontage* animMontage, bool bInterrupted);
};

