// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "OSMElementActor.generated.h"

UCLASS()
class OSMPROJECT_API AOSMElementActor : public AActor
{
	GENERATED_BODY()

private:
	USphereComponent* CollisionComp; // Sphere collision component, needs to be more general (cube, ...)
	
public:	
	// Sets default values for this actor's properties
	AOSMElementActor(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
