// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Strafe.h"

#include "AI_Project/AI_ProjectCharacter.h"
#include "AIController.h"

UBTTask_Strafe::UBTTask_Strafe()
{
	NodeName = TEXT("Strafe");
}

EBTNodeResult::Type UBTTask_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAI_ProjectCharacter* Player = Cast<AAI_ProjectCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	Player->bIsTargetLocked = bStrafeEnable;
	return EBTNodeResult::Succeeded;
}
