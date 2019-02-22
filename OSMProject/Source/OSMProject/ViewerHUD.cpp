// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewerHUD.h"
#include "Engine/Canvas.h"
#include "Engine/GameEngine.h"
#include "Public/CanvasItem.h"

AViewerHUD::AViewerHUD() {
	CrosshairColor_ = FColor::White;
	CrosshairSize_ = 15;
	showInfoBox_ = false;
	infoBoxLines_ = 1;
}

void AViewerHUD::DrawHUD()
{
	Super::DrawHUD();

	// Find the center of our canvas.
	FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// Crosshair
	Draw2DLine(Center.X - CrosshairSize_, Center.Y, Center.X + CrosshairSize_, Center.Y, CrosshairColor_);
	Draw2DLine(Center.X, Center.Y - CrosshairSize_, Center.X, Center.Y + CrosshairSize_, CrosshairColor_);

	// Info Box
	if (showInfoBox_) {
		DrawRect(
			FLinearColor(0, 0, 0, 0.4), // Box color and opacity.
			Center.X + 150,				// X coordinates of box's top left corner.
			40,							// Y coordinates of box's top left corner.
			400,						// Box's width.
			infoBoxLines_ * 15			// Box's height.
		);

		// Info Box content
		for(int i = 0; i < infoBoxLines_; i++){
			DrawText(TEXT("Here it'll be info box content !"), FLinearColor::White, Center.X + 150, 40 + (i * 15));
		}
	}

	DrawText(TEXT("ViewerHUD v1"), FLinearColor::Black, Center.X - 40, 10);
}

void AViewerHUD::setCrosshairColor(FColor color) {
	CrosshairColor_ = color;
}

void AViewerHUD::setInfoBoxVisibility(bool v) {
	showInfoBox_ = v;
}

void AViewerHUD::sendActorInfo(/* Parameter, by reference, in which it'll contain actor info */) {
	// Attribute of ViewerHUD = parameter

	// infoBoxLines_ = number of infos to show
}

