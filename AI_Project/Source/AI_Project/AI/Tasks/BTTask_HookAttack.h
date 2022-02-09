// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_HookAttack.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API UBTTask_HookAttack : public UBTTaskNode
{
	GENERATED_BODY()
		UBTTask_HookAttack();

	UPROPERTY(EditAnywhere, Category = Blackboard)
		FBlackboardKeySelector TargetActor;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
