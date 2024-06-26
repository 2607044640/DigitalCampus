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
	float LengthY = 100.f;
	float LengthX = 200.f;
	AMyGameState* MyGameState = Cast<AMyGameState>(UGameplayStatics::GetGameState(GetWorld()));
	const float EveryLengthY = static_cast<double>(LengthY) / static_cast<double>(MyGameState->GetMaxValueOfAP());
	const float EveryLengthX = static_cast<double>(LengthX) / static_cast<double>(DailyMinutes);

	for (int i = 0; i < DailyMinutes; ++i)
	{
		float StartX = DrawLengthX;
		float StartY = MyGameState->Map_AP.Contains(i)
			               ? EveryLengthY * MyGameState->Map_AP[i]
			               : 0;
		float EndX = DrawLengthX + EveryLengthX;
		float EndY = MyGameState->Map_AP.Contains(i + 1)
			             ? EveryLengthY * MyGameState->Map_AP[i + 1]
			             : 0;
		DrawLengthX += EveryLengthX;
		FLinearColor DrawLineLinearColor(.1f, .50f, .99f, 1.0f);
		DrawLine(EdgeX + ScaleX * (StartX), EdgeY + ScaleY * (LengthY - StartY),
		         EdgeX + ScaleX * (EndX), EdgeY + ScaleY * (LengthY - EndY),
		         DrawLineLinearColor, 2);
	}
	DrawLengthX = 0;

	float ShowUserTime = ShowUser_APTime * EveryLengthX;

	float StartX = ShowUserTime;
	float StartY = 0;
	float EndX = ShowUserTime;
	float EndY = MyGameState->Map_AP.Contains(ShowUser_APTime)
		             ? EveryLengthY * MyGameState->Map_AP[ShowUser_APTime]
		             : 0;
	FLinearColor DrawLineLFinearColor(0.0f, 0.0f, 1.0f, 1.0f);
	float LineThickness = 2;
	DrawLine(EdgeX + ScaleX * (StartX), EdgeY + ScaleY * (LengthY - StartY),
	         EdgeX + ScaleX * (EndX), EdgeY + ScaleY * (LengthY - EndY),
	         DrawLineLFinearColor, LineThickness);


	Super::DrawHUD();
}
