// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_Building.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class DIGITALCAMPUS_API UUMG_Building : public UUserWidget
{
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta=(BindWidget))
	UButton* Button_Building;
	GENERATED_BODY()
};
