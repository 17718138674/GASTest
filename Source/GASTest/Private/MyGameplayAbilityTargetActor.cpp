// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameplayAbilityTargetActor.h"

#include "Abilities/GameplayAbility.h"


AMyGameplayAbilityTargetActor::AMyGameplayAbilityTargetActor()
{
	PrimaryActorTick.bCanEverTick=true;
	Range=2000.0f;
	DebugReticle=nullptr;
	bDestroyOnConfirmation=true;
	ShouldProduceTargetDataOnServer=true;
}

void AMyGameplayAbilityTargetActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	LineTrace(HitResult);
	if (DebugReticle)
	{
		if (HitResult.bBlockingHit)
		{
			GEngine->AddOnScreenDebugMessage(0,100.0f,FColor::Blue,FString(HitResult.ImpactPoint.ToString()));
			DebugReticle->SetActorLocation(HitResult.ImpactPoint,false,nullptr,ETeleportType::None);
			
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(1,100.0f,FColor::Blue,FString(HitResult.ImpactPoint.ToString()));
			DebugReticle->SetActorLocation(HitResult.TraceEnd,false,nullptr,ETeleportType::None);
		}
		
	}
}

AGameplayAbilityWorldReticle* AMyGameplayAbilityTargetActor::SpawnReticle(FVector Location, FRotator Rotation)
{
	if (ReticleClass)
	{
		if (DebugReticle==nullptr)
		{
			DebugReticle=GetWorld()->SpawnActor<AGameplayAbilityWorldReticle>(ReticleClass,Location,Rotation);
			if (DebugReticle)
			{
				DebugReticle->InitializeReticle(this,PrimaryPC,ReticleParams);
				return  DebugReticle;
			}
		}
		else
		{
			return DebugReticle;
		}
	}
	return nullptr;
}

void AMyGameplayAbilityTargetActor::DestroyedReticl()
{
	if (DebugReticle)
	{
		DebugReticle->Destroy();
	}
}

void AMyGameplayAbilityTargetActor::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	GEngine->AddOnScreenDebugMessage(2,100.0f,FColor::Black,FString("StartTargeting"));
	OwningAbility=Ability;
	PrimaryPC=Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
	DebugReticle=SpawnReticle(GetActorLocation(),GetActorRotation());
	
}

void AMyGameplayAbilityTargetActor::ConfirmTargetingAndContinue()
{
	Super::ConfirmTargetingAndContinue();
	GEngine->AddOnScreenDebugMessage(3,100.0f,FColor::Orange,FString("ConfirmTargetingAndContinue"));
	FHitResult LocalyHitResult;
	FGameplayAbilityTargetDataHandle TargetHandle;
	TargetHandle=StartLocation.MakeTargetDataHandleFromHitResult(OwningAbility,LocalyHitResult);
	if (TargetHandle==nullptr)
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(TargetHandle);
	}
	DestroyedReticl();
}

void AMyGameplayAbilityTargetActor::CancelTargeting()
{
	Super::CancelTargeting();
	DestroyedReticl();
}

bool AMyGameplayAbilityTargetActor::LineTrace(FHitResult TraceHitResult)
{
	FVector Location;
	FRotator Rotator;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex=true;
	APawn* PlayerPawn=PrimaryPC->GetPawn();
	if (PlayerPawn)
	{
	
		QueryParams.AddIgnoredActor(PlayerPawn);
	}

	//PrimaryPC->GetActorEyesViewPoint(Location,Rotator);
	PrimaryPC->GetPlayerViewPoint(Location,Rotator);
	
	bool bHit =	GetWorld()->LineTraceSingleByChannel(TraceHitResult,Location,Rotator.Vector()*Range+Location,ECC_Visibility,QueryParams);
	
	GEngine->AddOnScreenDebugMessage(4,100.0f,FColor::Red,FString(Location.ToString()));
	GEngine->AddOnScreenDebugMessage(5,100.0f,FColor::Green,FString(Rotator.ToString()));

	return  bHit;
}
