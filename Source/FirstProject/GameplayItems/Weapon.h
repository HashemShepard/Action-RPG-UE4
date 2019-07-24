// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayMechanics/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWeaponState :uint8
{
	//When we have a uenum property BlueprintType we have to give the enum members a displayname
	EWS_PickUp   UMETA(DisplayName = "PickUp"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),

	EWS_MAX       UMETA(DisplayName = "default")//just a default 
};

UCLASS()
class FIRSTPROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	AWeapon();
	
	EWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeaponCombatBox")
	class UBoxComponent* WeaponCombatBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particle")
	bool bEnableWeaponParitcls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
	class USoundCue* EquppingWeaponSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Damage")
	float WeaponDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
    USoundCue* SwingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
		TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
		AController* WInstigator;

protected:
	virtual void BeginPlay() override;

public:
	//we used ufunction here becase the functions name are diffrent from that inhereted from item
	UFUNCTION()
		virtual void OnWeaponCombatBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void OnWeaponCombatBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Equip(class AMainCharacter* MainCharacter);


	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }
	FORCEINLINE void  SetWeaponState(EWeaponState NewWeaponState) { WeaponState = NewWeaponState; }

	UFUNCTION(BlueprintCallable)
		void ActivateCollison();
	UFUNCTION(BlueprintCallable)
		void deActivateCollison();
	UFUNCTION(BlueprintCallable)
		void PlaySound();

	
}; 
