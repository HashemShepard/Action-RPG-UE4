// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus :uint8
{
	//When we have a uenum property BlueprintType we have to give the enum members a displayname
	EMS_Idle         UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget UMETA(DisplayName = "MoveToTarget"),
	EMS_Attacking    UMETA(DisplayName = "Attacking"),
	EMS_Dead         UMETA(DisplayName = "Dead"),
	EMS_MAX          UMETA(DisplayName = "MAX")//just a default 
};

UCLASS()
class FIRSTPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	FVector StartingLocation;
	FRotator StartingRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enums")
	EEnemyMovementStatus EnemyMovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ai")
	class USphereComponent* AgroSphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ai")
	USphereComponent* CombatSphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ai")
	class AAIController* AiController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ai")
	bool bIsOverlappingCombatSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ai")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Sound")
	class USoundCue* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Sound")
    USoundCue* SwingSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyCombatBox")
	class UBoxComponent* LeftEnemyCombatBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyCombatBox")
	class UBoxComponent* RightEnemyCombatBox;



	/*setting up attacking animations*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
		class UAnimMontage* CombatMontage;

	/*
	*
	*Enemy STATS:-
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float AttackMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float AttackMaxTime;
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	TSubclassOf<UDamageType> DamageTypeClass;


	/*
	*
	*Enemy Items:-
	*/
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMainCharacter* Target);

	FORCEINLINE void  SetEnemyMovementStatus(EEnemyMovementStatus Status) { EnemyMovementStatus = Status; }
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() { return EnemyMovementStatus; }

	UFUNCTION()
	virtual void OnAgroRadiusOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnAgroRadiusEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	virtual void OnCombatRadiusOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnCombatRadiusEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnLeftOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnLeftEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	virtual void OnRightOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnRightEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	FORCEINLINE void  SetHealth(float NewHealth) { Health = NewHealth; if (Health <= 0) Die(); }
	FORCEINLINE float GetHealth() { return Health; }
	
	 

	void Attack();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void Die();
	void Disappear();
	UFUNCTION(BlueprintCallable)
	void DeathEnd();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	bool bIsAttacking;
	UFUNCTION(BlueprintCallable)
	float GetHealthPercentage();
};
