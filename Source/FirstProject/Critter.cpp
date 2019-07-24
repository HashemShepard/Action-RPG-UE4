// Fill out your copyright notice in the Description page of Project Settings.


#include "Critter.h"
//#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

// Sets default values
ACritter::ACritter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentVelocity = FVector(0.f);
	MaxSpeed = 100.f;

	//We dont need to create a Root Component variable as it's inhereted from Actor
	RootComponent= CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	///*Course:-*/  MeshComponent->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	///*Course:-*/ Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector(-300.f, 0.f, 300.f));
	Camera->SetRelativeRotation(FRotator(-45.f, 0, 0.f));

	//AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ACritter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACritter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;
	SetActorLocation(NewLocation);

}

// Called to bind functionality to input
void ACritter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//The Axis Scale is passed To the Assigned Function
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&ACritter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACritter::MoveRight);

}

void ACritter::MoveForward(float Scale)
{
	//The Value Scale is passed from the BindAxis call
	CurrentVelocity.Y = FMath::Clamp(Scale, -1.f, 1.f)*MaxSpeed;
}

void ACritter::MoveRight(float Scale)
{
	//The Value Scale is passed from the BindAxis call
	CurrentVelocity.X = FMath::Clamp(Scale, -1.f, 1.f) * MaxSpeed;
}

