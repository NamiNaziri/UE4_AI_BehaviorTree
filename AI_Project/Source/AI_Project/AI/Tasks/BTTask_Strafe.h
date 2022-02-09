// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Strafe.generated.h"

/**
 * This is for animation logic
 * Strafe blend space or simple walk blend space
 */
UCLASS()
class AI_PROJECT_API UBTTask_Strafe : public UBTTaskNode
{
	GENERATED_BODY()
		UBTTask_Strafe();

	UPROPERTY(EditAnywhere)
		bool bStrafeEnable = false;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
