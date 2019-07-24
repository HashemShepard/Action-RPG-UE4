// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameplayItems/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "GameplayActors/Enemy.h"
#include "MyPlayerController.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(64.f, 114.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 600.f; //Camera Follow Distance	
	SpringArm->bUsePawnControlRotation = true; //Rotate Arm based on controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); //Socket name to which the camera is attached to
	FollowCamera->bUsePawnControlRotation = false; //only follow the spring arm

	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	// the character it self wont change its rotation with the camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//make the character look at the same direction its moving
	GetCharacterMovement()->bOrientRotationToMovement=true;
	GetCharacterMovement()->RotationRate = FRotator(0,550,0);
	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = 0.1f;

	bIsShiftKeyPressed = false;
	bIsEKeyPressed = false;
	bIsLMBPressed = false;

	//initializing Player Stats
	Health = 100.f;
	MaxHealth = 100.f;
	Stamina = 100.f;
	MaxStamina = 100.f;
	Coins = 0;
	RunningSpeed = 650.f;
	SprintingSpeed = 950.f;
	StaminaDrainRate = 5.f;
	MinSprintStamina = 15.f;
	InterpSpeed = 15.f;
	bInterpToEnemy = false;
	bHasCombatTarget = false;
	//initializing Player Stats Enums
	MovementStatus = EMovementStatus::EMS_Running;
	StaminaStatus = EStaminaStatus::EMS_Normal;
}



// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	MyPlayerController = Cast<AMyPlayerController>(GetController());
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float DeltaStamina = StaminaDrainRate * DeltaTime;

	//Stamina Settings
	switch (StaminaStatus)
	{
	case EStaminaStatus::EMS_Normal:
		if (bIsShiftKeyPressed)
		{
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
			if (Stamina - DeltaStamina <= MinSprintStamina)
			{
				Stamina -= DeltaStamina;
				SetStaminaStatus(EStaminaStatus::EMS_BelowMinimum);
			}
			else
			{
				Stamina -= DeltaStamina;
			}
		}	
		else
		{
			if (!(Stamina + DeltaStamina > MaxStamina))
				Stamina += DeltaStamina;
			SetMovementStatus(EMovementStatus::EMS_Running	);
		}	
		break;
	case EStaminaStatus::EMS_BelowMinimum:
		if (bIsShiftKeyPressed)
		{
			if (Stamina - DeltaStamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::EMS_Exhausted);
				Stamina = 0.f;
				SetMovementStatus(EMovementStatus::EMS_Running);
			}
			else
			{
				Stamina -= DeltaStamina;
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}
		else
		{
			if (!(Stamina + DeltaStamina > MinSprintStamina))
			{
				SetStaminaStatus(EStaminaStatus::EMS_Normal);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}	
			SetMovementStatus(EMovementStatus::EMS_Running);
		}
		break;
	case EStaminaStatus::EMS_Exhausted:
		if (bIsShiftKeyPressed)
		{
			Stamina = 0.f;
		}
		else
		{
			SetStaminaStatus(EStaminaStatus::EMS_RecoverExhausted);
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Running);
		break;
	case EStaminaStatus::EMS_RecoverExhausted:
		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::EMS_Normal);
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Running);
		break;
	case EStaminaStatus::EMS_MAX:
		break;
	default:
		break;
	}

	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (MyPlayerController)
		{
			MyPlayerController->EnemyLocation = CombatTarget->GetActorLocation();
		}
		if (bInterpToEnemy)
		{
			FRotator InterpRotation = FMath::RInterpTo(
				GetActorRotation(), //starting Rotation
				GetLookAtYawRotation(CombatTarget->GetActorLocation()), //Ending Rotation
				DeltaTime,
				InterpSpeed);

			SetActorRotation(InterpRotation);
		}
		
	}

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);


	//moving
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",   this, &AMainCharacter::MoveRight);

	//camera mouse - the character class already inherit from the pawn a mouse function
	PlayerInputComponent->BindAxis("CameraPitch", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("CameraYaw",   this, &APawn::AddControllerYawInput);

	//camera keys
	PlayerInputComponent->BindAxis("TurnRate",   this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainCharacter::LookUpAtRate);

	//jumping and &ACharacter::Jump is an inhereted function
	PlayerInputComponent->BindAction("Jump", IE_Pressed,  this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Sprinting
	PlayerInputComponent->BindAction("Sprint", IE_Pressed,  this, &AMainCharacter::ShiftKeyPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::ShiftKeyReleased);

	//Interact
	PlayerInputComponent->BindAction("Interact", IE_Pressed,  this, &AMainCharacter::EKeyPressed);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AMainCharacter::EKeyReleased);

	//LMB
	PlayerInputComponent->BindAction("LMB", IE_Pressed,  this, &AMainCharacter::LMBPressed);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMainCharacter::LMBReleased);


}

void AMainCharacter::MoveForward(float Scale)
{
	// We want it to move with the camera and the camera depend on the spring arm and the spring arm takes rotation from controller
	if (Controller != nullptr && Scale != 0.f && !bIsAttacking) //check that controller is not null
	{
		//Get which way is forward
		const FRotator YawRotation = FRotator(0, Controller->GetControlRotation().Yaw,0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);//X Forward , Y Side , Z Up
		AddMovementInput(Direction, Scale);
	}
}

void AMainCharacter::MoveRight(float Scale)
{
	if (Controller!=nullptr && Scale != 0.f && !bIsAttacking) //check that controller is not null
	{
		//Get which way is side -> Right
		const FRotator YawRotation = FRotator(0, Controller->GetControlRotation().Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);//X Forward , Y Side , Z Up
		AddMovementInput(Direction, Scale);
	}
}


void AMainCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

FRotator AMainCharacter::GetLookAtYawRotation(FVector Target)
{
	//UKismetMathLibrary::FindLookAtRotation gives us a rotation between a starting point and ending point
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw = FRotator(0, LookAtRotation.Yaw, 0);
	return LookAtRotationYaw;
}

void AMainCharacter::ShiftKeyPressed(){	bIsShiftKeyPressed = true;}
void AMainCharacter::ShiftKeyReleased(){ bIsShiftKeyPressed = false;}

void AMainCharacter::EKeyPressed() 
{ 
	bIsEKeyPressed = true;
	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			ActiveOverlappingItem = nullptr;
		}
	}
}
void AMainCharacter::EKeyReleased() { bIsEKeyPressed = false; }

void AMainCharacter::LMBPressed() 
{ 
	bIsLMBPressed = true;
	if (EquippedWeapon)
	{ 
		Attack();
	}
		
}
void AMainCharacter::LMBReleased() { bIsLMBPressed = false; }

void AMainCharacter::SetWeapon(AWeapon* InWeapon)
{
	if (EquippedWeapon) { EquippedWeapon->Destroy(); } 
	EquippedWeapon = InWeapon;
}


void AMainCharacter::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	else if (MovementStatus == EMovementStatus::EMS_Running)
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
}



void AMainCharacter::Attack()
{
	if (!bIsAttacking)
	{
		bIsAttacking = true;
		SetInterpToEnemy(true);
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			int32 Section = FMath::RandRange(0, 1);
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
			
		}
	}
}

void AMainCharacter::FinishAttack()
{
	bIsAttacking = false;
	SetInterpToEnemy(false);
	if (bIsLMBPressed) //hold button attack (Opitional)
	{
		Attack();
	}
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	SetHealth(Health - DamageAmount);
	if (Health <= 0)
		Die();
	return DamageAmount;
}


void AMainCharacter::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{ 
			AnimInstance->Montage_Play(CombatMontage);
			AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
		}

}
