// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class FIRSTPROJECT_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

	UPROPERTY(EditAnywhere, Category = "Platform")
	FVector StartPoint;

	//meta -> visualization widget to manipulate at editor
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"), Category = "Platform")
	FVector EndPoint;

	//Mesh For the platform
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	class UStaticMeshComponent* Mesh;

	//platform moving speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float InterpTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	bool bIsInterping;

	FTimerHandle InterpTimer;

	float DistBetweenStartAndCurrentLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleInterping();

	void SwapVectors(FVector &a, FVector &b);
};
