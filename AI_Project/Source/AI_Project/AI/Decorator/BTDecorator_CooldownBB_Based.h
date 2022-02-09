// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Decorators/BTDecorator_Cooldown.h"
#include "BTDecorator_CooldownBB_Based.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API UBTDecorator_CooldownBB_Based : public UBTDecorator_Cooldown
{
	GENERATED_UCLASS_BODY()
		//UBTDecorator_CooldownBB_Based(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Category = Decorator, EditAnywhere)
		int TargetNumber = 0;
	UPROPERTY(Category = Decorator, EditAnywhere)
		struct FBlackboardKeySelector Target;


protected:
	virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) override;
	
};
