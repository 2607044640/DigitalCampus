// Fill out your copyright notice in the Description page of Project Settings.


#include "DC_Building.h"

#include "Components/WidgetComponent.h"
#include "DigitalCampus/Framework/MyDefaultPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include"Components/StaticMeshComponent.h"


void ADC_Building::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	int TempNum = 0;
	for (UStaticMesh*
	     MeshToAdd : StaticMeshesToAdd)
	{
		TempNum++;

		// auto InStaticMeshComponent = NewObject<UStaticMeshComponent>(this);
		// InStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
		// InStaticMeshComponent->AttachToComponent(RootComponent,
		// FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		UStaticMeshComponent* InStaticMeshComponent = Cast<UStaticMeshComponent>(
			AddComponentByClass(UStaticMeshComponent::StaticClass(), false,
			                    FTransform{
				                    FRotator::ZeroRotator, FVector(0, 0, TempNum * BuildingHeight), FVector(1, 1, 1)
			                    },
			                    false));
		if (MeshToAdd)
		{
			InStaticMeshComponent->SetRelativeLocation(FVector(0, 0, TempNum * BuildingHeight));
			InStaticMeshComponent->SetupAttachment(RootComponent);
			InStaticMeshComponent->SetStaticMesh(MeshToAdd);
			InStaticMeshComponent->OnClicked.AddDynamic(this, &ADC_Building::StaticMeshComponentOnClicked);
		}
	}
	
	BuildingMainWidgetComponent->SetRelativeLocation(FVector(0, 0, TempNum * BuildingHeight + UMGHeight));
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
	// BuildingMainWidgetComponent->SetHiddenInGame(true);
	// AddedWidgetComponent->SetHiddenInGame(true);
	MyDefaultPawn->TimelineStart(this);
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
