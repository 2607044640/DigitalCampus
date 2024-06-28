// Fill out your copyright notice in the Description page of Project Settings.


#include "DC_Building.h"


ADC_Building::ADC_Building()
{
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
StaticMeshComponent->OnClicked
	
}

void ADC_Building::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADC_Building::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

