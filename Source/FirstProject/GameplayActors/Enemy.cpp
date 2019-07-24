// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Classes/AIController.h"
#include "Main/MainCharacter.h"
#include "Main/MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bIsOverlappingCombatSphere = false;

	RightEnemyCombatBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightEnemyCombatBox"));
	LeftEnemyCombatBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftEnemyCombatBox"));

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(210.f);

	Health = 100.f;
	MaxHealth = 100.f;
	Damage = 10.f;
	AttackMaxTime = 2.f;
	AttackMinTime = 0.5f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAgroRadiusOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnAgroRadiusEndOverlap);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnCombatRadiusOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnCombatRadiusEndOverlap);

	LeftEnemyCombatBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnLeftOverlapBegin);
	LeftEnemyCombatBox->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnLeftEndOverlap);
	RightEnemyCombatBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnRightOverlapBegin);
	RightEnemyCombatBox->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnRightEndOverlap);

	if(GetController())
		AiController = Cast<AAIController>(GetController());

	 StartingLocation = GetActorLocation();
	 StartingRotation = GetActorRotation();

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::MoveToTarget(AMainCharacter* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
	if (AiController)
	{
		//AiController->MoveToActor(Target,10,false);
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(30.f);

		FNavPathSharedPtr OutPath;
		AiController->MoveTo(MoveRequest, &OutPath);
	}
}

void AEnemy::OnAgroRadiusOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharacter* Target = Cast<AMainCharacter>(OtherActor);
		if(Target)
		 MoveToTarget(Target);
	}
}

void AEnemy::OnAgroRadiusEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
		if (MainCharacter)
		{
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			AiController->MoveToLocation(StartingLocation,5.f);
			SetActorRotation(StartingRotation);
		}
	}
}

void AEnemy::OnCombatRadiusOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsOverlappingCombatSphere = true;
	if (OtherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
		if (MainCharacter)
		{
			MainCharacter->SetCombatTarget(this);
			if (MainCharacter->MyPlayerController)
			{
				MainCharacter->MyPlayerController->DisplayEnemyHealthBar();
			}
			MainCharacter->SetHasCombatTarget(true);
			bIsOverlappingCombatSphere = true;
			Attack();
		}		
	}
}

void AEnemy::OnCombatRadiusEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsOverlappingCombatSphere = false;
	if (OtherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
		if (MainCharacter)
		{
			if (MainCharacter->CombatTarget)
			{
				MainCharacter->SetCombatTarget(nullptr);
				MainCharacter->SetHasCombatTarget(false);
			}
			if (MainCharacter->MyPlayerController)
			{
				MainCharacter->MyPlayerController->RemoveEnemeyHealthBar();
			}
			bIsOverlappingCombatSphere = false;
			GetWorldTimerManager().ClearTimer(AttackTimer);

			if(EnemyMovementStatus!= EEnemyMovementStatus::EMS_Attacking)
				MoveToTarget(MainCharacter);
		}
	}
}

void AEnemy::OnRightOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
		{
			if (MainCharacter)
			{
				if (MainCharacter->HitParticles)
				{
					//const USkeletalMeshSocket* ParticleSocket = SkeletalMesh->GetSocketByName("ParticleSocket"); ParticleSocket->GetSocketLocation(SkeletalMesh)
					//if(ParticleSocket) (Course Method)
					
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MainCharacter->HitParticles, MainCharacter->GetActorLocation(), FRotator(0.f), true);
					
					if (MainCharacter->HitSound)
						UGameplayStatics::PlaySound2D(this, MainCharacter->HitSound);
					
					//Damaging The Target
					if (DamageTypeClass)
						UGameplayStatics::ApplyDamage(MainCharacter, Damage/2, AiController, this, DamageTypeClass);
				}
			}

		}
	}
}

void AEnemy::OnRightEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


void AEnemy::OnLeftOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
		{
			if (MainCharacter)
			{
				{
					//Damaging The Target
					if (DamageTypeClass)
						UGameplayStatics::ApplyDamage(MainCharacter, Damage/2, AiController, this, DamageTypeClass);
				}
			}

		}
	}
}

void AEnemy::OnLeftEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}



void AEnemy::Attack()
{
	/*if (AiController)
	{
		AiController->StopMovement();
		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
	}*/
	if (!bIsAttacking)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			int32 Section = FMath::RandRange(0, 1);
			UE_LOG(LogTemp, Warning, TEXT("ATTACK"));
			switch (Section)
			{
			case 0:
				AnimInstance->Montage_Play(CombatMontage, 2.0f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				break;
			case 1:
				AnimInstance->Montage_Play(CombatMontage, 2.0f);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
				break;
			}
			
			
			bIsAttacking = true;
		}

		/*if (SwingSound)
			UGameplayStatics::PlaySound2D(this, SwingSound);*/
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	SetHealth(Health - DamageAmount);
	if (Health <= 0)
		Die();
	return DamageAmount;
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(CombatMontage);
			AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Disappear, 1.25f);
		}
		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);
		LeftEnemyCombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RightEnemyCombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AEnemy::AttackEnd()
{
	bIsAttacking = false;
	if (bIsOverlappingCombatSphere)
	{
		float attackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, attackTime);
	
	}
		
}

float AEnemy::GetHealthPercentage()
{
	return Health/MaxHealth;
}

void AEnemy::Disappear()
{
	Destroy();
}
/*function bool alive()
return getmovementstatus != ems_dead; 
then add the function alive as a check in all if statements 
*/