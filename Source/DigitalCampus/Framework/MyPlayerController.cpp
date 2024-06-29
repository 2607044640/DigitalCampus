// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bEnableClickEvents = true;
	bShowMouseCursor = true;
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);
}
