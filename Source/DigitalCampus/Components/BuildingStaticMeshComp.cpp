// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingStaticMeshComp.h"

#include "DigitalCampus/Building/DC_Building.h"
#include "DigitalCampus/Framework/MyDefaultPawn.h"
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
	else
	{
		FString TempStr = FString::Printf(TEXT("Not Found! : %s"), Path);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TempStr, true, FVector2D(3, 3));
		UE_LOG(LogTemp, Error, TEXT("%s"), *TempStr);
	}
}

void UBuildingStaticMeshComp::OnTimelineTick(float DeltaTime)
{
	FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();

	FVector FaceVector(0, 0, 0);
	switch (FaceSituation)
	{
	case 1:
		//R
		FaceVector = GetRightVector();
		break;
	case 2:
		//-R
		FaceVector = -GetRightVector();
		break;
	case 3:
		//F
		FaceVector = GetForwardVector();
		break;
	case 4:
		//-F
		FaceVector = -GetForwardVector();
		break;
	}


	FVector StartLocation = GetRelativeLocation();
	FVector EndLocation = bIsClicked
		                      ? GetRelativeLocation() + FaceVector *
		                      GetOwner_DC_Building->
		                      LengthOfSplitBuildingOut
		                      : SavedLocation;
	FVector NewLocationFinal = UKismetMathLibrary::VLerp(StartLocation,
	                                                     EndLocation,
	                                                     CurveFloat->GetFloatValue(DeltaTime));
	SetRelativeLocation(NewLocationFinal);
}

void UBuildingStaticMeshComp::OnTimelineEndEvent()
{
}

void UBuildingStaticMeshComp::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SMComp_Timeline.TickTimeline(DeltaTime);
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
		SMComp_Timeline.AddInterpFloat(CurveFloat, OnTimelineFloatEvent);
		SMComp_Timeline.SetTimelineLength(ETimelineLengthMode::TL_LastKeyFrame);

		SMComp_Timeline.SetTimelineFinishedFunc(OnTimelineFinishedEvent);
	}

	this->OnClicked.AddDynamic(this, &UBuildingStaticMeshComp::StaticMeshComponentOnClicked);
	SavedMaterial = GetMaterial(0); //todo 0

	MyDefaultPawn = Cast<AMyDefaultPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	OnMouseClickedBuildingStaticMesh.AddUObject(MyDefaultPawn, &AMyDefaultPawn::OnMouseClickedBuildingStaticMesh);

	GetOwner_DC_Building = Cast<ADC_Building>(GetOwner());
	OnMouseClickedBuildingStaticMesh.AddUObject(
		GetOwner_DC_Building, &ADC_Building::OnMouseClickedBuildingStaticMesh);
}

UBuildingStaticMeshComp::UBuildingStaticMeshComp()
{
	PrimaryComponentTick.bCanEverTick = true; //Improtant: cant place it in BeginPlay
	FindMyObject<UCurveFloat>(CurveFloat, *SplitBuildingCurve_Path);
}

void UBuildingStaticMeshComp::MaterialFadeOut()
{
	FString TempStr = FString::Printf(TEXT("fadeout,%s"), *GetName());
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TempStr, true, FVector2D(3, 3));
	UE_LOG(LogTemp, Error, TEXT("%s"), *TempStr);
}

int32 UBuildingStaticMeshComp::CalcFace()
{
	FVector Distance = MyDefaultPawn->GetActorLocation() - GetComponentLocation();
	Distance.Normalize();

	int Situation = -1;

	float DotValue_Temp = FVector::DotProduct(GetRightVector(), Distance);
	float DotValue = -10;

	if (DotValue < DotValue_Temp)
	{
		Situation = 1;
		DotValue = DotValue_Temp;
	}

	DotValue_Temp = FVector::DotProduct(GetRightVector() * -1, Distance);
	if (DotValue < DotValue_Temp)
	{
		Situation = 2;
		DotValue = DotValue_Temp;
	}

	DotValue_Temp = FVector::DotProduct(GetForwardVector(), Distance);
	if (DotValue < DotValue_Temp)
	{
		Situation = 3;
		DotValue = DotValue_Temp;
	}

	DotValue_Temp = FVector::DotProduct(GetForwardVector() * -1, Distance);
	if (DotValue < DotValue_Temp)
	{
		Situation = 4;
	}

	return Situation;
}

void UBuildingStaticMeshComp::SMComp_StartTimeline()
{
	FaceSituation = CalcFace();
	SavedLocation = GetRelativeLocation();
	SMComp_Timeline.PlayFromStart();
}

void UBuildingStaticMeshComp::StaticMeshComponentOnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (bIsClicked)
	{
		SMComp_Timeline.ReverseFromEnd();
	}
	else
	{
		UBuildingStaticMeshComp* StaticMeshComponent = Cast<UBuildingStaticMeshComp>(TouchedComponent);

		if (OnMouseClickedBuildingStaticMesh.IsBound())
		{
			OnMouseClickedBuildingStaticMesh.Broadcast(StaticMeshComponent);
		}
		MyDefaultPawn->TimelineStart(GetOwner_DC_Building);

		// BuildingMainWidgetComponent->SetHiddenInGame(true);
		// AddedWidgetComponent->SetHiddenInGame(true);
	}
	bIsClicked = !bIsClicked;
}
