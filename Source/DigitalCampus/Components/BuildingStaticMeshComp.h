// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "BuildingStaticMeshComp.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALCAMPUS_API UBuildingStaticMeshComp : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	//Timeline
	UPROPERTY()
	FTimeline MyTimeline;
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* CurveFloat;
	UFUNCTION()
	void OnTimelineTick(float DeltaTime);
	UFUNCTION()
	void OnTimelineEndEvent();
	//Timeline

	UBuildingStaticMeshComp();
	virtual void BeginPlay() override;
};
