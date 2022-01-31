// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainPlayer.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/InteractableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Math/TransformCalculus3D.h"
#include "Utils/UtilitiesBPFunctionLibrary.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Instantiating your class Components
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	//Set the location and rotation of the Character Mesh Transform
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	// Attaching your class Components to the default character's Skeletal Mesh Component.
	SpringArmComp->SetupAttachment(GetMesh());
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	//Setting class variables of the spring arm
	SpringArmComp->bUsePawnControlRotation = true;

	//Actor Components
	InventoryActorComponent = CreateDefaultSubobject<UPlayerInventoryActorComponent>(TEXT("InventoryComponent"));

	//Setting class variables of the Character movement component
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurveTimeline.TickTimeline((DeltaTime));
	CheckLookingAtInteractableObject();
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	SprintTimelineCallback.BindDynamic(this, &AMainPlayer::UpdateSprintPlayerVelocity);

	if (SprintVelocityCurve)
	{
		CurveTimeline.AddInterpFloat(SprintVelocityCurve, SprintTimelineCallback);
	}
}

void AMainPlayer::UpdateSprintPlayerVelocity(float Value)
{
	GetCharacterMovement()->MaxWalkSpeed = Value;
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);
	PlayerInputComponent->BindAxis("TurnUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRight", this, &APawn::AddControllerYawInput);

	// Movement Actions
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainPlayer::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMainPlayer::EndCrouch);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainPlayer::BeginSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainPlayer::EndSprint);

	// Gameplay Actions
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainPlayer::Interact);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this->InventoryActorComponent,
	                                 &UPlayerInventoryActorComponent::DropCurrentWeapon);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainPlayer::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMainPlayer::StopFire);
}

void AMainPlayer::MoveForward(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void AMainPlayer::MoveRight(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		// Find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement in that direction
		AddMovementInput(Direction, AxisValue);
	}
}

void AMainPlayer::BeginSprint()
{
	// GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	CurveTimeline.PlayFromStart();
}

void AMainPlayer::EndSprint()
{
	//GetCharacterMovement()->MaxWalkSpeed = 300.f;
	CurveTimeline.Reverse();
}

void AMainPlayer::BeginCrouch()
{
	Crouch();
}

void AMainPlayer::EndCrouch()
{
	UnCrouch();
}

void AMainPlayer::CheckLookingAtInteractableObject()
{
	FHitResult HitResult;
	FVector EndLocation = UUtilitiesBPFunctionLibrary::CalculateRaycastEnd(
		this->GetController(), InteractDistance, PlayerViewPointLocation, PlayerViewPointRotation);

	if (GetWorld())
	{
		bool actorHit = GetWorld()->LineTraceSingleByChannel(HitResult, PlayerViewPointLocation, EndLocation,
		                                                     ECC_WorldDynamic, FCollisionQueryParams(),
		                                                     FCollisionResponseParams());

		if (actorHit && HitResult.GetActor())
		{
			if (HitResult.GetActor()->Implements<UInteractableInterface>())
			{
				CurrentInteractableObject = HitResult.GetActor();
			}
			else
			{
				CurrentInteractableObject = nullptr;
			}
		}
	}
}

void AMainPlayer::Interact()
{
	if (CurrentInteractableObject)
	{
		IInteractableInterface* InteractableActor = Cast<IInteractableInterface>(CurrentInteractableObject);
		InteractableActor->InteractWith(this);
	}
}

void AMainPlayer::StartFire()
{
	CurveTimeline.Reverse();
	if (InventoryActorComponent->IsPlayerArmed())
	{
		InventoryActorComponent->GetCurrentWeapon()->Shoot();
		GetWorld()->GetTimerManager().SetTimer(TimeHandle_FireShot, InventoryActorComponent->GetCurrentWeapon(),
		                                       &ABaseWaepon::Shoot,
		                                       60 / InventoryActorComponent->GetCurrentWeapon()->FireRate, true);
	}
}

void AMainPlayer::StopFire()
{
	if (InventoryActorComponent->IsPlayerArmed())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimeHandle_FireShot);
	}
}

UPlayerInventoryActorComponent* AMainPlayer::GetInventory()
{
	return InventoryActorComponent;
}