// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus :uint8
{
	//When we have a uenum property BlueprintType we have to give the enum members a displayname
	EMS_Running   UMETA(DisplayName="Running"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),

	EMS_MAX       UMETA(DisplayName = "MAX")//just a default 
};

UENUM(BlueprintType)
enum class EStaminaStatus :uint8
{
	//When we have a uenum property BlueprintType we have to give the enum members a displayname
	EMS_Normal           UMETA(DisplayName = "Normal"),
	EMS_BelowMinimum     UMETA(DisplayName = "BelowMinimum"),
	EMS_Exhausted        UMETA(DisplayName = "Exhausted"),
	EMS_RecoverExhausted UMETA(DisplayName = "RecoverExhausted"),

	EMS_MAX              UMETA(DisplayName = "MAX")
};

UCLASS()
class FIRSTPROJECT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	//already inherits a skeletal mesh and a capsule as (ROOT) from ACharacter , we dont need to create one

	TArray<FVector> PickUpLocations;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMyPlayerController* MyPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	void SetMovementStatus(EMovementStatus Status);

	/** a Follow Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera; //accessible only in the current blueprint (meta)

	/** Position the Camera behind the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm; //CameraBoom in course

	/** Base Turn rates for camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	/*setting up attacking animations*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Sound")
	class USoundCue* HitSound;

	/*
	*
	*PLAYER STATS:-
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	float MaxStamina;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	float Stamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float StaminaDrainRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float MinSprintStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	int32 Coins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float RunningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float SprintingSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
	bool bIsAttacking;
	UFUNCTION(BlueprintCallable)
	void FinishAttack();

	float InterpSpeed;
	bool bInterpToEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
	class AEnemy* CombatTarget; //Set at the enemy cpp file
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	bool bHasCombatTarget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	FVector CombatTargetLocation;

	

	/*
	*
	*PLAYER Items:-
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Items")
	class AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Items")
	class AItem* ActiveOverlappingItem;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void Die();
	void Attack();
	void MoveForward(float Scale);
	void MoveRight(float Scale);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** Called Via input to turn/LookUp at given rate
	*@param Rate this is a normalized rate 
	*/
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	FRotator GetLookAtYawRotation(FVector Target);

	/** Setting Sprinting
	*Same as using it in the Blueprint
	*/
	bool bIsShiftKeyPressed;
	void ShiftKeyPressed();
	void ShiftKeyReleased();

	/** Setting Interacting
	*Same as using it in the Blueprint
	*/
	bool bIsEKeyPressed;
	void EKeyPressed();
	void EKeyReleased();

	/** Setting LMB for attacking
	*Same as using it in the Blueprint
	*/
	bool bIsLMBPressed;
	void LMBPressed();
	void LMBReleased();


	FORCEINLINE void SetCameraComponent(UCameraComponent* InCamera) { FollowCamera = InCamera; }
	FORCEINLINE UCameraComponent* GetCameraComponent() { return FollowCamera; }

	FORCEINLINE void SetSpringArm(USpringArmComponent* InSpringArm) { SpringArm = InSpringArm; }
	FORCEINLINE USpringArmComponent* GetSpringArm() { return SpringArm; }

	FORCEINLINE void  SetHealth(float NewHealth) { Health = NewHealth; if (Health <= 0) Die(); }
	FORCEINLINE float GetHealth()   { return Health; }

	FORCEINLINE void  SetStamina(float NewStamina) { Stamina = NewStamina; }
	FORCEINLINE float GetStamina() { return Stamina; }

	FORCEINLINE void  SetCoins(float NewCoins) { Coins = NewCoins;}
	FORCEINLINE float GetCoins() { return Coins; }

	FORCEINLINE void  SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }
	FORCEINLINE EStaminaStatus  GetStaminaStatus() { return StaminaStatus ; }

	void SetWeapon(AWeapon* InWeapon);
	FORCEINLINE AWeapon* GetWeapon() { return EquippedWeapon ; }

	FORCEINLINE void  SetActiveOverlappingItem(AItem* Item) { ActiveOverlappingItem = Item; }
	FORCEINLINE AItem* GetActiveOverlappingItem() { return ActiveOverlappingItem ; }

	FORCEINLINE void SetInterpToEnemy(bool Interp) { bInterpToEnemy = Interp; }
	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTarget = Target; }

	FORCEINLINE void  SetHasCombatTarget(bool HasCombatTarget) { bHasCombatTarget = HasCombatTarget; }
	FORCEINLINE float GetHasCombatTarget() { return bHasCombatTarget; }
	
};
