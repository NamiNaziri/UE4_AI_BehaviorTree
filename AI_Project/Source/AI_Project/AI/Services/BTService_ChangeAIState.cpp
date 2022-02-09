// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeAIState.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTService_ChangeAIState::UBTService_ChangeAIState()
{
	NodeName = "ChangeAIState";
}

void UBTService_ChangeAIState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(State.SelectedKeyName, (uint8)NewState);
}
