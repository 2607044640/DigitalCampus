// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_Main.generated.h"

class UUMG_PawnInfoController;
/**
 * 
 */
UCLASS()
class DIGITALCAMPUS_API UUMG_Main : public UUserWidget
{
	UPROPERTY(meta=(BindWidget))
	UUMG_PawnInfoController* PawnInfoController;
	GENERATED_BODY()
};
