// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ViewerHUD.generated.h"

/**
 * 
 */
UCLASS()
class OSMPROJECT_API AViewerHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	FColor CrosshairColor_;
	float CrosshairSize_;
	int infoBoxLines_;
	bool showInfoBox_;

public:
	AViewerHUD();
	virtual void DrawHUD() override;
	void setCrosshairColor(FColor color);
	void setInfoBoxVisibility(bool v);
	void sendActorInfo();
};
