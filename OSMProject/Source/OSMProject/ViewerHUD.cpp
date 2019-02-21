// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewerHUD.h"
#include "Engine/Canvas.h"
#include "Engine/GameEngine.h"
#include "Public/CanvasItem.h"

AViewerHUD::AViewerHUD() {
	CrosshairColor = FColor::White;
	CrosshairSize = 15;
}

void AViewerHUD::DrawHUD()
{
	Super::DrawHUD();

	// Find the center of our canvas.
	FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	//Crosshair
	Draw2DLine(Center.X - CrosshairSize, Center.Y, Center.X + CrosshairSize, Center.Y, CrosshairColor);
	Draw2DLine(Center.X, Center.Y - CrosshairSize, Center.X, Center.Y + CrosshairSize, CrosshairColor);

	DrawText(TEXT("ViewerHUD v1"), FLinearColor::Black, Center.X - 40, 10);
}

void AViewerHUD::setCrosshairColor(FColor color) {
	CrosshairColor = color;
}

