// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ChangeMovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API UBTTask_ChangeMovementSpeed : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_ChangeMovementSpeed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MovementSpeed = 400.f;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
