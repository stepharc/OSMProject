// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
//#include "osmPart/osmPart/osmPart/OsmWithreadosm.h"
#include "OSMProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class OSMPROJECT_API AOSMProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
		UWorld* World_;
		enum osm_library_choice {READOSM, OSMIUM};
		osm_library_choice osmLibChoice_;

public:
		AOSMProjectGameModeBase();
		virtual void PostLogin(APlayerController* NewPlayer);
		bool SpawnNode(FVector location);
		//void testReadosm();
};
