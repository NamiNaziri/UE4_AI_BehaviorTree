// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LightAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI_Project/AI_ProjectCharacter.h"
UBTTask_LightAttack::UBTTask_LightAttack()
{
	NodeName = TEXT("LightAttack");
}

EBTNodeResult::Type UBTTask_LightAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAI_ProjectCharacter* Player = Cast<AAI_ProjectCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if(Player)
	{
		Player->bIsAttackingInterrupted = false;
		Player->LightAttack();
	}
	

	OwnerComp.GetBlackboardComponent()->SetValueAsInt(AttackCount.SelectedKeyName
		, Player->AttackCount);

	
	return EBTNodeResult::Succeeded;
}
