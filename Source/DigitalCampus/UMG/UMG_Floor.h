// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_Floor.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class DIGITALCAMPUS_API UUMG_Floor : public UUserWidget
{
	UPROPERTY(meta=(BindWidget))
	UButton* ViewBuilding;
	GENERATED_BODY()
};
