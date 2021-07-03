// Fill out your copyright notice in the Description page of Project Settings.


#include "RP_CharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapons/RP_Weapon.h"

// Sets default values
ARP_CharacterPlayer::ARP_CharacterPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseFirstPersonView = true;
	FPSCameraSocketName = "SCK_Camera";

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCameraComponent"));
	FPSCameraComponent->bUsePawnControlRotation = true;
	FPSCameraComponent->SetupAttachment(GetMesh(), FPSCameraSocketName);

	SpringArmComponent_R = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent_R"));
	SpringArmComponent_R->bUsePawnControlRotation = true;
	SpringArmComponent_R->SetupAttachment(RootComponent);

	TPSCameraComponent_R = CreateDefaultSubobject<UCameraComponent>(TEXT("TPSCameraComponent_R"));
	TPSCameraComponent_R->SetupAttachment(SpringArmComponent_R);
}

FVector ARP_CharacterPlayer::GetPawnViewLocation() const
{
	if(IsValid(FPSCameraComponent) && bUseFirstPersonView)
	{
		return FPSCameraComponent->GetComponentLocation();
	}else if(IsValid(TPSCameraComponent_R) && !bUseFirstPersonView)
	{
		return TPSCameraComponent_R->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

// Called when the game starts or when spawned
void ARP_CharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	CreateInitialWeapon();
}

// Called every frame
void ARP_CharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARP_CharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ARP_CharacterPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARP_CharacterPlayer::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ARP_CharacterPlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &ACharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARP_CharacterPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARP_CharacterPlayer::StopJumping);

	PlayerInputComponent->BindAction("WeaponAction", IE_Pressed, this, &ARP_CharacterPlayer::StartWeaponAction);
	PlayerInputComponent->BindAction("WeaponAction", IE_Released, this, &ARP_CharacterPlayer::StopWeaponAction);
}

void ARP_CharacterPlayer::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}

void ARP_CharacterPlayer::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}

void ARP_CharacterPlayer::Jump()
{
	Super::Jump();
}

void ARP_CharacterPlayer::StopJumping()
{
	Super::StopJumping();
}

void ARP_CharacterPlayer::StartWeaponAction()
{
	if(IsValid(CurrentWeapon))
	{
		CurrentWeapon->StartAction();
	}
}

void ARP_CharacterPlayer::StopWeaponAction()
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StopAction();
	}
}

void ARP_CharacterPlayer::CreateInitialWeapon()
{
	if (IsValid(InitialWeaponClass))
	{
		CurrentWeapon = GetWorld()->SpawnActor<ARP_Weapon>(InitialWeaponClass, GetActorLocation(), GetActorRotation());

		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->SetCharacterOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}


void ARP_CharacterPlayer::AddControllerPitchInput(float value)
{
	Super::AddControllerPitchInput(bIsLookInversion? -value : value);
}

void ARP_CharacterPlayer::AddKey(FName NewKey)
{
	DoorKeys.Add(NewKey);
}

bool ARP_CharacterPlayer::HasKey(FName KeyTag)
{
	return DoorKeys.Contains(KeyTag);
}
