// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_RandomLocAroundTarget.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API UBTService_RandomLocAroundTarget : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_RandomLocAroundTarget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Radius = 100.f;

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector Target;

	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector TargetDestination;
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
