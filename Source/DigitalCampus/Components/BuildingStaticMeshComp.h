// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "DigitalCampus/Framework/MyDefaultPawn.h"
#include "BuildingStaticMeshComp.generated.h"

/**
 * 
 */

class UBuildingStaticMeshComp;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMouseClickedSM, UBuildingStaticMeshComp*);

UCLASS()
class DIGITALCAMPUS_API UBuildingStaticMeshComp : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	FOnMouseClickedSM OnMouseClickedBuildingStaticMesh;
	UPROPERTY()
	AMyDefaultPawn* MyDefaultPawn;
	UPROPERTY()
	ADC_Building* GetOwner_DC_Building;

protected:
	bool bIsClicked;
	int32 FaceSituation;
	FVector SavedLocation;
	//Timeline
	FTimeline SMComp_Timeline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* CurveFloat;
	UFUNCTION()
	void OnTimelineTick(float DeltaTime);
	UFUNCTION()
	void OnTimelineEndEvent();
	//Timeline
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	UBuildingStaticMeshComp();
public:
	void MaterialFadeOut();
	int32 CalcFace();
	void SMComp_StartTimeline();
	UFUNCTION()
	void StaticMeshComponentOnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
	UPROPERTY()
	UMaterialInterface* SavedMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	UMaterialInstance* FadeOutMaterial;
};
