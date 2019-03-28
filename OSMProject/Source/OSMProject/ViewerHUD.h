// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include <map>
#include <string>
#include <vector>
#include "ViewerHUD.generated.h"

class SSGameMenuWidget;

/**
 * 
 */
UCLASS()
class OSMPROJECT_API AViewerHUD : public AHUD
{
	GENERATED_BODY()

public:
	enum DrawStatus { NOT_DRAWED, TO_BE_UNDRAWED, TO_BE_DRAWED, DRAWED };

private:
	FColor CrosshairColor_;
	float CrosshairSize_;
	int infoBoxLines_;
	bool showInfoBox_;
	bool showCrosshair_;
	bool showGameMenu_;

	std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>> CatsSubcatsNodes_;
	std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>> CatsSubcatsWays_;
	std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>> CatsSubcatsRelations_;

	void drawCrosshair(int32 x, int32 y);
	void drawInfoBoxAndContent();
	void drawGameMenu();
	void addCatSubcatToHashmap(std::string category, std::string subcategory, std::map<std::string, std::vector<std::pair<std::string, DrawStatus>>>& hashmap);
	void readOSMArborescenceFile();

public:
	TSharedPtr<SSGameMenuWidget> GMWidget;

	AViewerHUD();
	virtual void DrawHUD() override;
	void setCrosshairColor(FColor color);
	void setInfoBoxVisibility(bool v);
	void setInGameCrosshairVisibility(bool v);
	void setGameMenuVisibility(bool v);
	bool getGameMenuVisibility();
	std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>> getCatsSubcatsNodes();
	std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>> getCatsSubcatsWays();
	std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>> getCatsSubcatsRelations();
	void requestShowOSMElement(std::string category, std::string subcategory, std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>>& hashmap);
	void requestHideOSMElement(std::string category, std::string subcategory, std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>>& hashmap);
	void updateOSMElementsInWorld();
	void sendActorInfo();
	void BeginPlay();
};
