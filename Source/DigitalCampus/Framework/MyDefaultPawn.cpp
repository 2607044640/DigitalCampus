// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDefaultPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "DigitalCampus/Building/DC_Building.h"
#include "DigitalCampus/Components/BuildingStaticMeshComp.h"
#include "DigitalCampus/Other/Paths.h"
#include "GameFramework/PlayerInput.h"
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
	// CameraComponent->bUsePawnControlRotation = false;
	LeftMouseDown = false;
	RightMouseDown = false;
	Building = InBuilding;
	TickRotationbyBuildingSMComp = false;
	SavedTempLocation = GetActorLocation();
	MyPawnTimeline.PlayFromStart();
}

void AMyDefaultPawn::AddControllerYawInput(float Val)
{
	if (!TickRotationbyBuilding || !RotatePawnByBuildingSpeed)
	{
		Super::AddControllerYawInput(Val);
	}
}

void AMyDefaultPawn::AddControllerPitchInput(float Val)
{
	if (!TickRotationbyBuilding || !RotatePawnByBuildingSpeed)
	{
		Super::AddControllerPitchInput(Val);
	}
}

void AMyDefaultPawn::OnMouseClickedFunc()
{
	if (TickRotationbyBuilding)
	{
		// GetController()->SetControlRotation(CameraComponent->GetComponentRotation());
		// CameraComponent->bUsePawnControlRotation = true;
	}
	TickRotationbyBuilding = false;
}

void AMyDefaultPawn::OnMouseClickedBuildingStaticMesh(UBuildingStaticMeshComp* StaticMeshComponent)
{
	BuildingStaticMeshComponent = StaticMeshComponent;
	ADC_Building* InBuilding = Cast<ADC_Building>(BuildingStaticMeshComponent->GetOwner());
	Building = InBuilding;
	TickRotationbyBuildingSMComp = true;
	// CameraComponent->bUsePawnControlRotation = false;
}

AMyDefaultPawn::AMyDefaultPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	FindMyObject<UCurveFloat>(CurveFloat, *BuildingCurve_Path);

	WidgetInteractionComponent = CreateDefaultSubobject<
		UWidgetInteractionComponent>(TEXT("WidgetInteractionComponent"));
	WidgetInteractionComponent->SetupAttachment(RootComponent);
	WidgetInteractionComponent->InteractionDistance = MAX_flt;

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
	FRotator LookAtRotation;

	if (BuildingStaticMeshComponent)
	{
		FVector StartLocation = GetActorLocation();
		FVector TargetLocation = BuildingStaticMeshComponent->GetComponentLocation();
		LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
	}
	else
	{
		FVector StartLocation = GetActorLocation();
		FVector TargetLocation = Building->GetActorLocation();
		LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
	}

	//todo
	GetController()->SetControlRotation(LookAtRotation);
}

void AMyDefaultPawn::OnTimelineTick(float DeltaTime)
{
	//点击按钮滞空comp
	if (!LeftMouseDown || !RightMouseDown)
	{
		SetRotationByBuilding();
		FVector NewLocationFinal;
		//Location
		NewLocationFinal = UKismetMathLibrary::VLerp(SavedTempLocation,
		                                             BuildingStaticMeshComponent
			                                             ? BuildingStaticMeshComponent->GetComponentLocation() +
			                                             Building->BuildingRelativeDistanceFromPlayer
			                                             : Building->GetActorLocation() +
			                                             Building->BuildingRelativeDistanceFromPlayer,
		                                             CurveFloat->GetFloatValue(DeltaTime));

		SetActorLocation(NewLocationFinal);
	}
	else
	{
		// GetController()->SetControlRotation(CameraComponent->GetComponentRotation());
		// CameraComponent->bUsePawnControlRotation = true;
		MyPawnTimeline.Stop();
	}
}

void AMyDefaultPawn::OnTimelineEndEvent()
{
	if (!LeftMouseDown || !RightMouseDown)
	{
		TickRotationbyBuilding = true;

		if (BuildingStaticMeshComponent)
		{
			BuildingStaticMeshComponent->SMComp_StartTimeline();
		}
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
		MyPawnTimeline.AddInterpFloat(CurveFloat, OnTimelineFloatEvent);
		MyPawnTimeline.SetTimelineLength(ETimelineLengthMode::TL_LastKeyFrame);
		MyPawnTimeline.SetPlayRate(PlayerRate);
		MyPawnTimeline.SetTimelineFinishedFunc(OnTimelineFinishedEvent);
	}
}

void AMyDefaultPawn::BeginPlay()
{
	Super::BeginPlay();
	TimelineBegin();
	OnRightMouseClick_Canceled.AddUObject(this, &AMyDefaultPawn::OnMouseClickedFunc);
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

void AMyDefaultPawn::SetTransformWhenViewBuilding()
{
	if (TickRotationbyBuildingSMComp)
	{
		//Rotation
		FVector StartLocation = GetActorLocation();
		FVector TargetLocation = BuildingStaticMeshComponent->GetComponentLocation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
		CameraComponent->SetWorldRotation(LookAtRotation);
	}
	if (TickRotationbyBuilding)
	{
		if (Building)
		{
			SetRotationByBuilding();
			if (Building->AddedWidgetComponent || Building->BuildingMainWidgetComponent)
			{
				Building->BuildingMainWidgetComponent->SetHiddenInGame(true);
				Building->AddedWidgetComponent->SetHiddenInGame(true);
			}

			//Location
			FVector CenterLocation = BuildingStaticMeshComponent
				                         ? BuildingStaticMeshComponent->GetComponentLocation()
				                         : Building->GetActorLocation();
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
			if (Building->AddedWidgetComponent && Building->BuildingMainWidgetComponent)
			{
				Building->BuildingMainWidgetComponent->SetHiddenInGame(false);
				Building->AddedWidgetComponent->SetHiddenInGame(false);
			}
		}
	}
}

void AMyDefaultPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MyPawnTimeline.TickTimeline(DeltaTime);
	SetTransformWhenViewBuilding();

	if (MyPawnTimeline.IsPlaying())
	{
		DisableInput(GetLocalViewingPlayerController());
	}
	else
	{
		EnableInput(GetLocalViewingPlayerController());
	}
}

void AMyDefaultPawn::LeftMousePressed()
{
	WidgetInteractionComponent->PressPointerKey(EKeys::LeftMouseButton);
	LeftMouseDown = true;
	// if (OnMouseClicked.IsBound())
	// {
	// 	OnMouseClicked.Broadcast();
	// }
}


void AMyDefaultPawn::RightMouse()
{
	if (TickRotationbyBuildingSMComp)
	{
		TickRotationbyBuildingSMComp = false;
		// GetController()->SetControlRotation(CameraComponent->GetComponentRotation());
		// CameraComponent->bUsePawnControlRotation = true;
	}

	RightMouseDown = true;
	if (OnRightMouseClick_Canceled.IsBound())
	{
		OnRightMouseClick_Canceled.Broadcast();
	}
}

void AMyDefaultPawn::LeftMouseReleased()
{
	WidgetInteractionComponent->ReleasePointerKey(EKeys::LeftMouseButton);
}

void AMyDefaultPawn::RightMouseReleased()
{
}

//
// void InitializeDefaultPawnInputBindings()
// {
// 	static bool bBindingsAdded = false;
// 	if (!bBindingsAdded)
// 	{
// 		bBindingsAdded = true;
//
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveForward", EKeys::W, 1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveForward", EKeys::S, -1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveForward", EKeys::Up, 1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveForward", EKeys::Down, -1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveForward", EKeys::Gamepad_LeftY, 1.f));
//
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveRight", EKeys::A, -1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveRight", EKeys::D, 1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveRight", EKeys::Gamepad_LeftX, 1.f));
//
// 		// HACK: Android controller bindings in ini files seem to not work
// 		//  Direct overrides here some to work
// #if !PLATFORM_ANDROID
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveUp", EKeys::Gamepad_LeftThumbstick, 1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveUp", EKeys::Gamepad_RightThumbstick, -1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveUp", EKeys::Gamepad_FaceButton_Bottom, 1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveUp", EKeys::LeftControl, -1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveUp", EKeys::SpaceBar, 1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveUp", EKeys::C, -1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveUp", EKeys::E, 1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveUp", EKeys::Q, -1.f));
// #else
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveUp", EKeys::Gamepad_LeftTriggerAxis, -0.5f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveUp", EKeys::Gamepad_RightTriggerAxis, 0.5f));
// #endif
//
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_TurnRate", EKeys::Gamepad_RightX, 1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_TurnRate", EKeys::Left, -1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_TurnRate", EKeys::Right, 1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_Turn", EKeys::MouseX, 1.f));
//
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_LookUpRate", EKeys::Gamepad_RightY, 1.f));
// 		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_LookUp", EKeys::MouseY, -1.f));
// 	}
// }

void AMyDefaultPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// check(PlayerInputComponent);
	//
	// if (bAddDefaultMovementBindings)
	// {
	// 	InitializeDefaultPawnInputBindings();
	//
	// 	PlayerInputComponent->BindAxis("DefaultPawn_MoveForward", this, &ADefaultPawn::MoveForward);
	// 	PlayerInputComponent->BindAxis("DefaultPawn_MoveRight", this, &ADefaultPawn::MoveRight);
	// 	PlayerInputComponent->BindAxis("DefaultPawn_MoveUp", this, &ADefaultPawn::MoveUp_World);
	// 	PlayerInputComponent->BindAxis("DefaultPawn_Turn", this, &ADefaultPawn::AddControllerYawInput);
	// 	PlayerInputComponent->BindAxis("DefaultPawn_TurnRate", this, &ADefaultPawn::TurnAtRate);
	// 	PlayerInputComponent->BindAxis("DefaultPawn_LookUp", this, &ADefaultPawn::AddControllerPitchInput);
	// 	PlayerInputComponent->BindAxis("DefaultPawn_LookUpRate", this, &ADefaultPawn::LookUpAtRate);
	// }

	PlayerInputComponent->BindAction("LeftMouse", IE_Pressed, this, &AMyDefaultPawn::LeftMousePressed);
	PlayerInputComponent->BindAction("LeftMouse", IE_Released, this, &AMyDefaultPawn::LeftMouseReleased);
	PlayerInputComponent->BindAction("RightMouse", IE_Pressed, this, &AMyDefaultPawn::RightMouse);
	PlayerInputComponent->BindAction("LeftMouse", IE_Released, this, &AMyDefaultPawn::RightMouseReleased);
}
