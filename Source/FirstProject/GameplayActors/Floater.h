// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

UCLASS()//inherets blueprintable from Actor Class
class FIRSTPROJECT_API AFloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloater();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite ,Category="ActorMeshComponents")
	UStaticMeshComponent* StaticMesh;

	//Location of Actor On Begin Play
	UPROPERTY(EditInstanceOnly , BlueprintReadWrite,Category = "FloaterVariable")
	FVector InitialLocation;

	//Location of Actor when dragged to world
	UPROPERTY(VisibleInstanceOnly , BlueprintReadWrite, Category = "FloaterVariable")
	FVector PlacedLocation;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "FloaterVariable")
	FVector WorldOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = "FloaterVariable")
	FVector InitialDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVariable")
	bool bShouldFloat;

	UPROPERTY(EditDefaultsOnly, Category = "FloaterVariable")
	bool bIntializeFloaterLocations;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float RunningTime;

	//The Sign | is used to create a sub category
	UPROPERTY(EditAnywhere, Category = "FloaterVariable | Wave Parameters")
	float Amplitude;
	UPROPERTY(EditAnywhere, Category = "FloaterVariable | Wave Parameters")
	float TimeScale; 
	UPROPERTY(EditAnywhere, Category = "FloaterVariable | Wave Parameters")
	float Phase;
};
