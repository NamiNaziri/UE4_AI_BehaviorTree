// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FocusOnTarget.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API UBTTask_FocusOnTarget : public UBTTaskNode
{
	GENERATED_BODY()
		UBTTask_FocusOnTarget();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FocusActive = true;

	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector Target;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
