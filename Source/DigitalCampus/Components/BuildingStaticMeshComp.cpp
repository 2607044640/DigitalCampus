// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingStaticMeshComp.h"

#include "DigitalCampus/Building/DC_Building.h"
#include "DigitalCampus/Other/Paths.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

template <typename T>
void FindMyObject(T*& YourObject, const TCHAR* Path)
{
	if (ConstructorHelpers::FObjectFinder<T> ObjectFinder(Path); ObjectFinder.Succeeded())
	{
		YourObject = ObjectFinder.Object;
	}
}

void UBuildingStaticMeshComp::OnTimelineTick(float DeltaTime)
{
	MyTimeline.TickTimeline(DeltaTime);

	FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	FVector FinalNormalizedLocation = GetComponentLocation() - PlayerLocation;
	UKismetMathLibrary::Vector_Normalize(FinalNormalizedLocation);

	ADC_Building* Building = Cast<ADC_Building>(GetOwner());

	FVector NewLocationFinal = UKismetMathLibrary::VLerp(GetRelativeLocation(), GetRelativeLocation() + FinalNormalizedLocation *Building->LengthOfSplitBuildingOut,
	                                                     CurveFloat->GetFloatValue(DeltaTime));
	SetRelativeLocation(NewLocationFinal);
}

void UBuildingStaticMeshComp::OnTimelineEndEvent()
{
}

UBuildingStaticMeshComp::UBuildingStaticMeshComp(): CurveFloat(nullptr)
{
	FindMyObject<UCurveFloat>(CurveFloat, *SplitBuildingCurve_Path);
}


void UBuildingStaticMeshComp::BeginPlay()
{
	Super::BeginPlay();
	if (CurveFloat)
	{
		FOnTimelineFloat OnTimelineFloatEvent;
		FOnTimelineEvent OnTimelineFinishedEvent;

		OnTimelineFloatEvent.BindUFunction(this, TEXT("OnTimelineTick"));
		OnTimelineFinishedEvent.BindUFunction(this, TEXT("OnTimelineEndEvent"));
		MyTimeline.AddInterpFloat(CurveFloat, OnTimelineFloatEvent);
		MyTimeline.SetTimelineLength(ETimelineLengthMode::TL_LastKeyFrame);

		MyTimeline.SetTimelineFinishedFunc(OnTimelineFinishedEvent);
	}
}
