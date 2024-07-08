// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

void AMyPlayerState::BeginPlay()
{
	Super::BeginPlay();
	FetchMonitorData();
}

void AMyPlayerState::FetchMonitorData()
{
	FString URL = TEXT("http://192.168.82.69");

	FHttpModule* Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	
	Request->OnProcessRequestComplete().BindUObject(this, &AMyPlayerState::OnMonitorDataReceived);
	Request->SetURL(URL);
	Request->SetVerb(TEXT("GET"));
	Request->ProcessRequest();
}

void AMyPlayerState::OnMonitorDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		FString ResponseContent = Response->GetContentAsString();
		
		FString TempStr = FString::Printf(TEXT("Nice"));
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TempStr, true, FVector2D(3, 3));
		UE_LOG(LogTemp, Error, TEXT("%s"), *TempStr);

		UE_LOG(LogTemp, Warning, TEXT("Monitor Data: %s"), *ResponseContent);

		// 处理和存储监控数据
	}
	else
	{
		FString TempStr = FString::Printf(TEXT("Nice2"));
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TempStr, true, FVector2D(3, 3));
		UE_LOG(LogTemp, Error, TEXT("%s"), *TempStr);
		UE_LOG(LogTemp, Error, TEXT("Failed to fetch monitor data"));
	}
}
