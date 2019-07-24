// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Collider.generated.h"

UCLASS()
class FIRSTPROJECT_API ACollider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere,Category="Mesh")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UColliderMovementComponent* MovementComponent;

	FORCEINLINE void SetMeshComponent(UStaticMeshComponent* Mesh) { MeshComponent = Mesh; }
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() { return MeshComponent; }

	FORCEINLINE void SetSphereComponent(USphereComponent* Sphere) { SphereComponent = Sphere; }
	FORCEINLINE USphereComponent* GetSphereComponent() { return SphereComponent; }

	FORCEINLINE void SetCameraComponent(UCameraComponent* InCamera) { Camera = InCamera; }
	FORCEINLINE UCameraComponent* GetCameraComponent() { return Camera; }

	FORCEINLINE void SetSpringArm(USpringArmComponent* InSpringArm) { SpringArm = InSpringArm; }
	FORCEINLINE USpringArmComponent* GetSpringArm() { return SpringArm; }

	//the pawn already have a GetMovementComponent , we Gonna override it
	virtual UPawnMovementComponent* GetMovementComponent()const;

private:
	void MoveForward(float Scale);
	void MoveRight(float Scale);

	void CameraPitch(float Scale);
	void CameraYaw(float Scale);

	FVector CurrentVelocity;
	FVector2D CameraInput;
};
