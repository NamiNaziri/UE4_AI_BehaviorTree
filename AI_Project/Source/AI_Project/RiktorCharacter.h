// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_ProjectCharacter.h"
#include "RiktorCharacter.generated.h"


UENUM(BlueprintType)
enum class AIStates : uint8
{
	Attack       UMETA(DisplayName = "Attack"),
	Wander        UMETA(DisplayName = "Wander"),
	Hook        UMETA(DisplayName = "Hook"),
};


/**
 * 
 */
UCLASS()
class AI_PROJECT_API ARiktorCharacter : public AAI_ProjectCharacter
{
	GENERATED_BODY()
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(BlueprintReadWrite, Category = "Hook")
		bool bHookAttack;

	UPROPERTY( BlueprintReadWrite, Category = "Hook")
		bool bHookAnimThrow;

	UPROPERTY( BlueprintReadWrite, Category = "Hook")
		bool bHookAnimPull;

	UPROPERTY(BlueprintReadWrite, Category = "Hook")
		bool bForcePull;

	UPROPERTY(BlueprintReadWrite, Category = "Hook")
		float HookYaw;
	UPROPERTY(BlueprintReadWrite, Category = "Hook")
		float HookPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hook")
		UAnimMontage* AnimMontageHookThrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hook")
		UAnimMontage* AnimMontageHookPull;
	
	

	UFUNCTION(BlueprintCallable)
		bool HookAttack(FVector TargetLocation);

	UFUNCTION(BlueprintCallable)
		void ShockingPunch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	virtual void TakePointDamage(AActor* DamagedActor, float Damage, class AController*
		InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent,
		FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser) override;

	virtual void DealDamage(bool bRightWeapon, float DamageAmount) override;
private:
	UFUNCTION()
		void HookAttackLogic();

	UFUNCTION()
		void HookDamage(const FHitResult& Hit, float DamageAmount);

	//UPROPERTY(BlueprintReadWrite, Category = "Hook")
		UPROPERTY()
	float AnimMontageHookPullStartPosition;
};
