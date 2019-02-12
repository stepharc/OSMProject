// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "OSMProject.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Source : https://docs.unrealengine.com/en-us/Programming/Tutorials/PlayerCamera/1
	//Creates a basic, empty Scene Component as the root of our Component hierarchy, then creates and attaches a SpringArmComponent to it. 
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);

	//The Spring Arm is then set at a default pitch of -60 degrees (that is, looking 60 degrees downward) and a position of 50 units above the root.
	CameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));

	//We also establish a few values specific to the Spring Arm Component class that will determine its length and the smoothness of its motion.
	CameraSpringArm->TargetArmLength = 400.f;
	CameraSpringArm->bEnableCameraLag = true;
	CameraSpringArm->CameraLagSpeed = 3.0f;

	//We need to create and attach a Camera Component to the socket on the end of the Spring Arm Component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);

	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Zoom in if ZoomIn button is down, zoom back out if it's not
	{
		if (bZoomingIn)
		{
			ZoomFactor += DeltaTime / 0.5f;         //Zoom in over half a second
		}
		else
		{
			ZoomFactor -= DeltaTime / 0.25f;        //Zoom out over a quarter of a second
		}
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);

		//Blend our camera's FOV and our SpringArm's length based on ZoomFactor
		Camera->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
		CameraSpringArm->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);
	}
	//Rotate our actor's yaw, which will turn our camera because we're attached to it
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);
	}
	//Rotate our camera's and actor's pitch. 
	{
		FRotator NewRotation = CameraSpringArm->GetComponentRotation();
		FRotator NewActorRotation = GetActorRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, 80.0f);
		NewActorRotation.Pitch = NewRotation.Pitch;
		SetActorRotation(NewActorRotation);
		CameraSpringArm->SetWorldRotation(NewRotation);
	}
	//Handle movement based on our "MoveX" and "MoveY" axes
	{
		if (!MovementInput.IsZero())
		{
			//Scale our movement input axis values by 1000 units per second
			MovementInput = MovementInput.GetSafeNormal() * 1000.0f;
			FVector NewLocation = GetActorLocation();
			NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
			NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
			SetActorLocation(NewLocation);
		}
	}
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Hook up events for "ZoomIn" and "ZoomOut"
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &APlayerPawn::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &APlayerPawn::ZoomOut);

	//Hook up every-frame handling for our four axes
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPawn::MoveRight);
	PlayerInputComponent->BindAxis("CameraPitch", this, &APlayerPawn::PitchCamera);
	PlayerInputComponent->BindAxis("CameraYaw", this, &APlayerPawn::YawCamera);
}

//Input functions
void APlayerPawn::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APlayerPawn::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APlayerPawn::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void APlayerPawn::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void APlayerPawn::ZoomIn()
{
	bZoomingIn = true;
}

void APlayerPawn::ZoomOut()
{
	bZoomingIn = false;
}

