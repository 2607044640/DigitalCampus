// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_PawnInfoController.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "DigitalCampus/Framework/MyDefaultPawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"

void UUMG_PawnInfoController::MoveValueChanged(float Value)
{
	FloatingPawnMovement->MaxSpeed = MoveMaxSpeed * BaseValue * Value;
}

void UUMG_PawnInfoController::RotateValueChanged(float Value)
{
	MyDefaultPawn->RotatePawnByBuildingSpeed = Value * BaseValue * RotateSpeed;
}

void UUMG_PawnInfoController::MoveResetValueOnClicked()
{
	Slider_PawnMoveSpeedController->SetValue(1 / BaseValue);
}

void UUMG_PawnInfoController::RotateResetValueOnClicked()
{
	Slider_PawnRotateSpeedController->SetValue(1 / BaseValue);
}

void UUMG_PawnInfoController::NativeConstruct()
{
	Super::NativeConstruct();

	Button_MoveResetValue->OnClicked.AddDynamic(this, &UUMG_PawnInfoController::MoveResetValueOnClicked);
	Button_RotateResetValue->OnClicked.AddDynamic(this, &UUMG_PawnInfoController::RotateResetValueOnClicked);
	Slider_PawnMoveSpeedController->OnValueChanged.AddDynamic(this, &UUMG_PawnInfoController::MoveValueChanged);
	Slider_PawnRotateSpeedController->OnValueChanged.AddDynamic(this, &UUMG_PawnInfoController::RotateValueChanged);


	MyDefaultPawn = Cast<AMyDefaultPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	RotateSpeed = MyDefaultPawn->RotatePawnByBuildingSpeed;
	FloatingPawnMovement = Cast<UFloatingPawnMovement>(MyDefaultPawn->GetMovementComponent());
	MoveMaxSpeed = FloatingPawnMovement->MaxSpeed;

	MoveResetValueOnClicked();
	RotateResetValueOnClicked();
}
