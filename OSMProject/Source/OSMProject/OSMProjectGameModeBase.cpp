// Fill out your copyright notice in the Description page of Project Settings.

#include "OSMProjectGameModeBase.h"
#include "Engine/GameEngine.h"
#include "ViewerHUD.h"
#include "PlayerPawn.h"
#include "OSMElementActor.h"

AOSMProjectGameModeBase::AOSMProjectGameModeBase() {
	HUDClass = AViewerHUD::StaticClass();
	DefaultPawnClass = APlayerPawn::StaticClass();
	World_ = GetWorld();
	osmLibChoice_ = OSMIUM;
}

bool AOSMProjectGameModeBase::SpawnNode(FVector location) {
	bool spawned = true;
	if (World_)
	{
		const FRotator tempRotation = FRotator(0.);
		AOSMElementActor* const OSMElement = World_->SpawnActor<AOSMElementActor>(AOSMElementActor::StaticClass(), location, tempRotation);
		if (OSMElement != NULL) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("OSMElement spawned."));
	}
	else {
		spawned = false;
	}
	return spawned;
}

/*void AOSMProjectGameModeBase::testReadosm() {
	OsmWithreadosm o;
	std::vector<std::string> filtre;
	filtre.push_back("");
	filtre.push_back("");
	std::vector<Nodereadosm> Ns;
	std::vector<Wayreadosm> Ws;
	std::vector<Relationreadosm> Rs;
	o.initWithCat(filtre, filtre, filtre);

	Ns = o.getNodes();
	Ws = o.getWays();
	Rs = o.getRelations();
}*/

void AOSMProjectGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Logged."));
	//testReadosm();
	bool b = SpawnNode(FVector(1000.f, 100.f, 150.f));
}