// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameplayAbilityTargetActor.h"

#include "Abilities/GameplayAbility.h"
#include "Kismet/KismetSystemLibrary.h"


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
			DebugReticle->SetActorLocation(HitResult.Location,false,nullptr,ETeleportType::None);
			GEngine->AddOnScreenDebugMessage(3,0.0f,FColor::Purple,FString(HitResult.Location.ToString()));
		}
		else
		{
			DebugReticle->SetActorLocation(HitResult.TraceEnd,false,nullptr,ETeleportType::None);
			GEngine->AddOnScreenDebugMessage(3,0.0f,FColor::Orange,FString(HitResult.TraceEnd.ToString()));
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
	GEngine->AddOnScreenDebugMessage(2,100.0f,FColor::Black,FString(DebugReticle->GetName()));
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

bool AMyGameplayAbilityTargetActor::LineTrace(FHitResult &TraceHitResult)
{
	FVector Location;
	FRotator Rotator;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex=true;
	TArray<AActor*> IgnorActor;
	APawn* PlayerPawn=PrimaryPC->GetPawn();
	if (PlayerPawn)
	{
	
		IgnorActor.Add(PlayerPawn);
	}

	//PrimaryPC->GetActorEyesViewPoint(Location,Rotator);
	PrimaryPC->GetPlayerViewPoint(Location,Rotator);
	
	//bool bHit =	GetWorld()->LineTraceSingleByChannel(TraceHitResult,Location,Rotator.Vector()*Range+Location,ECC_Visibility,QueryParams);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(),Location,Rotator.Vector()*Range+Location,TraceTypeQuery1,true,IgnorActor,EDrawDebugTrace::ForDuration,TraceHitResult,true);
	GEngine->AddOnScreenDebugMessage(3,0.0f,FColor::Cyan,FString(HitResult.Location.ToString()));


	return  TraceHitResult.bBlockingHit;
}
