// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDefaultPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "DigitalCampus/Building/DC_Building.h"
#include "DigitalCampus/Components/BuildingStaticMeshComp.h"
#include "DigitalCampus/Other/Paths.h"
#include "GameFramework/SpringArmComponent.h"
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


void AMyDefaultPawn::TimelineStart(ADC_Building* InBuilding)
{
	CameraComponent->bUsePawnControlRotation = false;
	LeftMouseDown = false;
	RightMouseDown = false;
	Building = InBuilding;
	TickRotationbyBuildingSMComp = false;
	SavedTempLocation = GetActorLocation();
	MyTimeline.PlayFromStart();
}

void AMyDefaultPawn::OnMouseClickedFunc()
{
	if (TickRotationbyBuilding)
	{
		GetController()->SetControlRotation(CameraComponent->GetComponentRotation());
		CameraComponent->bUsePawnControlRotation = true;
	}
	TickRotationbyBuilding = false;
}

void AMyDefaultPawn::OnMouseClickStaticMesh(UBuildingStaticMeshComp*& StaticMeshComponent)
{
	BuildingStaticMeshComponent = StaticMeshComponent;
	ADC_Building* InBuilding = Cast<ADC_Building>(BuildingStaticMeshComponent->GetOwner());
	Building = InBuilding;

	TickRotationbyBuildingSMComp = true;

	CameraComponent->bUsePawnControlRotation = false;
}

AMyDefaultPawn::AMyDefaultPawn()
{
	WidgetInteractionComponent = CreateDefaultSubobject<
		UWidgetInteractionComponent>(TEXT("WidgetInteractionComponent"));
	WidgetInteractionComponent->SetupAttachment(RootComponent);
	PrimaryActorTick.bCanEverTick = true;
	FindMyObject<UCurveFloat>(CurveFloat, *BuildingCurve_Path);
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CamaraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void AMyDefaultPawn::SetRotationByBuilding()
{
	//Rotation
	FVector StartLocation = GetActorLocation();
	FVector TargetLocation = Building->GetActorLocation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
	CameraComponent->SetWorldRotation(LookAtRotation);
}

void AMyDefaultPawn::OnTimelineTick(float DeltaTime)
{
	if (!LeftMouseDown || !RightMouseDown)
	{
		SetRotationByBuilding();

		//Location
		FVector Distance = SavedTempLocation - Building->GetActorLocation();
		UKismetMathLibrary::Vector_Normalize(Distance);
		FVector NewLocationFinal = UKismetMathLibrary::VLerp(SavedTempLocation, Building->GetActorLocation() +
		                                                     Distance * DistanceBetweenBuilding + FVector(
			                                                     0, 0, Building->DefaultBuildingHeight),
		                                                     CurveFloat->GetFloatValue(DeltaTime));
		SetActorLocation(NewLocationFinal);
	}
	else
	{
		GetController()->SetControlRotation(CameraComponent->GetComponentRotation());
		CameraComponent->bUsePawnControlRotation = true;
		MyTimeline.Stop();
	}
}

void AMyDefaultPawn::OnTimelineEndEvent()
{
	if (!LeftMouseDown || !RightMouseDown)
	{
		TickRotationbyBuilding = true;
	}
}

void AMyDefaultPawn::TimelineBegin()
{
	if (CurveFloat)
	{
		FOnTimelineFloat OnTimelineFloatEvent;
		FOnTimelineEvent OnTimelineFinishedEvent;

		OnTimelineFloatEvent.BindUFunction(this, TEXT("OnTimelineTick"));
		OnTimelineFinishedEvent.BindUFunction(this, TEXT("OnTimelineEndEvent"));
		MyTimeline.AddInterpFloat(CurveFloat, OnTimelineFloatEvent);
		MyTimeline.SetTimelineLength(ETimelineLengthMode::TL_LastKeyFrame);
		MyTimeline.SetPlayRate(PlayerRate);
		MyTimeline.SetTimelineFinishedFunc(OnTimelineFinishedEvent);
	}
}

void AMyDefaultPawn::BeginPlay()
{
	Super::BeginPlay();
	TimelineBegin();
	OnMouseClicked.AddUObject(this, &AMyDefaultPawn::OnMouseClickedFunc);
}

FHitResult AMyDefaultPawn::LineTraceSingleForObjects_CameraShoot(float Distance,
                                                                 TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes)
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector Startpoint = CameraManager->GetCameraLocation();
	FVector Endpoint = CameraManager->GetActorForwardVector().GetSafeNormal() * Distance + CameraManager->
		GetCameraLocation();

	// ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

	TArray<AActor*> ActorsToIgnore;
	FHitResult OnHit;
	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Startpoint, Endpoint, ObjectTypes, false,
	                                                ActorsToIgnore,
	                                                EDrawDebugTrace::Persistent, OnHit, true);
	return OnHit;
}

void AMyDefaultPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MyTimeline.TickTimeline(DeltaTime);


	if (TickRotationbyBuildingSMComp)
	{
		//Rotation
		FVector StartLocation = GetActorLocation();
		FVector TargetLocation = BuildingStaticMeshComponent->GetComponentLocation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
		CameraComponent->SetWorldRotation(LookAtRotation);

		FString TempStr = FString::Printf(TEXT("%f , %f , %f"), LookAtRotation.Pitch, TargetLocation.Y,
		                                  TargetLocation.Z);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TempStr, true, FVector2D(3, 3));
		UE_LOG(LogTemp, Error, TEXT("%s"), *TempStr);
	}


	if (TickRotationbyBuilding)
	{
		SetRotationByBuilding();
		if (Building)
		{
			if (Building->AddedWidgetComponent || Building->BuildingMainWidgetComponent)
			{
				Building->BuildingMainWidgetComponent->SetHiddenInGame(true);
				Building->AddedWidgetComponent->SetHiddenInGame(true);
			}


			//Location
			FVector CenterLocation = Building->GetActorLocation();
			FVector Direction = GetActorLocation() - CenterLocation;

			float DeltaAngle = RotatePawnByBuildingSpeed;
			float CosAngle = FMath::Cos(DeltaAngle);
			float SinAngle = FMath::Sin(DeltaAngle);

			float NewX = Direction.X * CosAngle - Direction.Y * SinAngle;
			float NewY = Direction.X * SinAngle + Direction.Y * CosAngle;

			FVector NewLocation = CenterLocation + FVector(NewX, NewY, Direction.Z);
			SetActorLocation(NewLocation);
		}
	}
	else
	{
		if (Building)
		{
			if (Building->AddedWidgetComponent || Building->BuildingMainWidgetComponent)
			{
				Building->BuildingMainWidgetComponent->SetHiddenInGame(false);
				Building->AddedWidgetComponent->SetHiddenInGame(false);
			}
		}
	}
}

void AMyDefaultPawn::LeftMousePressed()
{
	WidgetInteractionComponent->PressPointerKey(EKeys::LeftMouseButton);


	LeftMouseDown = true;
	if (OnMouseClicked.IsBound())
	{
		OnMouseClicked.Broadcast();
	}
}


void AMyDefaultPawn::RightMouse()
{
	if (TickRotationbyBuildingSMComp)
	{
		TickRotationbyBuildingSMComp = false;
		GetController()->SetControlRotation(CameraComponent->GetComponentRotation());
		CameraComponent->bUsePawnControlRotation = true;
	}

	RightMouseDown = true;
	if (OnMouseClicked.IsBound())
	{
		OnMouseClicked.Broadcast();
	}
}

void AMyDefaultPawn::LeftMouseReleased()
{
	WidgetInteractionComponent->ReleasePointerKey(EKeys::LeftMouseButton);
}

void AMyDefaultPawn::RightMouseReleased()
{
}

void AMyDefaultPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("LeftMouse", IE_Pressed, this, &AMyDefaultPawn::LeftMousePressed);
	PlayerInputComponent->BindAction("LeftMouse", IE_Released, this, &AMyDefaultPawn::LeftMouseReleased);
	PlayerInputComponent->BindAction("RightMouse", IE_Pressed, this, &AMyDefaultPawn::RightMouse);
	PlayerInputComponent->BindAction("LeftMouse", IE_Released, this, &AMyDefaultPawn::RightMouseReleased);
}
