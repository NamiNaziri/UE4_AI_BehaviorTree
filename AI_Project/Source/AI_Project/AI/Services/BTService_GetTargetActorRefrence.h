// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GetTargetActorRefrence.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API UBTService_GetTargetActorRefrence : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_GetTargetActorRefrence();


	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector Target;


protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
