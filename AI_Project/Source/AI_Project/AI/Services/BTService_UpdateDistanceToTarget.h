// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateDistanceToTarget.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API UBTService_UpdateDistanceToTarget : public UBTService
{
	GENERATED_BODY()


public:
	UBTService_UpdateDistanceToTarget();


	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector SelfActor;
	
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector TargetActor;

	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector DistanceToTarget;



protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};

