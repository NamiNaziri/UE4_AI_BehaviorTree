// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_LightAttack.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API UBTTask_LightAttack : public UBTTaskNode
{
	GENERATED_BODY()
		UBTTask_LightAttack();

	UPROPERTY(EditAnywhere, Category = Blackboard)
		FBlackboardKeySelector AttackCount;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
