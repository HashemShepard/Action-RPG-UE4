// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	
	//inherted intializer
	virtual void NativeInitializeAnimation() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class APawn* Pawn; // for our character pawn

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class AMainCharacter* MainCharacter; // for our MainCharacter To Get functionality from it (Sprinting)

	//our own properties updater 
	UFUNCTION(BlueprintCallable,Category="AnimationProperties")
	void UpdateAnimationProperties();
};
