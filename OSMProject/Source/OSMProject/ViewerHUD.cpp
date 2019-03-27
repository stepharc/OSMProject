// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewerHUD.h"
#include "SSGameMenuWidget.h"
#include "Engine/Canvas.h"
#include "Engine/GameEngine.h"
#include "Public/CanvasItem.h"
#include "Widgets/SWeakWidget.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include <fstream>
#include <algorithm>

AViewerHUD::AViewerHUD() {
	CrosshairColor_ = FColor::White;
	CrosshairSize_ = 15;
	showInfoBox_ = false;
	showCrosshair_ = true;
	showGameMenu_ = false;
	infoBoxLines_ = 1;
	readOSMArborescenceFile();
}

void AViewerHUD::BeginPlay() {
	GMWidget = SNew(SSGameMenuWidget).OwnerHUDArg(this);
	GEngine->GameViewport->AddViewportWidgetContent(
		SNew(SWeakWidget)
		.PossiblyNullContent(GMWidget.ToSharedRef())
	);
	GMWidget->SetVisibility(EVisibility::Hidden);
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
	else {
		if(GMWidget->GetVisibility() != EVisibility::Hidden) GMWidget->SetVisibility(EVisibility::Hidden);
	}

	// Crosshair
	if (showCrosshair_) {
		// Find the center of our canvas.
		FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
		drawCrosshair(Center.X, Center.Y);
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
		FLinearColor(0.2, 0.2, 0.2, 1),
		0,
		0,
		Canvas->ClipX,
		Canvas->ClipY
	);

	//Menu Content
	if (GMWidget->GetVisibility() != EVisibility::Visible) GMWidget->SetVisibility(EVisibility::Visible);

	DrawText(TEXT("Press M to quit menu."), FLinearColor::Black, Canvas->ClipX - 150, Canvas->ClipY - 40);
}

void AViewerHUD::addCatSubcatToHashmap(std::string category, std::string subcategory, std::map<std::string, std::vector<std::pair<std::string, DrawStatus>>>& hashmap) {
	auto it = hashmap.find(category);
	if (it != hashmap.end()) {
		it->second.push_back(std::pair<std::string, DrawStatus>(subcategory, NOT_DRAWED));
	}
	else {
		std::vector<std::pair<std::string, DrawStatus>> v { std::pair<std::string, DrawStatus>(subcategory, NOT_DRAWED) };
		hashmap.insert(std::pair<std::string, std::vector<std::pair<std::string, DrawStatus>>>(category, v));
	}
}

void AViewerHUD::readOSMArborescenceFile() {
	std::string filePath = "D:\\UnrealProjects\\OSMProject\\SourceOSM\\arbo.txt";
	//std::string filePath = ".\\..\\..\\SourceOSM\\arbo.txt";
	std::string line, category, subcategory, osmentity;
	std::map<std::string, std::vector<std::pair<std::string, DrawStatus>>> hashmap;

	std::ifstream filestream(filePath, std::ios::in);

	if (filestream)
	{
		while (getline(filestream, line)) {
			if (line.find("\t\t") != std::string::npos) {
				//not a blank line, subcategory line
				if (line != "\t\t") {
					subcategory = line.substr(2, std::string::npos);
					//Initialize category/subcategory in hashmap.
					if (osmentity.compare("Way") == 0) {
						addCatSubcatToHashmap(category, subcategory, CatsSubcatsWays_);
					}
					else {
						if (osmentity.compare("Node") == 0) {
							addCatSubcatToHashmap(category, subcategory, CatsSubcatsNodes_);
						}
						else {
							addCatSubcatToHashmap(category, subcategory, CatsSubcatsRelations_);
						}
					}
				}
			}
			else {
				if (line.find("\t") != std::string::npos) { //Which hashmap ? (way/node/relation)
					osmentity = line.substr(2, line.find(']') - 2);
				}
				else { //category line.
					category = line.substr(0, line.find(" ["));
				}
			}
		}
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

std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>> AViewerHUD::getCatsSubcatsNodes() {
	return CatsSubcatsNodes_;
}

std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>> AViewerHUD::getCatsSubcatsWays() {
	return CatsSubcatsWays_;
}

std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>> AViewerHUD::getCatsSubcatsRelations() {
	return CatsSubcatsRelations_;
}

void AViewerHUD::requestShowOSMElement(std::string category, std::string subcategory, std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>>& hashmap) {
	auto it = hashmap.find(category);
	auto itbis = std::find_if(it->second.begin(), it->second.end(), [&subcategory](const std::pair<std::string, AViewerHUD::DrawStatus> element) { return (element.first == subcategory); });
	switch (itbis->second) {
		case NOT_DRAWED:
			itbis->second = TO_BE_DRAWED;
			break;
		case TO_BE_UNDRAWED:
			itbis->second = DRAWED;
			break;
		case TO_BE_DRAWED:
			break;
		case DRAWED:
			break;
	}
}

void AViewerHUD::requestHideOSMElement(std::string category, std::string subcategory, std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>>& hashmap) {
	auto it = hashmap.find(category);
	auto itbis = std::find_if(it->second.begin(), it->second.end(), [&subcategory](const std::pair<std::string, AViewerHUD::DrawStatus> element) { return (element.first == subcategory); });
	switch (itbis->second) {
		case NOT_DRAWED:
			break;
		case TO_BE_UNDRAWED:
			break;
		case TO_BE_DRAWED:
			itbis->second = NOT_DRAWED;
			break;
		case DRAWED:
			itbis->second = TO_BE_UNDRAWED;
			break;
	}
}

void AViewerHUD::updateOSMElementsInWorld() {
	int drawed = 0, undrawed = 0;
	
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Drawed : " + FString::FromInt(drawed) + " / Undrawed : " + FString::FromInt(undrawed));
}

void AViewerHUD::sendActorInfo(/* Parameter, by reference, in which it'll contain actor info */) {
	// Attribute of ViewerHUD = parameter

	// infoBoxLines_ = number of infos to show
}

