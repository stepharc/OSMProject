// Fill out your copyright notice in the Description page of Project Settings.

#include "OSMElementActor.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine/GameEngine.h"

// Sets default values
AOSMElementActor::AOSMElementActor(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	RootComponent = Mesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("/Game/Sphere"));
	if (StaticMesh.Succeeded())
	{
		Mesh->SetStaticMesh(StaticMesh.Object);
		Mesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("No asset loaded."));
	}
	Mesh->GetCollisionEnabled();
}

// Called when the game starts or when spawned
void AOSMElementActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AOSMElementActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

