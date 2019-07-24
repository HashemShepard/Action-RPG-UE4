// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Main/MainCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "GameplayActors/Enemy.h"


AWeapon::AWeapon()
{
	WeaponCombatBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCombatBox"));
	WeaponCombatBox->SetupAttachment(GetRootComponent());

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	bEnableWeaponParitcls = false;

	WeaponState = EWeaponState::EWS_PickUp;
	WeaponDamage = 25;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	/*we made these two for the combat box only and not for the collision sphere
	 *because the sphere functions are inhereted from the item class but these combat 
	 *two functions are exclusive to the weapon , that's why we also added UFUNCTION
	*/
	WeaponCombatBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponCombatBoxOverlapBegin);
	WeaponCombatBox->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnWeaponCombatBoxEndOverlap);

	/*
	* Setting Up Collision
	*/
	WeaponCombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCombatBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	WeaponCombatBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponCombatBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (WeaponState==EWeaponState::EWS_PickUp && OtherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
		if (MainCharacter)
		{
			MainCharacter->SetActiveOverlappingItem(this);
		}
	}
}

void AWeapon::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
		if (MainCharacter)
		{
			MainCharacter->SetActiveOverlappingItem(nullptr);
		}
	}
}

void AWeapon::Equip(AMainCharacter* MainCharacter)
{
	if (MainCharacter)
	{
		WInstigator = MainCharacter->GetController();
		// to make the camera ignore weapon collision (camera affected only by the actor it self
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

		// to make the camera ignore weapon collision with our character (camera affected only by the actor it self
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		//better to avoid collisions
		SkeletalMesh->SetSimulatePhysics(false);

		if (!bEnableWeaponParitcls)
			IdleParticleSystem->Deactivate();

		//get the socket we will attach to 
		const USkeletalMeshSocket* WeaponSocket = MainCharacter->GetMesh()->GetSocketByName("Weapon");
		if (WeaponSocket != nullptr) // if it's valid
		{
			//then attach
			WeaponSocket->AttachActor(this, MainCharacter->GetMesh()); 
			//stop rotating weapon when equiped
			bIsRotating = false; //inherted
			MainCharacter->SetWeapon(this);
			MainCharacter->SetActiveOverlappingItem(nullptr);

			//play a sound for equiping 
			if (EquppingWeaponSound)
				UGameplayStatics::PlaySound2D(this, EquppingWeaponSound);
		}
	}
}

void AWeapon::ActivateCollison()
{
	WeaponCombatBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::deActivateCollison()
{
	WeaponCombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::PlaySound()
{
	if (SwingSound)
	{
		UGameplayStatics::PlaySound2D(this,SwingSound);
	}
}


void AWeapon::OnWeaponCombatBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		{
			if (Enemy)
			{
				if (Enemy->HitParticles)
				{
					//const USkeletalMeshSocket* ParticleSocket = SkeletalMesh->GetSocketByName("ParticleSocket"); ParticleSocket->GetSocketLocation(SkeletalMesh)
					//if(ParticleSocket)
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticles, Enemy->GetActorLocation(), FRotator(0.f), true);
					if (Enemy->HitSound)
					{
						UGameplayStatics::PlaySound2D(this, Enemy->HitSound);
					}
					if (DamageTypeClass) // we can use Instigator as course instead of GetInstigatorController
						UGameplayStatics::ApplyDamage(Enemy, WeaponDamage,GetInstigatorController(), this, DamageTypeClass);
				}
			}

		}
	}
}

void AWeapon::OnWeaponCombatBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}



