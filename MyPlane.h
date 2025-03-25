// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "HoopsPawn.h"
#include "HoopsGameMode.h"
#include "MyPlane.generated.h"

class UArrowComponent;
class USpringArmComponent;
class UCameraComponent;

class UInputMappingContext;
class UInputAction;

class UBoxComponent;

UCLASS()
class PLANEGAME_API AMyPlane : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PlaneMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;
	// ~Components


	// Inputs
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* IMC_Player;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_ChangeAcceleration;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Pitch;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Roll;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Yaw;
	// ~Inputs


	// Movement Properties
	UPROPERTY(EditAnywhere, Category = Movement)
	float MaxThrust;

	UPROPERTY(EditAnywhere, Category = Movement)
	float MinThrust;

	UPROPERTY(EditAnywhere, Category = Movement)
	float startingVol = MaxThrust / 2;

	UPROPERTY(EditAnywhere, Category = Movement)
	float SensitivityRoll;

	UPROPERTY(EditAnywhere, Category = Movement)
	float MaxPitchForce;

	UPROPERTY(EditAnywhere, Category = Movement)
	float MinPitchForce;
	
	UPROPERTY(EditAnywhere, Category = Movement)
	float SensitivityYaw;

	UPROPERTY(EditAnywhere, Category = Movement)
	float MaxRadialVelocityInRadians = 1000.f;
	// ~Movement Properties


	UPROPERTY(BlueprintReadOnly)
	float m_Thrust = 100.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int16 CheckPointNum;

private:
	bool addingUpThrust = false;

	float m_PitchForce;

	float m_ChangePitch = 0;
	float m_GL_DeltaTime;

	void ChangeAcceleration(const FInputActionValue& rValue);

	void PlanePitch(const FInputActionValue& rValue);
	void PlaneRoll(const FInputActionValue& rValue);
	void PlaneYaw(const FInputActionValue& rValue);
	

	void Accelerate();
	void ChangePitch();
};
