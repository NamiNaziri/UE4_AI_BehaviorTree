// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_HookAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI_Project/RiktorCharacter.h"
UBTTask_HookAttack::UBTTask_HookAttack()
{
	NodeName = "HookAttack";
}

EBTNodeResult::Type UBTTask_HookAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	const AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActor.SelectedKeyName));

	
	ARiktorCharacter* Player = Cast<ARiktorCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if(!Target)
	{
		return EBTNodeResult::Failed;
	}
	const bool Succeed =  Player->HookAttack(Target->GetActorLocation());
	 
	return Succeed ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
