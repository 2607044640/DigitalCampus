// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DigitalCampus/Framework/MyDefaultPawn.h"
#include "GameFramework/Actor.h"
#include "DC_Building.generated.h"

class UBuildingStaticMeshComp;
class UWidgetComponent;

USTRUCT()
struct FBuildingInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category=JFSetting)
	int32 BuildingCount = 1;
	UPROPERTY(EditAnywhere, Category=JFSetting)
	float BuildingHeight = -1;
	UPROPERTY(EditAnywhere, Category=JFSetting)
	UStaticMesh* StaticMeshToAdd;
};

UCLASS()
class DIGITALCAMPUS_API ADC_Building : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	float LengthOfSplitBuildingOut = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	FVector BuildingRelativeDistanceFromPlayer;
		
	//todo
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	// float SplitBuildingHeight;

	UPROPERTY(EditAnywhere, Category=JFSetting)
	TArray<FBuildingInfo> BuildingInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	float DefaultBuildingHeight = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	double UMGHeight = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	float ScaleOfMainWidgetComp = 1;

	UFUNCTION(BlueprintCallable)
	void JFAddWidget(TSubclassOf<UUserWidget> InWidgetClass, FVector2D PosToAdd = FVector2D(0, 500));


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	TArray<UBuildingStaticMeshComp*> AddedStaticMeshComponents;

public:
	UPROPERTY()
	AMyDefaultPawn* MyDefaultPawn;
	UFUNCTION()
	void ViewBuildingButtonOnClicked();
	UFUNCTION()
	void OnMouseClickedBuildingStaticMesh(UBuildingStaticMeshComp* BuildingStaticMeshComp);

protected:
	UPROPERTY()
	TSubclassOf<UUserWidget> WBP_Building;
	virtual void OnConstruction(const FTransform& Transform) override;
	ADC_Building();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	UWidgetComponent* AddedWidgetComponent;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	// UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	UWidgetComponent* BuildingMainWidgetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	USceneComponent* SceneComponent;
};
