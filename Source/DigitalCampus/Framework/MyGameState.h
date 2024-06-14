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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	TMap<int32, float> Map_AP;

	const int32 DailyMinutes = 1440;

protected:
	int32 Map_AP_Key = 0;
	int32 Map_AP_KeyBefore = 0;
	
	virtual void BeginPlay() override;
	void InitMap_AP();

	UFUNCTION(BlueprintCallable)
	bool GetNextKey();
	UFUNCTION(BlueprintCallable)
	void AddTillFullMap();
	GENERATED_BODY()
};
