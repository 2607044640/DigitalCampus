// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DC_Building.generated.h"

class UWidgetComponent;

USTRUCT()
struct FBuildingInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category=JFSetting)
	int32 BuildingCount = 1;
	UPROPERTY(EditAnywhere, Category=JFSetting)
	float BuildingHeight = -1;
};

UCLASS()
class DIGITALCAMPUS_API ADC_Building : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	float BuildingHeight = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	double UMGHeight = 300;
	UFUNCTION(BlueprintCallable)
	void JFAddWidget(TSubclassOf<UUserWidget> InWidgetClass, FVector2D PosToAdd = FVector2D(0, 500));
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	TArray<UStaticMeshComponent*> AddedStaticMeshComponents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	TArray<UStaticMesh*> StaticMeshesToAdd;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	UFUNCTION()
	void StaticMeshComponentOnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
	ADC_Building();
	virtual void BeginPlay() override;
	bool state;

public:
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	UWidgetComponent* AddedWidgetComponent;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	// UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	UWidgetComponent* BuildingMainWidgetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	USceneComponent* SceneComponent;
};
