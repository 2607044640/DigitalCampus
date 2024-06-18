// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALCAMPUS_API AMyGameState : public AGameState
{
public:
	UPROPERTY(BlueprintReadOnly, Category=Setting)
	TMap<int32, float> Map_AP;
	double APValue;
	int32 Map_AP_KeyTemp;

	float GetMaxValueOfAP();
protected:
	int32 Map_AP_NextKey;
	int32 Map_AP_ThisKey;

	virtual void BeginPlay() override;
	void InitApBaseValue();
	void InitMap_AP();

	UFUNCTION(BlueprintCallable)
	int32 GetNextKey();
	UFUNCTION(BlueprintCallable)
	void AddTillFullMap();
	GENERATED_BODY()
};
