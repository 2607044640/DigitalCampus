// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/IHttpRequest.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALCAMPUS_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	   
public:
	virtual void BeginPlay() override;

private:
	void FetchMonitorData();
	void OnMonitorDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
};
