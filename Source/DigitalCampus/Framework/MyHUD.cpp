// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "MyGameState.h"
#include "DigitalCampus/DigitalCampus.h"
#include "DigitalCampus/Other/Paths.h"
#include "Kismet/GameplayStatics.h"
#include "DigitalCampus/UMG/UMG_Main.h"//do not delete this!

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	MakeUserWidget(MainUI, *WBP_Main_Path);
}

void AMyHUD::DrawHUD()
{
	float ScaleX = 1.5;
	float ScaleY = 1.5;

	float LengthY = 100.f;
	float LengthX = 200.f;

	float Edge = 20.f;
	AMyGameState* MyGameState = Cast<AMyGameState>(UGameplayStatics::GetGameState(GetWorld()));
	const float EveryLengthY = static_cast<double>(LengthY) / static_cast<double>(MyGameState->GetMaxValueOfAP());
	const float EveryLengthX = static_cast<double>(LengthX) / static_cast<double>(DailyMinutes);

	for (int i = 0; i < DailyMinutes; ++i)
	{
		float StartX = GS_ThisTimeSearch;
		float StartY = MyGameState->Map_AP.Contains(GS_ThisTimeSearch)
			               ? EveryLengthY * MyGameState->Map_AP[GS_ThisTimeSearch]
			               : 0;
		float EndX = (GS_ThisTimeSearch + 1);
		float EndY = MyGameState->Map_AP.Contains(GS_ThisTimeSearch + 1)
			             ? EveryLengthY * MyGameState->Map_AP[GS_ThisTimeSearch + 1]
			             : 0;
		GS_ThisTimeSearch += EveryLengthX;
		FLinearColor DrawLineLinearColor(.1f, .50f, .99f, 1.0f);
		DrawLine(Edge + ScaleX * (StartX), Edge + ScaleY * (LengthY - StartY),
		         Edge + ScaleX * (EndX), Edge + ScaleY * (LengthY - EndY),
		         DrawLineLinearColor, 2);
	}
	GS_ThisTimeSearch = 0;

	float ShowUserTime = ShowUser_APTime * EveryLengthX;

	float StartX = ShowUserTime;
	float StartY = 0;
	float EndX = ShowUserTime;
	float EndY = MyGameState->Map_AP.Contains(ShowUserTime)
		             ? EveryLengthY * MyGameState->Map_AP[ShowUserTime]
		             : 0;
	FLinearColor DrawLineLFinearColor(0.0f, 0.0f, 1.0f, 1.0f);
	float LineThickness = 2;
	DrawLine(Edge + ScaleX * (StartX), Edge + ScaleY * (LengthY - StartY),
	         Edge + ScaleX * (EndX), Edge + ScaleY * (LengthY - EndY),
	         DrawLineLFinearColor, LineThickness);


	Super::DrawHUD();
}
