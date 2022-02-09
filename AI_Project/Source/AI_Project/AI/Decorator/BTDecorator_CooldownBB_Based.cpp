// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CooldownBB_Based.h"

UBTDecorator_CooldownBB_Based::UBTDecorator_CooldownBB_Based(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "CooldownBB_Based";
	bAllowAbortChildNodes = false;

	bNotifyTick = false;
	bNotifyDeactivation = true;
}

void UBTDecorator_CooldownBB_Based::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData,
	EBTNodeResult::Type NodeResult)
{
	FBTCooldownDecoratorMemory* DecoratorMemory = GetNodeMemory<FBTCooldownDecoratorMemory>(SearchData);


	if (SearchData.OwnerComp.GetBlackboardComponent()->GetValueAsInt(Target.SelectedKeyName) == TargetNumber)
	{
		Super::OnNodeDeactivation(SearchData, NodeResult);
		
	}
	else if (DecoratorMemory->bRequestedRestart)
	{
		DecoratorMemory->LastUseTimestamp = -FLT_MAX;
		DecoratorMemory->bRequestedRestart = false;
	}

	
}
