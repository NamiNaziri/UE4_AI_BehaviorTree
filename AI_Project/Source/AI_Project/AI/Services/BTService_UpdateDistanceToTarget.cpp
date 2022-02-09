// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateDistanceToTarget.h"

#include "BehaviorTree/BlackboardComponent.h"


UBTService_UpdateDistanceToTarget::UBTService_UpdateDistanceToTarget()
{
	NodeName = "UpdateDistanceToTarget";
}

void UBTService_UpdateDistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	const AActor* Self = Cast<AActor>( OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfActor.SelectedKeyName));
	const AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActor.SelectedKeyName));
	if(!Self || !Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTService_UpdateDistanceToTarget: No ActorFound"));
		return;
	}
	const float Dis = FVector::Dist(Self->GetActorLocation(), Target->GetActorLocation());
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(DistanceToTarget.SelectedKeyName,Dis);
}

