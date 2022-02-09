// Fill out your copyright notice in the Description page of Project Settings.


#include "RiktorCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AI_Project/Gameplay/DamageType/DamageType_Hook.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI_Project/AI_ProjectCharacter.h"
#include "DrawDebugHelpers.h"
void ARiktorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	HookAttackLogic();


	
	
}

bool ARiktorCharacter::HookAttack(FVector TargetLocation)
{
	bHookAttack = false;
	// Check if the attack is possible
	FHitResult HitResult;

	const float SphereRadius = 15.f;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	const bool bWeaponHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), TargetLocation,
		SphereRadius, ETraceTypeQuery::TraceTypeQuery12, false, ActorsToIgnore
		, EDrawDebugTrace::ForDuration, HitResult, true);

	if(Cast<AAI_ProjectCharacter>(HitResult.Actor))
	{
		bHookAttack = true;
	}
	
	return bHookAttack;
	
	
}

void ARiktorCharacter::ShockingPunch()
{
}


void ARiktorCharacter::BeginPlay()
{
	Super::BeginPlay();
	//OnTakePointDamage.AddDynamic(this, &ARiktorCharacter::TakePointDamage);
}

void ARiktorCharacter::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
                                       FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
                                       const UDamageType* DamageType, AActor* DamageCauser)
{
	
	Super::TakePointDamage(DamagedActor, Damage, InstigatedBy, HitLocation, FHitComponent, BoneName, ShotFromDirection, DamageType, DamageCauser);
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Magenta, "DamagedActor: " + DamagedActor->GetName());
	//GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Magenta, "BoneName: " + BoneName.ToString());

}

 void ARiktorCharacter::DealDamage(bool bRightWeapon, float DamageAmount)
{
	 
	 if(bHookAttack)
	 {
		 FHitResult HitResult;
		 const bool IsHit = WeaponTrace(HitResult, TEXT("hook_bottom"), TEXT("hook_top"),7.f);
		 if (IsHit)
		 {
			
			 HookDamage(HitResult, DamageAmount);
			 //manage Hook
		 }
	 }
	 else
	 {
		 Super::DealDamage(bRightWeapon, DamageAmount);
	 }
	
}

 void ARiktorCharacter::HookAttackLogic()
 {


	 
	 if (bHookAttack)
	 {
		 if (bHookAnimThrow)
		 {
			 if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(AnimMontageHookThrow))
			 {
				 
				 PlayAnimMontage(AnimMontageHookThrow);
			 }
		 }
		 else
		 {
			 if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(AnimMontageHookThrow))
			 {
				
				 StopAnimMontage(AnimMontageHookThrow);
			 }


			 if (bHookAnimPull)
			 {
				 if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(AnimMontageHookPull))
				 {

					
				 	
					 //float FullLength = GetMesh()->GetAnimInstance()->GetCurrentActiveMontage()->GetSectionLength(CurrentSection);
					 const float Length = AnimMontageHookPull->GetPlayLength();
					 PlayAnimMontage(AnimMontageHookPull);
					 GetMesh()->GetAnimInstance()->Montage_SetPosition(AnimMontageHookPull, (1-AnimMontageHookPullStartPosition) * Length);
					 
					
				 }
			 }
			 else
			 {
				 if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(AnimMontageHookPull))
				 {
					 bForcePull = false;
					 StopAnimMontage(AnimMontageHookPull);
				 }
			 }
		 }

	 }
 }

 void ARiktorCharacter::HookDamage(const FHitResult& Hit, float DamageAmount)
 {
	const TSubclassOf<UDamageType_Hook> HookDamage = UDamageType_Hook::StaticClass();
	
	float i = UGameplayStatics::ApplyPointDamage(Hit.GetActor(), DamageAmount,
	 (Hit.Normal).GetSafeNormal(),
	 Hit, GetController(), this, HookDamage);

	if (!GetMesh()->GetAnimInstance()->GetCurrentActiveMontage())
		return;

	
	const int32 CurrentSection = GetMesh()->GetAnimInstance()->GetCurrentActiveMontage()->GetSectionIndex("Default");
	const float FullLength = GetMesh()->GetAnimInstance()->GetCurrentActiveMontage()->GetSectionLength(CurrentSection);
	const float CurrentPosition = GetMesh()->GetAnimInstance()->Montage_GetPosition(AnimMontageHookThrow);
	///Animation processing
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Magenta, "Montage_POs:  " +
	FString::SanitizeFloat(CurrentPosition/FullLength ));

	AnimMontageHookPullStartPosition = (CurrentPosition / FullLength);

	//GetMesh()->GetAnimInstance()->Montage_Pause(AnimMontageHookThrow);

	bForcePull = true;
	bHookAnimThrow = false;
	bHookAnimPull = true;
	
		 

 }