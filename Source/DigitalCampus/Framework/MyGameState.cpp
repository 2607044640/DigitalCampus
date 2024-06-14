// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();
	InitMap_AP();
}

void AMyGameState::InitMap_AP()
{
	Map_AP.Empty();

	Map_AP.Add(0, 0);
	Map_AP.Add(1, 2);
	Map_AP.Add(DailyMinutes - 1, 1);

	//test 1440不会被Get到
	Map_AP.Add(15, 5000.999);
	Map_AP.Add(50, 8000.999);
	Map_AP.Add(70, 3000.999);
	Map_AP.Add(100, 1000.999);
	Map_AP.Add(110, 3000.99);
	Map_AP.Add(130, 100.999);
	Map_AP.Add(120, 100.999);
	Map_AP.Add(1420, 1000.999);
	//test key不能相同
	Map_AP.KeyStableSort([](const int32& A, const int32& B) { return A > B; });
}

bool AMyGameState::GetNextKey()
{
	for (int32 i = Map_AP_Key + 1; i < DailyMinutes; ++i)
	{
		if (Map_AP.Contains(i))
		{
			Map_AP_KeyBefore = Map_AP_Key;
			Map_AP_Key = i;
			// FString TempStr = FString::Printf(TEXT("%i"), Map_AP_KeyBefore);
			// if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TempStr, true);
			// UE_LOG(LogTemp, Error, TEXT("%s"), *TempStr);
			// FString rfwe = FString::Printf(TEXT("Map_AP_Key %i"), Map_AP_Key);
			// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, rfwe, true, FVector2D(3, 3));
			// UE_LOG(LogTemp, Error, TEXT("%s"), *rfwe);
			return true;
		}
	}
	return false;
}

void AMyGameState::AddTillFullMap()
{
	float TempAPValue = Map_AP[Map_AP_Key] - Map_AP[Map_AP_KeyBefore];
	int32 TempAPKey = Map_AP_Key - Map_AP_KeyBefore;
	const float AverageValue = TempAPValue / TempAPKey;
	float Value = 0;
	for (int i = Map_AP_KeyBefore + 1; i < Map_AP_Key; ++i)
	{
		Value += AverageValue;
		Map_AP.Add(i, Value);

		FString TempStr = FString::Printf(TEXT("i  %i"), i);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TempStr, true);
		UE_LOG(LogTemp, Error, TEXT("%s"), *TempStr);
		FString rfwe = FString::Printf(TEXT("Map_AP_KeyBefore %f"), Value);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, rfwe, true, FVector2D(3, 3));
		UE_LOG(LogTemp, Error, TEXT("%s"), *rfwe);
	}
}
