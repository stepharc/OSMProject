// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SSGameMenuWidget.h"
#include <map>
#include <string>
#include <vector>
#include "ViewerHUD.generated.h"

/**
 * 
 */
UCLASS()
class OSMPROJECT_API AViewerHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	FVector2D MouseLocation_;
	FColor CrosshairColor_;
	float CrosshairSize_;
	int infoBoxLines_;
	bool showInfoBox_;
	bool showCrosshair_;
	bool showGameMenu_;

	enum DrawStatus { NOT_DRAWED, TO_BE_UNDRAWED, TO_BE_DRAWED, DRAWED };
	std::map<std::string, std::vector<std::pair<std::string, DrawStatus>>> CatsSubcatsNodes_;
	std::map<std::string, std::vector<std::pair<std::string, DrawStatus>>> CatsSubcatsWays_;
	std::map<std::string, std::vector<std::pair<std::string, DrawStatus>>> CatsSubcatsRelations_;

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
	std::map<std::string, std::vector<std::pair<std::string, DrawStatus>>> getCatsSubcatsNodes();
	std::map<std::string, std::vector<std::pair<std::string, DrawStatus>>> getCatsSubcatsWays();
	std::map<std::string, std::vector<std::pair<std::string, DrawStatus>>> getCatsSubcatsRelations();
	void sendActorInfo();
	void BeginPlay();
};
