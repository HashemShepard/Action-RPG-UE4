// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloater::AFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomStaticMesh"));
	InitialLocation = FVector(0.f);
	PlacedLocation = FVector(0.f);
	WorldOrigin = FVector(0.f);
	InitialDirection = FVector(0.f);
	bShouldFloat = false;
	bIntializeFloaterLocations = false;
	RunningTime = 0.f;
	Amplitude = TimeScale = Phase = 1.f;

}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();

	float InitialX = FMath::FRandRange(-500,500);
	float InitialY = FMath::FRandRange(-500, 500);
	float InitialZ = FMath::FRandRange(-500, 500);

	InitialLocation = FVector(InitialX, InitialY, InitialZ);

	PlacedLocation = GetActorLocation();

	if(bIntializeFloaterLocations)
	SetActorLocation(InitialLocation);

}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
	if (bShouldFloat)
	{
		FHitResult HitResult;
		FVector NewLocation = GetActorLocation();
		NewLocation.Z += Amplitude * FMath::Sin(TimeScale * RunningTime + Phase);
		NewLocation.X += 5;

		SetActorLocation(NewLocation);
	}
}

