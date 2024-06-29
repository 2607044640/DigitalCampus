// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALCAMPUS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
};
