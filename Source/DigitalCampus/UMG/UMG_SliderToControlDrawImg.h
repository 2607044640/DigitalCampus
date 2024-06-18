// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_SliderToControlDrawImg.generated.h"

class USlider;
/**
 * 
 */
UCLASS()
class DIGITALCAMPUS_API UUMG_SliderToControlDrawImg : public UUserWidget
{
	UPROPERTY(meta=(BindWidget))
	USlider* Slider;
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnValueChanged(float Value);
	GENERATED_BODY()
};
