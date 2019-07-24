// Fill out your copyright notice in the Description page of Project Settings.


#include "Spider.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


ASpider::ASpider()
{
	RightEnemyCombatBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Front_Right_Knee"));
	LeftEnemyCombatBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Front_Left_Knee"));
}

void ASpider::BeginPlay()
{
	Super::BeginPlay();


	/*
	 * Setting Up Collision
	 */
	RightEnemyCombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightEnemyCombatBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightEnemyCombatBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightEnemyCombatBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	/*
	 * Setting Up Collision
	 */
	LeftEnemyCombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftEnemyCombatBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftEnemyCombatBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftEnemyCombatBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

}

void ASpider::ActivateCollison()
{
	RightEnemyCombatBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftEnemyCombatBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (SwingSound)
		UGameplayStatics::PlaySound2D(this, SwingSound);
}

void ASpider::deActivateCollison()
{
	RightEnemyCombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftEnemyCombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

