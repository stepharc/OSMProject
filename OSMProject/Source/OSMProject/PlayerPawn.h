// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

//DECLARE_DELEGATE_OneParam(ActorDelegate, AActor *);

UCLASS()
class OSMPROJECT_API APlayerPawn : public APawn
{
	GENERATED_BODY()

private:
	bool DoTrace(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams);

protected:
	//Spring arms are a good, simple way to attach a camera (or other Components) so that they're not overly rigid and feel more fluid as they move.
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* CameraSpringArm;
	class UCameraComponent* Camera;

	//Input variables
	FVector2D MovementInput;
	FVector2D CameraInput;
	float ZoomFactor;
	bool bZoomingIn;

	AActor * selectedActor;
	AActor * hoveredActor;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Input functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void ChangeSelectedActorVisibility();
	void ZoomIn();
	void ZoomOut();
	void CastTrace();
	void CastHoverTrace();

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
