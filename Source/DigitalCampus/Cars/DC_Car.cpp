// This project is Digital Campus. If you use this project maliciously, you will be held accountable according to law. CopyRight_Zonghan Luo


#include "DC_Car.h"

#include "DigitalCampus/Other/Paths.h"
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

ADC_Car::ADC_Car()
{
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	FindMyObject<UCurveFloat>(CurveFloat, *ArrivedCarCurve_Path);
}

void ADC_Car::BeginPlay()
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
	SavedLocation = GetActorLocation();
}

void ADC_Car::OnTimelineTick(float DeltaTime)
{
	FVector StartLocation = SavedLocation;
	FVector EndLocation = GetActorForwardVector() * DistanceToGetOut;
	FVector NewLocationFinal = UKismetMathLibrary::VLerp(StartLocation, EndLocation,
	                                                     CurveFloat->GetFloatValue(DeltaTime));
	SetActorLocation(NewLocationFinal);
}

void ADC_Car::OnTimelineEndEvent()
{
	Destroy();
}

void ADC_Car::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MyTimeline.TickTimeline(DeltaTime);
}
