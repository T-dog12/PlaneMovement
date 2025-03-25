// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlane.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "kismet/GameplayStatics.h"

// Sets default values
AMyPlane::AMyPlane()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Sets up the default objects, so that the blueprint template will have everything it needs
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>("PlaneMesh");
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
	CollisionBox = CreateDefaultSubobject<UBoxComponent>("Box");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	RootComponent = PlaneMesh;
	
	TurretMesh->SetupAttachment(PlaneMesh);
	CollisionBox->SetupAttachment(PlaneMesh);
	SpringArm->SetupAttachment(PlaneMesh);
	Camera->SetupAttachment(SpringArm);
	

	PlaneMesh->SetSimulatePhysics(true);
	
}

// Called when the game starts or when spawned
void AMyPlane::BeginPlay()
{
	Super::BeginPlay();
	
	// Sets up the input map to add to it later
	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Player, 0);
		}
	}

	// Sets the default thrust so plane doesn't instantly falls
	m_Thrust = startingVol;
}

// Called every frame
void AMyPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_GL_DeltaTime = DeltaTime;

	Accelerate();
	ChangePitch();
}

// Called to bind functionality to input
void AMyPlane::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Adds all the Key inputs and assigns it to the desired functions
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Speed
		EnhancedInputComponent->BindAction(IA_ChangeAcceleration, ETriggerEvent::Triggered, this, &AMyPlane::ChangeAcceleration);
		
		// Rotation
		EnhancedInputComponent->BindAction(IA_Pitch, ETriggerEvent::Triggered, this, &AMyPlane::PlanePitch);
		EnhancedInputComponent->BindAction(IA_Pitch, ETriggerEvent::None, this, &AMyPlane::PlanePitch);
		EnhancedInputComponent->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &AMyPlane::PlaneRoll);
		EnhancedInputComponent->BindAction(IA_Yaw, ETriggerEvent::Triggered, this, &AMyPlane::PlaneYaw);

	}
}

void AMyPlane::ChangeAcceleration(const FInputActionValue& rValue)
{
	if (ensure(rValue.GetValueType() == EInputActionValueType::Axis1D))
	{
		// Increases/Decreases the thrust power
		const float temp = FMath::Lerp(MaxThrust/2 + MinThrust, MaxThrust, rValue.Get<float>());
		
		m_Thrust = FMath::FInterpTo(m_Thrust, temp, m_GL_DeltaTime, .25f);
	}
}

void AMyPlane::PlanePitch(const FInputActionValue& rValue)
{
	addingUpThrust = rValue.Get<bool>();

	if (ensure(rValue.GetValueType() == EInputActionValueType::Axis1D))
	{
		const float temp = FMath::Lerp(MinPitchForce, MaxPitchForce, rValue.Get<float>());

		m_ChangePitch = FMath::FInterpTo(m_ChangePitch, temp, m_GL_DeltaTime, 1.f);
	}
}

void AMyPlane::PlaneRoll(const FInputActionValue& rValue)
{
	if (ensure(rValue.GetValueType() == EInputActionValueType::Axis1D))
	{
		// changes rotation to the hole plane
		// bAccelChange needs to be true to apply the force
		PlaneMesh->AddTorqueInDegrees(rValue.Get<float>() * GetActorForwardVector() * -SensitivityRoll, NAME_None, true);
	}
}

void AMyPlane::PlaneYaw(const FInputActionValue& rValue)
{	
	if (ensure(rValue.GetValueType() == EInputActionValueType::Axis1D))
	{

		// changes rotation to the hole plane
		// bAccelChange needs to be true to apply the force
		PlaneMesh->AddTorqueInDegrees(rValue.Get<float>() * GetActorUpVector() * SensitivityYaw, NAME_None, true);
	}
}


void AMyPlane::Accelerate()
{
	FVector upVol;
	FVector forwardVol = PlaneMesh->GetForwardVector() * m_Thrust;


	// applies the changes
	PlaneMesh->SetPhysicsLinearVelocity(upVol + forwardVol);
}

void AMyPlane::ChangePitch()
{

	// changes rotation to the hole plane
	// bAccelChange needs to be true to apply the force
	PlaneMesh->AddTorqueInDegrees(m_ChangePitch * GetActorRightVector(), NAME_None, true);
	if (!addingUpThrust)
	{
		m_ChangePitch = FMath::FInterpTo(m_ChangePitch, 0, m_GL_DeltaTime, 10.f);
	}
}

// speed too low = falling
// check forward vol
//		too low fall
