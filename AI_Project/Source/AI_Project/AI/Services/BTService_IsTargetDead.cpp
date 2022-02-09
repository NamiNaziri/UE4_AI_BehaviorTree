// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsTargetDead.h"

#include "AIController.h"
#include "AI_Project/AI_ProjectCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_IsTargetDead::UBTService_IsTargetDead()
{
	NodeName = "Is Target Dead";
}

void UBTService_IsTargetDead::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAI_ProjectCharacter* Player = Cast<AAI_ProjectCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActor.SelectedKeyName));
	
	if (Player)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsTargetDead.SelectedKeyName, Player->IsDead());
		
	}

}
