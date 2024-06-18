// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();
	InitApBaseValue();
	InitMap_AP();
}

void AMyGameState::InitApBaseValue()
{
	Map_AP_ThisKey = 0;
	Map_AP_NextKey = 0;
	Map_AP_KeyTemp = 0;
	APValue = 0;
}

void AMyGameState::InitMap_AP()
{
	Map_AP.Empty();

	Map_AP.Add(0, 0);
	Map_AP.Add(1, 2);

	//test 1440不会被Get到
	Map_AP.Add(15, 5000.999);
	Map_AP.Add(50, 8000.999);
	Map_AP.Add(70, 3000.999);
	Map_AP.Add(100, 1000.999);
	Map_AP.Add(110, 3000.99);
	Map_AP.Add(130, 100.999);
	Map_AP.Add(120, 100.999);
	Map_AP.Add(1420, 1000.999);
	Map_AP.Add(1430, 100);
	Map_AP.Add(DailyMinutes - 2, 0);
	Map_AP.Add(DailyMinutes - 1, 1);
	Map_AP.Add(DailyMinutes, 2);
	//test key不能相同
	Map_AP.KeyStableSort([](const int32& A, const int32& B) { return A > B; });

	GetNextKey();
	GetNextKey();
	AddTillFullMap();

	Map_AP.KeyStableSort([](const int32& A, const int32& B) { return A > B; });
}

int32 AMyGameState::GetNextKey()
{
	for (int32 i = Map_AP_NextKey + 1; i < DailyMinutes; ++i)
	{
		if (Map_AP.Contains(i))
		{
			Map_AP_ThisKey = Map_AP_NextKey;
			Map_AP_KeyTemp = Map_AP_ThisKey;
			Map_AP_NextKey = i;
			return Map_AP_NextKey;
		}
	}
	return -1;
}

void AMyGameState::AddTillFullMap()
{
	for (int i = 0; i < DailyMinutes; ++i)
	{
		double TempAPValue = static_cast<double>(Map_AP[Map_AP_NextKey]) - static_cast<double>(Map_AP[
			Map_AP_ThisKey]);
		double TempDevValue = TempAPValue / static_cast<double>(Map_AP_NextKey - Map_AP_ThisKey);

		APValue += TempDevValue;

		if (Map_AP_NextKey == ++Map_AP_KeyTemp)
		{
			GetNextKey();
			Map_AP.KeyStableSort([](const int32& A, const int32& B) { return A > B; });
		}
		Map_AP.Add(Map_AP_KeyTemp, APValue < 0 ? 0 : APValue);
	}
}

float AMyGameState::GetMaxValueOfAP()
{
	float Map_APMaxValue = TNumericLimits<float>::Lowest();
	for (TTuple<signed int, float>
	     AP : Map_AP)
	{
		if (AP.Value > Map_APMaxValue)
		{
			Map_APMaxValue = AP.Value;
		}
	}
	return  Map_APMaxValue;
}
