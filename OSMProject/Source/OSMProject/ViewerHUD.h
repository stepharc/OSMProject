// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include <map>
#include <string>
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

	std::map<std::string, std::pair<std::string, bool>> CatsSubcatsNodes_;
	std::map<std::string, std::pair<std::string, bool>> CatsSubcatsWays_;
	std::map<std::string, std::pair<std::string, bool>> CatsSubcatsRelations_;

	void drawCrosshair(int32 x, int32 y);
	void drawInfoBoxAndContent();
	void drawGameMenu();
	void readOSMArborescenceFile();

public:
	AViewerHUD();
	virtual void DrawHUD() override;
	void setCrosshairColor(FColor color);
	void setInfoBoxVisibility(bool v);
	void setInGameCrosshairVisibility(bool v);
	void setGameMenuVisibility(bool v);
	bool getGameMenuVisibility();
	void sendActorInfo();
};
