// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Public/TimerManager.h"


// Sets default values
AFloorSwitch::AFloorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsCharacterOnSwitch = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //Only used for spatial queries (raycasts, sweeps, and overlaps)
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);//ignore all
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);//response only to pawns by overlapping
	TriggerBox->SetBoxExtent(FVector(62, 62, 32));

	FloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor Switch"));
	FloorSwitch->SetupAttachment(GetRootComponent());

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::OnEndOverlap);

	InitialDoorLocation = Door->GetComponentLocation();
	InitialSwitchLocation= FloorSwitch->GetComponentLocation();
}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsCharacterOnSwitch) { bIsCharacterOnSwitch = true; }
	RaiseDoor();
	LowerFloorSwitch();
}

void AFloorSwitch::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsCharacterOnSwitch) { bIsCharacterOnSwitch = false; }
	GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitch::CloseDoor, SwitchTime);
}

void AFloorSwitch::UpdateObjectLocation(UStaticMeshComponent* Object, float Z, FString Name)
{
	if(Name=="Door")
		Object->SetWorldLocation(InitialDoorLocation + FVector(0, 0, Z));
	if(Name=="Switch")
		Object->SetWorldLocation(InitialSwitchLocation + FVector(0, 0, Z));
}

void AFloorSwitch::CloseDoor()
{
	if (!bIsCharacterOnSwitch) 
	{
		LowerDoor();
		RaiseFloorSwitch();
	}
}



