// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DigitalCampus/Framework/MyDefaultPawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UMG_PawnInfoController.generated.h"

class UButton;
class USlider;
/**
 * 
 */
UCLASS()
class DIGITALCAMPUS_API UUMG_PawnInfoController : public UUserWidget
{
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	float BaseValue = 5;

private:
	UPROPERTY(meta=(BindWidget))
	USlider* Slider_PawnMoveSpeedController;
	UPROPERTY(meta=(BindWidget))
	USlider* Slider_PawnRotateSpeedController;

	UFUNCTION()
	void MoveValueChanged(float Value);
	UFUNCTION()
	void RotateValueChanged(float Value);

	UPROPERTY(meta=(BindWidget))
	UButton* Button_MoveResetValue;
	UPROPERTY(meta=(BindWidget))
	UButton* Button_RotateResetValue;
	
protected:
	UFUNCTION()
	void MoveResetValueOnClicked();
	UFUNCTION()
	void RotateResetValueOnClicked();
	virtual void NativeConstruct() override;

	UPROPERTY()
	AMyDefaultPawn* MyDefaultPawn;

	float RotateSpeed;
	float MoveMaxSpeed;
	UPROPERTY()
	UFloatingPawnMovement* FloatingPawnMovement;

	GENERATED_BODY()
};
