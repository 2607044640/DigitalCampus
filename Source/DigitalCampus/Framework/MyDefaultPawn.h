// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "MyDefaultPawn.generated.h"

class UBuildingStaticMeshComp;
class UWidgetInteractionComponent;
class UCameraComponent;
class USpringArmComponent;
class ADC_Building;


DECLARE_MULTICAST_DELEGATE(FOnMouseClicked);
UCLASS()
class DIGITALCAMPUS_API AMyDefaultPawn : public ADefaultPawn
{
	GENERATED_BODY()
	
	virtual void AddControllerYawInput(float Val) override;
	virtual void AddControllerPitchInput(float Val) override;
	bool RightMouseDown;
	bool LeftMouseDown;
	FVector SavedTempLocation;
	bool TickRotationbyBuilding;
	bool TickRotationbyBuildingSMComp;
	void OnMouseClickedFunc();

public:
	FOnMouseClicked OnRightMouseClick_Canceled;

	void OnMouseClickedBuildingStaticMesh(UBuildingStaticMeshComp* StaticMeshComponent);

	UFUNCTION(BlueprintCallable)
	void TimelineStart(ADC_Building* InBuilding);

public:
	//Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	UWidgetInteractionComponent* WidgetInteractionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	UCameraComponent* CameraComponent;

public:
	AMyDefaultPawn();
	void SetRotationByBuilding();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	float RotatePawnByBuildingSpeed = 0.01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=jfsetting)
	UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=jfsetting)
	float PlayerRate = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=jfsetting)
	float DistanceBetweenBuilding = 700;

protected:
	FVector NormalizeLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	TSubclassOf<ADC_Building> BuildingFromPlayer;
	void SetTransformWhenViewBuilding();
	virtual void BeginPlay() override;


	virtual void Tick(float DeltaTime) override;

	void LeftMousePressed();
	void RightMouse();
	void LeftMouseReleased();
	void RightMouseReleased();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY()
	UBuildingStaticMeshComp* BuildingStaticMeshComponent;
	UPROPERTY()
	ADC_Building* Building;

public:
	//Timeline
	FTimeline MyPawnTimeline;

protected:
	UFUNCTION()
	void OnTimelineTick(float DeltaTime);
	UFUNCTION()
	void OnTimelineEndEvent();
	void TimelineBegin();
	//Timeline
};
