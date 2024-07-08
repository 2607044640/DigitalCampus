// This project is Digital Campus. If you use this project maliciously, you will be held accountable according to law. CopyRight_Zonghan Luo

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "DC_Car.generated.h"

UCLASS()
class DIGITALCAMPUS_API ADC_Car : public AActor
{
	GENERATED_BODY()

public:
	ADC_Car();

protected:
	virtual void BeginPlay() override;

	//Timeline
	FTimeline MyTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* CurveFloat;
	UFUNCTION()
	void OnTimelineTick(float DeltaTime);
	UFUNCTION()
	void OnTimelineEndEvent();
	//Timeline
	FVector SavedLocation;
	
	float DistanceToGetOut=2000;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JFSetting)
	UStaticMeshComponent* StaticMeshComponent;
	virtual void Tick(float DeltaTime) override;
};
