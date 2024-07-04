// Fill out your copyright notice in the Description page of Project Settings.


#include "DC_Building.h"

#include "Components/Button.h"
#include "Components/WidgetComponent.h"
#include "DigitalCampus/Framework/MyDefaultPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include"Components/StaticMeshComponent.h"
#include "DigitalCampus/Components/BuildingStaticMeshComp.h"
#include "DigitalCampus/Other/Paths.h"
#include "DigitalCampus/UMG/UMG_Building.h"

template <typename T>
void FindMyClass(TSubclassOf<T>& YourSubClass, const TCHAR* Path)
{
	if (ConstructorHelpers::FClassFinder<T> ClassFinder(Path); ClassFinder.Succeeded())
	{
		YourSubClass = ClassFinder.Class;
	}
}

void ADC_Building::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	int BuildNumCount = 0;
	float BuildingHeight = 0;
	for (FBuildingInfo BuildingInfo : BuildingInfos)
	{
		for (int i = 0; i < BuildingInfo.BuildingCount; ++i)
		{
			BuildNumCount++;
			// auto InStaticMeshComponent = NewObject<UStaticMeshComponent>(this);
			// InStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
			// InStaticMeshComponent->AttachToComponent(RootComponent,
			// FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			UBuildingStaticMeshComp* InStaticMeshComponent = Cast<UBuildingStaticMeshComp>(
				AddComponentByClass(UBuildingStaticMeshComp::StaticClass(), false,
				                    FTransform{
					                    FRotator::ZeroRotator, FVector(0, 0, 0),
					                    FVector(1, 1, 1)
				                    },
				                    false));
			if (BuildingInfo.StaticMeshToAdd)
			{
				if (BuildingInfo.BuildingHeight == -1)
				{
					InStaticMeshComponent->SetRelativeLocation(FVector(0, 0, BuildingHeight + DefaultBuildingHeight));
					BuildingHeight += DefaultBuildingHeight;
				}
				else
				{
					InStaticMeshComponent->SetRelativeLocation(
						FVector(0, 0, BuildingHeight + BuildingInfo.BuildingHeight));
					BuildingHeight += BuildingInfo.BuildingHeight;
				}
				InStaticMeshComponent->SetupAttachment(RootComponent);
				InStaticMeshComponent->SetStaticMesh(BuildingInfo.StaticMeshToAdd);
				InStaticMeshComponent->OnClicked.AddDynamic(this, &ADC_Building::StaticMeshComponentOnClicked);
			}
		}
	}

	BuildingMainWidgetComponent->SetRelativeLocation(FVector(0, 0, BuildingHeight + UMGHeight));
}

void ADC_Building::JFAddWidget(TSubclassOf<UUserWidget> InWidgetClass, FVector2D PosToAdd)
{
	if (!AddedWidgetComponent)
	{
		AddedWidgetComponent = NewObject<UWidgetComponent>(this);
		AddedWidgetComponent->RegisterComponentWithWorld(GetWorld());
		AddedWidgetComponent->AttachToComponent(RootComponent,
		                                        FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		FVector AddedLocation(0, PosToAdd.X, PosToAdd.Y);
		AddedWidgetComponent->SetRelativeLocation(BuildingMainWidgetComponent->GetRelativeLocation() + AddedLocation);

		//SetWidget
		auto ObjectPtr = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(), 0), InWidgetClass);
		AddedWidgetComponent->SetWidget(ObjectPtr);
	}
}

void ADC_Building::StaticMeshComponentOnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	AMyDefaultPawn* MyDefaultPawn = Cast<AMyDefaultPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	UBuildingStaticMeshComp* StaticMeshComponent = Cast<UBuildingStaticMeshComp>(TouchedComponent);

	MyDefaultPawn->OnMouseClickStaticMesh(StaticMeshComponent);
	// // BuildingMainWidgetComponent->SetHiddenInGame(true);
	// // AddedWidgetComponent->SetHiddenInGame(true);
	// MyDefaultPawn->TimelineStart(this);
}


ADC_Building::ADC_Building()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	// StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	// StaticMeshComponent->SetupAttachment(SceneComponent);
	// StaticMeshComponent->OnClicked.AddDynamic(this, &ADC_Building::StaticMeshComponentOnClicked);

	BuildingMainWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MainWidgetComponent"));
	BuildingMainWidgetComponent->SetupAttachment(SceneComponent);

	FindMyClass<UUserWidget>(WBP_Building, *WBP_Building_Path);
	BuildingMainWidgetComponent->SetWidgetClass(WBP_Building);
	
}

void ADC_Building::ViewBuildingButtonOnClicked()
{
	AMyDefaultPawn* MyDefaultPawn = Cast<AMyDefaultPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	MyDefaultPawn->TimelineStart(this);
}

void ADC_Building::BeginPlay()
{
	Super::BeginPlay();
	UUMG_Building* UMG_Building = Cast<UUMG_Building>(BuildingMainWidgetComponent->GetWidget());
	UMG_Building->Button_Building->OnClicked.AddDynamic(
		this, &ADC_Building::ViewBuildingButtonOnClicked);
}

void ADC_Building::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (APlayerCameraManager* PlayerCamera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		FVector StartLocation = BuildingMainWidgetComponent->GetComponentLocation();
		FVector TargetLocation = PlayerCamera->GetCameraLocation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
		BuildingMainWidgetComponent->SetWorldRotation(LookAtRotation);
		if (AddedWidgetComponent)
		{
			AddedWidgetComponent->SetWorldRotation(LookAtRotation);
		}
	}
}
