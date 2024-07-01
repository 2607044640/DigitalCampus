// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DC_Building.generated.h"

class UWidgetComponent;

UCLASS()
class DIGITALCAMPUS_API ADC_Building : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	float BuildingHeight=0;
	UFUNCTION(BlueprintCallable)
	void StaticMeshComponentOnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
	ADC_Building();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	UWidgetComponent* WidgetComponent;

	bool state;
	bool FlipFlop() ;
};
