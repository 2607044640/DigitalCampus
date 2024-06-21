// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_SliderToControlDrawImg.h"

#include "Components/Slider.h"
#include "DigitalCampus/DigitalCampus.h"
#include "DigitalCampus/Framework/MyHUD.h"
#include "Kismet/GameplayStatics.h"


void UUMG_SliderToControlDrawImg::NativeConstruct()
{
	Super::NativeConstruct();
	Slider->OnValueChanged.AddDynamic(this, &UUMG_SliderToControlDrawImg::OnValueChanged);
}

void UUMG_SliderToControlDrawImg::OnValueChanged(float Value)
{
	AMyHUD* MyHUD = Cast<AMyHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	MyHUD->bUserTouchedSlider=true;
	MyHUD->ShowUser_APTime = static_cast<double>(Value) * static_cast<double>(DailyMinutes);
}