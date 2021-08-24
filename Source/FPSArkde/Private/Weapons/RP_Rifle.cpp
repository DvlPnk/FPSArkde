// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/RP_Rifle.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "FPSArkde/FPSArkde.h"

ARP_Rifle::ARP_Rifle()
{
	TraceLenght = 10000.0f;
	MuzzleSocketName = "SCK_Muzzle";
}


void ARP_Rifle::StartAction()
{
	Super::StartAction();

	AActor* CurrentOwner = GetOwner();
	if(IsValid(CurrentOwner))
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		
		CurrentOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();	 
		FVector TraceEnd = EyeLocation + (ShotDirection * TraceLenght);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(CurrentOwner);
		QueryParams.bTraceComplex = true;

		FVector TraceEndPoint = TraceEnd;

		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams);
		if(bHit)
		{
			AActor* HitActor = HitResult.GetActor();
			if(IsValid(HitActor))
			{
				UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, HitResult, 
					CurrentOwner->GetInstigatorController(), this, DamageType);
			}
			
			TraceEndPoint = HitResult.ImpactPoint;
			
			if(IsValid(ImpactEffect))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, TraceEndPoint, HitResult.ImpactNormal.Rotation());
			}
		}
		if (bDrawLineTrace)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White,
				false, 1.0f, 0.0f, 1.0f);
		}

		if(IsValid(MuzzleEffect))
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, CurrentOwnerCharacter->GetMesh(), MuzzleSocketName);
		}
		if(IsValid(TraceEffect))
		{
			USkeletalMeshComponent* CharacterMeshComponent = CurrentOwnerCharacter->GetMesh();
			if (IsValid(CharacterMeshComponent))
			{
				FVector MuzzleSocketLocation = CharacterMeshComponent->GetSocketLocation(MuzzleSocketName);
				UParticleSystemComponent* TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
					TraceEffect, MuzzleSocketLocation);
				if(IsValid(TracerComponent))
				{
					TracerComponent->SetVectorParameter(TraceParamtName, TraceEndPoint);
				}
			}
		}
	}
}

void ARP_Rifle::StopAction()
{
	Super::StopAction();
	
}
