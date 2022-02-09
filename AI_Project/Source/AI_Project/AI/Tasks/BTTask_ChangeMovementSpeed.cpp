// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeMovementSpeed.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_ChangeMovementSpeed::UBTTask_ChangeMovementSpeed()
{

	NodeName = TEXT("ChangeMovementSpeed");
}

EBTNodeResult::Type UBTTask_ChangeMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetAIOwner()->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	return EBTNodeResult::Succeeded;
}
