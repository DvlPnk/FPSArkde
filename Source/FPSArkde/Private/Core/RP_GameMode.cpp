// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RP_GameMode.h"
#include "RP_CharacterPlayer.h"
#include "RP_SpectatingCamera.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RP_SpectatingCamera.h"

void ARP_GameMode::BeginPlay()
{
	Super::BeginPlay();
	SetupSpectatingCameras();
}

void ARP_GameMode::SetupSpectatingCameras()
{
	TArray<AActor*> SpectatingCameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARP_SpectatingCamera::StaticClass(), SpectatingCameraActors);
	if (SpectatingCameraActors.Num() > 0)
	{
		for (AActor* SpectatingActor : SpectatingCameraActors)
		{
			ARP_SpectatingCamera* SpectatingCamera = Cast<ARP_SpectatingCamera>(SpectatingActor);
			if (IsValid(SpectatingCamera))
			{
				switch (SpectatingCamera->GetCameraType())
				{
				case ERP_SpectatingCameraType::CameraType_Victory:
					VictoryCamera = SpectatingCamera;
					break;
				case ERP_SpectatingCameraType::CameraType_GameOver:
					GameOverCamera = SpectatingCamera;
					break;
				default:
					break;
				}
			}
		}
	}
}

void ARP_GameMode::MoveCameraSpectatingPoint(ARP_CharacterPlayer* Character, ARP_SpectatingCamera* SpectatingCamera)
{
	if(!IsValid(Character) || !IsValid(SpectatingCamera))
	{
		return;
	}
	AController* CharacterController = Character->GetController();
	if (IsValid(CharacterController))
	{
		APlayerController* PlayerController = Cast<APlayerController>(CharacterController);
		if (IsValid(PlayerController))
		{
			PlayerController->SetViewTargetWithBlend(SpectatingCamera, SpectatingBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);

		}
	}
}

void ARP_GameMode::Victory(ARP_CharacterPlayer* Character)
{
	Character->DisableInput(nullptr);

	MoveCameraSpectatingPoint(Character, VictoryCamera);
	
	BP_Victory(Character);
	
}

void ARP_GameMode::GameOver(ARP_CharacterPlayer* Character)
{
	Character->GetMovementComponent()->StopMovementImmediately();
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if(Character->HasToDestroy())
	{
		Character->DetachFromControllerPendingDestroy();
		Character->SetLifeSpan(5.0f);
	}
	else
	{
		Character->DisableInput(nullptr);
		MoveCameraSpectatingPoint(Character, GameOverCamera);
	}
	
	BP_GameOver(Character);
}
