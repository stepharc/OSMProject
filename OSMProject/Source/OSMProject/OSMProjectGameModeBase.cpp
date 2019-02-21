// Fill out your copyright notice in the Description page of Project Settings.

#include "OSMProjectGameModeBase.h"
#include "ViewerHUD.h"
#include "PlayerPawn.h"

AOSMProjectGameModeBase::AOSMProjectGameModeBase() {
	HUDClass = AViewerHUD::StaticClass();
	DefaultPawnClass = APlayerPawn::StaticClass();
}