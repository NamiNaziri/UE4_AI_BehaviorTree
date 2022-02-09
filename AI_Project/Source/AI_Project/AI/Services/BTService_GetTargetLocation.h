// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_GetTargetLocation.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API UBTService_GetTargetLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_GetTargetLocation();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
