// Fill out your copyright notice in the Description page of Project Settings.


#include "DC_Building.h"

#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "DigitalCampus/Framework/MyDefaultPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Slate/SGameLayerManager.h"


void ADC_Building::StaticMeshComponentOnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	AMyDefaultPawn* MyDefaultPawn = Cast<AMyDefaultPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	MyDefaultPawn->TimelineStart(this);
	FlipFlop();
}

ADC_Building::ADC_Building()
{
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->OnClicked.AddDynamic(this, &ADC_Building::StaticMeshComponentOnClicked);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
}

void ADC_Building::BeginPlay()
{
	Super::BeginPlay();
}

void ADC_Building::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (APlayerCameraManager* PlayerCamera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		FVector StartLocation = WidgetComponent->GetComponentLocation();
		FVector TargetLocation = PlayerCamera->GetCameraLocation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
		WidgetComponent->SetWorldRotation(LookAtRotation);
	}
}

bool ADC_Building::FlipFlop()
{
	WidgetComponent->SetHiddenInGame(state);
	if (state)
	{
	}
	else
	{
	}
	state = !state;
	return state;
}
