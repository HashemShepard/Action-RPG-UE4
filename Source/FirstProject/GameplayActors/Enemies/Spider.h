// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayActors/Enemy.h"
#include "Spider.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API ASpider : public AEnemy
{
	GENERATED_BODY()

public:
	ASpider();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		void ActivateCollison();
	UFUNCTION(BlueprintCallable)
		void deActivateCollison();
	
};
