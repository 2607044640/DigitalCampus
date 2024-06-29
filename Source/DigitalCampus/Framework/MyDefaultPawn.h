// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "MyDefaultPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
DECLARE_MULTICAST_DELEGATE(FOnMouseClicked);
class ADC_Building;


UCLASS()
class DIGITALCAMPUS_API AMyDefaultPawn : public ADefaultPawn
{
	GENERATED_BODY()
	bool RightMouseDown;
	bool LeftMouseDown;
	FVector SavedTempLocation;
	bool TickRotationbyBuilding;
	void OnMouseClickedFunc();

public:
	//Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	UCameraComponent* CameraComponent;;

public:
	AMyDefaultPawn();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	float RotatePawnByBuildingSpeed=10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	float RotatePawnByBuildingRate=0.1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=jfsetting)
	ADC_Building* Building;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=jfsetting)
	UCurveFloat* CurveFloat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=jfsetting)
	double RandomMin = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=jfsetting)
	double RandomMax = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=jfsetting)
	float PlayerRate = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=jfsetting)
	float DistanceBetweenBuilding;

protected:
	FVector NormalizeLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	TSubclassOf<ADC_Building> BuildingFromPlayer;
	FHitResult LineTraceSingleForObjects_CameraShoot(float Distance,
	                                                 TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes);
	virtual void BeginPlay() override;

public:
	void TimelineStart(ADC_Building* InBuilding);
	virtual void Tick(float DeltaTime) override;

	void LeftMouse();
	FOnMouseClicked OnMouseClicked;
	void RightMouse();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//Timeline
	FTimeline MyTimeline;

protected:
	UFUNCTION()
	void OnTimelineTick(float DeltaTime);
	void RotatePawnByBuilding();
	UFUNCTION()
	void OnTimelineEndEvent();
	void TimelineBegin();
	//Timeline
};
