// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_SliderToControlDrawImg.h"

#include "Components/Slider.h"

void UUMG_SliderToControlDrawImg::NativeConstruct()
{
	Super::NativeConstruct();
	Slider->OnValueChanged.AddDynamic(this, &UUMG_SliderToControlDrawImg::OnValueChanged);
}

void UUMG_SliderToControlDrawImg::OnValueChanged(float Value)
{
	
}
