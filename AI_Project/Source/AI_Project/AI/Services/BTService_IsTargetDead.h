// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_IsTargetDead.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API UBTService_IsTargetDead : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_IsTargetDead();
	UPROPERTY(EditAnywhere, Category = Blackboard)
		FBlackboardKeySelector TargetActor;
	UPROPERTY(EditAnywhere, Category = Blackboard)
		FBlackboardKeySelector IsTargetDead;
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
