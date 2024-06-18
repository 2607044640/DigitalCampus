// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALCAMPUS_API AMyHUD : public AHUD
{
	virtual void DrawHUD() override;
	GENERATED_BODY()
	float GS_ThisTimeSearch;

public:
	float ShowUser_APTime;
};
