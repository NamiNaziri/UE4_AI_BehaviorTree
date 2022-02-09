// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FocusOnTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_FocusOnTarget::UBTTask_FocusOnTarget()
{
	NodeName = "FocusOnTarget";
}

EBTNodeResult::Type UBTTask_FocusOnTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if(FocusActive)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Magenta, "FF: " + Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Target.SelectedKeyName))->GetName());
		OwnerComp.GetAIOwner()->GetCharacter()->GetCharacterMovement()->bOrientRotationToMovement = false;
		OwnerComp.GetAIOwner()->GetCharacter()->bUseControllerRotationYaw = true;
		OwnerComp.GetAIOwner()->SetFocus(Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Target.SelectedKeyName)), EAIFocusPriority::Gameplay);
		
	}
	else
	{
		OwnerComp.GetAIOwner()->GetCharacter()->GetCharacterMovement()->bOrientRotationToMovement = true;
		OwnerComp.GetAIOwner()->GetCharacter()->bUseControllerRotationYaw = false;
		OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);
	}

	return EBTNodeResult::Succeeded;
}
