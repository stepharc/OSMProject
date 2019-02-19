// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

//! PlayerPawn class.
/*!
 * This is the class which manages the player camera in the viewer.
 */

UCLASS()
class OSMPROJECT_API APlayerPawn : public APawn
{
	GENERATED_BODY()

private:
	//! DoTrace member.
	/*!
	 * \param RV_Hit a pointer in which it'll contain results of the raycasting.
	 * \param RV_TraceParams parameters for the trace.
	 * \return true if there's at least one intersected object as a result of the raytracing.
	 * false otherwise.
	 * Cast a ray from the user's camera position and orientation : its length
	 * is equal to InteractionDistance variable. Store results of the raycasting
	 * in RV_Hit parameter.
	 */
	bool DoTrace(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams);

protected:
	
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* CameraSpringArm; /*!< //Spring arms are a good, simple way to attach a camera (or other Components) so that they're not overly rigid and feel more fluid as they move. */
	class UCameraComponent* Camera;

	//Input variables
	FVector2D MovementInput; /*!< For mouse movement detection. */
	FVector2D CameraInput; /*!< For camera yaw and pitch manipulation. */
	float ZoomFactor; /*!< For camera zoom manipulation. */
	bool bZoomingIn; /*!< Zoom / Unzoom the camera. */

	//! selectedActor variable.
	/*!
	 * Actor (= element) selected by the user. NULL otherwise.
	 */ 
	AActor * selectedActor;
	
	//! hoveredActor variable.
	/*!
	 * Actor hovered by the user's camera. NULL if the camera points on nothing.
	 */
	AActor * hoveredActor;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Input functions
	//! MoveForward member.
	/*! \param AxisValue based on its value, camera will move forward or backward.
	 */
	void MoveForward(float AxisValue);
	
	//! MoveRight member.
	/*! \param AxisValue based on its value, camera will move right or left.
	 */
	void MoveRight(float AxisValue);
	
	//! PitchCamera member.
	/*! \param AxisValue pitch camera's future value.
	 */
	void PitchCamera(float AxisValue);
	
	//! YawCamera member.
	/*! \param AxisValue yaw camera's future value.
	 */
	void YawCamera(float AxisValue);
	
	//! ChangeSelectedActorVisibility member.
	/*! 
	 * Hide or show selected actor based on its game visibility.
	 * If there's no actor selected, this member will do nothing.
	 */
	void ChangeSelectedActorVisibility();
	
	//! ZoomIn member.
	/*! 
	 * Request a zoom on the user's camera.
	 */
	void ZoomIn();
	
	//! ZoomOut member.
	/*! 
	 * Request an unzoom on the user's camera.
	 */
	void ZoomOut();
	
	//! CastTrace member.
	/*! 
	 * \see DoTrace(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams)
	 * \see selectedActor
	 * Cast a ray, and analyse the results. If there's one intersected actor,
	 * modify selectedActor variable. Otherwise, set selectedActor to NULL.
	 */
	void CastTrace();
	
	//! CastTrace member.
	/*! 
	 * \see DoTrace(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams)
	 * \see hoveredActor
	 * Cast a ray, and analyse the results. If there's one intersected actor,
	 * modify hoveredActor variable. Otherwise, set hoveredActor to NULL.
	 */
	void CastHoverTrace();

public:
	//! APlayerPawn constructor.
	/*!
	 * Set up the spring arm, the camera and player control on it.
	 */
	APlayerPawn();

	//! Tick member.
	/*!
	 * Called every frame. Check if there's need to zoom/unzoom, rotate,
	 * translate the user's camera.
	 */
	virtual void Tick(float DeltaTime) override;

	//! SetupPlayerInputComponent member.
	/*!
	 * Called to bind functionality to input. 
	 * Hook up events for "ZoomIn", "ZoomOut", "Raycast", "Hover Raycast" and "Hide/Show slected actor".
	 * Hook up every-frame handling for camera translation, pitch and yaw.
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
