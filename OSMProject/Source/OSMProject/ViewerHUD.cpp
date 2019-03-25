// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewerHUD.h"
#include "Engine/Canvas.h"
#include "Engine/GameEngine.h"
#include "Public/CanvasItem.h"
#include <fstream>

AViewerHUD::AViewerHUD() {
	CrosshairColor_ = FColor::White;
	CrosshairSize_ = 15;
	showInfoBox_ = false;
	showCrosshair_ = true;
	showGameMenu_ = false;
	infoBoxLines_ = 1;
	readOSMArborescenceFile();
}

void AViewerHUD::DrawHUD()
{
	Super::DrawHUD();

	// Info Box
	if (showInfoBox_) {
		drawInfoBoxAndContent();
	}

	if (showGameMenu_) {
		drawGameMenu();
	}

	// Crosshair
	if (showCrosshair_) {
		// Find the center of our canvas.
		FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
		drawCrosshair(Center.X, Center.Y);
	}
	else {
		//Crosshair in Game Menu.
		if (showGameMenu_) {
			APlayerController* const PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
			if (PlayerController != NULL) {
				PlayerController->GetMousePosition(MouseLocation_.X, MouseLocation_.Y);
				drawCrosshair(MouseLocation_.X, MouseLocation_.Y);
			}
		}
	}

	DrawText(TEXT("ViewerHUD v2"), FLinearColor::Black, (Canvas->ClipX * 0.5f) - 25, 10);
}

void AViewerHUD::drawCrosshair(int32 x, int32 y) {
	int32 xmin = x - CrosshairSize_ < 0 ? 0 : x - CrosshairSize_;
	int32 xmax = x + CrosshairSize_ > Canvas->ClipX ? Canvas->ClipX : x + CrosshairSize_;
	int32 ymin = y - CrosshairSize_ < 0 ? 0 : y - CrosshairSize_;
	int32 ymax = y + CrosshairSize_ > Canvas->ClipY ? Canvas->ClipY : y + CrosshairSize_;
	Draw2DLine(xmin, y, xmax, y, CrosshairColor_);
	Draw2DLine(x, ymin, x, ymax, CrosshairColor_);
}

void AViewerHUD::drawInfoBoxAndContent() {
	DrawRect(
		FLinearColor(0, 0, 0, 0.4),		// Box color and opacity.
		(Canvas->ClipX * 0.5f) + 150,	// X coordinates of box's top left corner.
		40,								// Y coordinates of box's top left corner.
		400,							// Box's width.
		infoBoxLines_ * 15				// Box's height.
	);

	// Info Box content
	for (int i = 0; i < infoBoxLines_; i++) {
		DrawText(TEXT("Here it'll be info box content !"), FLinearColor::White, (Canvas->ClipX * 0.5f) + 150, 40 + (i * 15));
	}
}

void AViewerHUD::drawGameMenu() {
	//Game Menu Window.
	DrawRect(
		FLinearColor(1, 0.863, 0.725, 1),
		0,
		0,
		Canvas->ClipX,
		Canvas->ClipY
	);

	//TO DO : Content

	DrawText(TEXT("Press M to quit menu."), FLinearColor::Black, (Canvas->ClipX * 0.5f) - 40, Canvas->ClipY - 40);
}

void AViewerHUD::readOSMArborescenceFile() {
	std::string filePath = "D:\\UnrealProjects\\OSMProject\\SourceOSM\\arbo.txt";
	std::string line;

	//int subcat = 0, tline = 0, cat = 0, emptyline = 0;

	std::ifstream filestream(filePath, std::ios::in);

	if (filestream)
	{
		while (getline(filestream, line)) {
			if (line.find("\t\t") != std::string::npos) {
				//subcat++;
				if (line != "\t\t") { //not a blank line, subcat line.
					//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(line.c_str()));
				}
			}
			else {
				if (line.find("\t") != std::string::npos) { //Which hashmap ? (way/node/relation)
					//tline++;
				}
				else { //cat line.
					//cat++;
				}
			}
		}
		//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, TEXT("Subcats : ") + FString::FromInt(subcat) + TEXT(", Cats : ") + FString::FromInt(cat) + TEXT(", \\t lines : ") + FString::FromInt(tline) + TEXT(", empty lines : ") + FString::FromInt(emptyline));
	}
}

void AViewerHUD::setCrosshairColor(FColor color) {
	CrosshairColor_ = color;
}

void AViewerHUD::setInfoBoxVisibility(bool v) {
	showInfoBox_ = v;
}

void AViewerHUD::setInGameCrosshairVisibility(bool v) {
	showCrosshair_ = v;
}

void AViewerHUD::setGameMenuVisibility(bool v) {
	showGameMenu_ = v;
}

bool AViewerHUD::getGameMenuVisibility() {
	return showGameMenu_;
}

void AViewerHUD::sendActorInfo(/* Parameter, by reference, in which it'll contain actor info */) {
	// Attribute of ViewerHUD = parameter

	// infoBoxLines_ = number of infos to show
}

