// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GetTargetActorRefrence.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTService_GetTargetActorRefrence::UBTService_GetTargetActorRefrence()
{
	NodeName = TEXT("GetTargetActorReference");
}

void UBTService_GetTargetActorRefrence::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(Target.SelectedKeyName, Player);

}
