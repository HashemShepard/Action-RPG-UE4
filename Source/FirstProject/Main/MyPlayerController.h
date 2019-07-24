// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//Reference to the UMG asset in the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget >HudOverlayAsset;
	
	//Variable to Hold the widget after creation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class UUserWidget* HudOverlay;

	//Reference to the UMG asset in the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget >WEnemyHealthBar;

	//Variable to Hold the widget after creation
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	class UUserWidget* EnemyHealthBar;

	bool bIsEnemyHealthBarVisible;
	void DisplayEnemyHealthBar();
	void RemoveEnemeyHealthBar();

	virtual void Tick(float DeltaTime) override;
	FVector EnemyLocation;

protected:
	virtual void BeginPlay()override;
};
