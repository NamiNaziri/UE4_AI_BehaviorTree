// Fill out your copyright notice in the Description page of Project Settings.


#include "KhaimeraCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Kismet/GameplayStatics.h"
void AKhaimeraCharacter::BeginPlay()
{
	Super::BeginPlay();
	//OnTakePointDamage.AddDynamic(this, &AKhaimeraCharacter::TakePointDamage);
}


void AKhaimeraCharacter::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
                                         FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
                                         const UDamageType* DamageType, AActor* DamageCauser)
{
	Super::TakePointDamage(DamagedActor, Damage, InstigatedBy, HitLocation, FHitComponent, BoneName, ShotFromDirection, DamageType, DamageCauser);
	//UE_LOG(LogTemp, Warning, TEXT("FFF"));
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Magenta, "BoneName: " + BoneName.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Magenta, "DamagedActor: " + DamagedActor->GetName());
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Magenta, "BoneName: " + BoneName.ToString());
}

void AKhaimeraCharacter::DealDamage( bool bRightWeapon, float DamageAmount)
{

	Super::DealDamage( bRightWeapon, DamageAmount);
}


