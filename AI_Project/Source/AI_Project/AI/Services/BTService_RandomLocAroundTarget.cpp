// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_RandomLocAroundTarget.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_RandomLocAroundTarget::UBTService_RandomLocAroundTarget()
{
	NodeName = TEXT("FindRandomLocationAroundTarget");
}

void UBTService_RandomLocAroundTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	const float RandomAngle = FMath::RandRange(0.f, 360.f);
	const FVector Direction = FVector(FMath::Cos(FMath::DegreesToRadians(RandomAngle))
							   ,FMath::Sin(FMath::DegreesToRadians(RandomAngle))
							   ,0);
	
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Target.SelectedKeyName));
	if(!TargetActor)
		return;
	const FVector TargetLocation = TargetActor->GetActorLocation();

	DrawDebugLine(GetWorld(),
		TargetLocation,
		TargetLocation + Direction.GetSafeNormal() * Radius, 
		FColor::Blue,
		false,
		2,
		0,
		3);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(
		TargetDestination.SelectedKeyName,
		TargetLocation + Direction.GetSafeNormal() * Radius
	);
}
