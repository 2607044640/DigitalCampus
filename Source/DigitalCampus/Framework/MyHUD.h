// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UUMG_Main;
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
	UPROPERTY(BlueprintReadOnly)
	UUMG_Main* MainUI;

	double ShowUser_APTime;

protected:
	virtual void BeginPlay() override;

template <typename T>
void MakeUserWidget(T*& ObjectPtr, const TCHAR* Path);

};
template <typename T>
void AMyHUD::MakeUserWidget(T*& ObjectPtr, const TCHAR* Path)
{
	if (!ObjectPtr)
	{
		TSubclassOf<T> UIClass = LoadClass<T>(nullptr, Path);
		ObjectPtr = CreateWidget<T>(GetOwningPlayerController(), UIClass);
		ObjectPtr->AddToViewport();
	}
}
