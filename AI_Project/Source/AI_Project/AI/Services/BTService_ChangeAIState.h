// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AI_Project/RiktorCharacter.h"
#include "BTService_ChangeAIState.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API UBTService_ChangeAIState : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_ChangeAIState();


	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector State;
	
	UPROPERTY(EditAnywhere, Category = Blackboard)
		AIStates NewState;

	

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
