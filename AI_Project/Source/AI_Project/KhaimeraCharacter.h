// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_ProjectCharacter.h"
#include "KhaimeraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API AKhaimeraCharacter : public AAI_ProjectCharacter
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
	virtual void TakePointDamage(AActor* DamagedActor, float Damage, class AController*
		InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent,
		FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser) override ;

	
	 virtual void DealDamage( bool bRightWeapon, float DamageAmount) override;
};
