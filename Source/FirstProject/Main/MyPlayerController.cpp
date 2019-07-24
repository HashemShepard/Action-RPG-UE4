// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HudOverlayAsset) //make sure it's not null
	{
		//create the Hud Widget (just like create default subobject)
		HudOverlay = CreateWidget<UUserWidget>(this, HudOverlayAsset);
	}
	HudOverlay->AddToViewport(); //add to our viewport while playing
	HudOverlay->SetVisibility(ESlateVisibility::Visible); // set it to visible

	if (WEnemyHealthBar) //make sure it's not null
	{
		//create the Hud Widget (just like create default subobject)
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport(); //add to our viewport while playing
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden); // set it to visible
		}
		FVector2D Alignment(0.f, 0.f);
		EnemyHealthBar->SetAlignmentInViewport(Alignment);
	}
	
}

void AMyPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bIsEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible); // set it to visible
	}
}

void AMyPlayerController::RemoveEnemeyHealthBar()
{
	if (EnemyHealthBar)
	{
		bIsEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden); // set it to visible
	}
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (EnemyHealthBar)
	{
		FVector2D PostionInViewPort;
		ProjectWorldLocationToScreen(EnemyLocation, PostionInViewPort);
		PostionInViewPort.Y -= 90.f;
		FVector2D SizeInViewPort = FVector2D(200.f, 25.f);
		EnemyHealthBar->SetPositionInViewport(PostionInViewPort);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewPort);
	}
}
