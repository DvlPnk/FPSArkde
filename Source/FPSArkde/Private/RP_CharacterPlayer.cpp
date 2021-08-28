// Fill out your copyright notice in the Description page of Project Settings.


#include "RP_CharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapons/RP_Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/RP_HealthComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Core/RP_GameInstance.h"
#include "Core/RP_GameMode.h"
#include "FPSArkde/FPSArkde.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARP_CharacterPlayer::ARP_CharacterPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanUseWeapon = true;
	bCanMakeCombos = true;
	bUseFirstPersonView = true;
	FPSCameraSocketName = "SCK_Camera";
	MeleeSocketName = "SCK_Melee";
	MeleeDamage = 10.0f;
	MaxComboMultiplier = 4.0f;
	CurrentComboMultipier = 1.0f;

	MaxUltimateXP = 100.0f;
	MaxUltimateDuration = 10.0f;
	bUltimateWithTicks = true;
	UltimateFreq = 0.5f;
	UltimateWalkSpeed = 1400.0f;
	UltimatePlayRate = 1.5f;
	PlayRate = 1.0f;
	UltimateShootFreq = 0.25f;

	MainMenuMapName = "MainMenuMap";
	
	
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCameraComponent"));
	FPSCameraComponent->bUsePawnControlRotation = true;
	FPSCameraComponent->SetupAttachment(GetMesh(), FPSCameraSocketName);

	SpringArmComponent_R = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent_R"));
	SpringArmComponent_R->bUsePawnControlRotation = true;
	SpringArmComponent_R->SetupAttachment(RootComponent);

	TPSCameraComponent_R = CreateDefaultSubobject<UCameraComponent>(TEXT("TPSCameraComponent_R"));
	TPSCameraComponent_R->SetupAttachment(SpringArmComponent_R);

	MeleeDetectorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeDetectorComponent"));
	MeleeDetectorComponent->SetupAttachment(GetMesh(), MeleeSocketName);
	MeleeDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeDetectorComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);
	MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthComponent = CreateDefaultSubobject<URP_HealthComponent>(TEXT("HealthComponent"));
	
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
	InitializeReferences();
	CreateInitialWeapon();
	MeleeDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &ARP_CharacterPlayer::MakeMeleeDamage);
	
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ARP_CharacterPlayer::OnHealthChange);

	NormalWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void ARP_CharacterPlayer::InitializeReferences()
{
	if(IsValid(GetMesh()))
	{
		MyAnimInstance = GetMesh()->GetAnimInstance();
	}

	GameModeReference = Cast<ARP_GameMode>(GetWorld()->GetAuthGameMode());

	GameInstaceReference = Cast<URP_GameInstance>(GetWorld()->GetGameInstance());
}

// Called every frame
void ARP_CharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsUsingUltimate && bUltimateWithTicks)
	{
		UpdateUltimateDuration(DeltaTime);
	}
	
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

	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &ARP_CharacterPlayer::StartMelee);
	PlayerInputComponent->BindAction("Melee", IE_Released, this, &ARP_CharacterPlayer::StopMelee);

	PlayerInputComponent->BindAction("Ultimate", IE_Pressed, this, &ARP_CharacterPlayer::StartUltimate);

	PlayerInputComponent->BindAction("Exit", IE_Pressed, this, &ARP_CharacterPlayer::GoToMainMenu);
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
	if(!bCanUseWeapon)
	{
		return;
	}
	if(IsValid(CurrentWeapon))
	{
		CurrentWeapon->StartAction();

		if(bIsUsingUltimate)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AutomaticShoot, CurrentWeapon, &ARP_Weapon::StartAction, UltimateShootFreq, true);
		}
	}
}

void ARP_CharacterPlayer::StopWeaponAction()
{
	if(!bCanUseWeapon)
	{
		return;
	}
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StopAction();

		if(bIsUsingUltimate)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutomaticShoot);
		}
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

void ARP_CharacterPlayer::StartMelee()
{
	if(bIsDoingMelee && !bCanMakeCombos)
	{
		return;
	}

	if(bCanMakeCombos)
	{
		if(bIsDoingMelee)
		{
			if(bIsComboEnable)
			{
				if(CurrentComboMultipier < MaxComboMultiplier)
				{
					CurrentComboMultipier++;
					SetComboEnable(false);
				}else
				{
					return;
				}
			}else
			{
				return;
			}
		}
	}
	
	if(IsValid(MyAnimInstance) && IsValid(MeleeMontage))
	{
		MyAnimInstance->Montage_Play(MeleeMontage, PlayRate);
	}

	SetMeleeState(true);
}

void ARP_CharacterPlayer::StopMelee()
{
	if (IsValid(MyAnimInstance))
	{

	}
}

void ARP_CharacterPlayer::StartUltimate()
{
	if(bCanUseUltimate && !bIsUsingUltimate)
	{
		CurrentUltimateDuration = 0.0f;
		
		bIsUsingUltimate = true;

		bCanUseWeapon = false;
		
		if(IsValid(MyAnimInstance) && IsValid(UltimateMontage))
		{
			GetCharacterMovement()->MaxWalkSpeed = 0.0f;
			const float StartUltimateMontageDuration = MyAnimInstance->Montage_Play(UltimateMontage);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_BeginUltimateBehavior, this, &ARP_CharacterPlayer::BeginUltimateBehavior, StartUltimateMontageDuration, false);
		}
		else
		{
			BeginUltimateBehavior();
		}
		
		BP_StartUltimate();
	}
	
}

void ARP_CharacterPlayer::GoToMainMenu()
{
	if(IsValid(GameInstaceReference))
	{
		GameInstaceReference->SaveData();
	}
	
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuMapName);
}

void ARP_CharacterPlayer::MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(IsValid(OtherActor))
	{
		if(OtherActor == this)
		{
			return;
		}

		ARP_CharacterPlayer* MeleeTarget = Cast<ARP_CharacterPlayer>(OtherActor);

		if(IsValid(MeleeTarget))
		{
			bool bPlayerAttackingEnemy = GetCharacterType() == ERP_CharacterType::CharacterType_Player && MeleeTarget->GetCharacterType() == ERP_CharacterType::CharacterType_Enemy;
			bool bEnemyAttackingPlayer = GetCharacterType() == ERP_CharacterType::CharacterType_Enemy && MeleeTarget->GetCharacterType() == ERP_CharacterType::CharacterType_Player;

			if(bPlayerAttackingEnemy || bEnemyAttackingPlayer)
			{
				UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage * CurrentComboMultipier, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
			}
		}
		else
		{
			UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage * CurrentComboMultipier, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
		}
	}
}

void ARP_CharacterPlayer::OnHealthChange(URP_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if(HealthComponent->IsDead() && GetCharacterType() == ERP_CharacterType::CharacterType_Player)
	{
		if(IsValid(GameModeReference))
		{
			GameModeReference->GameOver(this);
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

bool ARP_CharacterPlayer::TryAddHealth(float HealthToAdd)
{
	return HealthComponent->TryAddHealth(HealthToAdd);
}

bool ARP_CharacterPlayer::HasKey(FName KeyTag)
{
	return DoorKeys.Contains(KeyTag);
}

void ARP_CharacterPlayer::SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState)
{
	MeleeDetectorComponent->SetCollisionEnabled(NewCollisionState);
}

void ARP_CharacterPlayer::SetMeleeState(bool NewState)
{
	bIsDoingMelee = NewState;
	bCanUseWeapon = !NewState;
}

void ARP_CharacterPlayer::SetComboEnable(bool NewState)
{
	bIsComboEnable = NewState;
}

void ARP_CharacterPlayer::ResetCombo()
{
	SetComboEnable(false);
	CurrentComboMultipier = 1.0f;
}

void ARP_CharacterPlayer::GainUltimateXP(float XPGained)
{
	if(bCanUseUltimate || bIsUsingUltimate)
	{
		return;
	}

	CurrentUltimateXP = FMath::Clamp(CurrentUltimateXP + XPGained, 0.0f, MaxUltimateXP);
	OnUltimateUpdateDelegate.Broadcast(CurrentUltimateXP, MaxUltimateXP);
	
	if(CurrentUltimateXP == MaxUltimateXP)
	{
		bCanUseUltimate = true;
		OnUltimateStatusDelegate.Broadcast(true);
	}

	BP_GainUltimateXP(XPGained);
}

void ARP_CharacterPlayer::UpdateUltimateDuration(float Value)
{
	CurrentUltimateDuration = FMath::Clamp(CurrentUltimateDuration + Value, 0.0f, MaxUltimateDuration);
	OnUltimateUpdateDelegate.Broadcast(MaxUltimateDuration - CurrentUltimateDuration, MaxUltimateDuration);
	BP_UpdateUltimateDuration(Value);

	if(CurrentUltimateDuration == MaxUltimateDuration)
	{
		bIsUsingUltimate = false;
		bCanUseUltimate = false;
		OnUltimateStatusDelegate.Broadcast(false);

		GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
		PlayRate = 1.0f;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutomaticShoot);

		if(!bUltimateWithTicks)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Ultimate);
		}
		CurrentUltimateXP = 0.0f;
		
		BP_StopUltimate();
	}
}

void ARP_CharacterPlayer::UpdateUltimateDurationWithTimer()
{
	UpdateUltimateDuration(UltimateFreq);
}

void ARP_CharacterPlayer::BeginUltimateBehavior()
{
	GetCharacterMovement()->MaxWalkSpeed = UltimateWalkSpeed;
	bIsUsingUltimate = true;
	bCanUseWeapon = true;
	PlayRate = UltimatePlayRate;

	if (!bUltimateWithTicks)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Ultimate, this, &ARP_CharacterPlayer::UpdateUltimateDurationWithTimer, UltimateFreq, true);
	}
	
}
