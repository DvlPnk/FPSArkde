// Fill out your copyright notice in the Description page of Project Settings.


#include "RP_CharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


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

// Called when the game starts or when spawned
void ARP_CharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
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

void ARP_CharacterPlayer::MoveSlowly()
{
	//AddMovementInput(GetAc)
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
