// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "MyGameplayAbilityTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class GASTEST_API AMyGameplayAbilityTargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	AMyGameplayAbilityTargetActor();
	virtual void Tick(float DeltaSeconds) override;
	AGameplayAbilityWorldReticle* DebugReticle;
	virtual  AGameplayAbilityWorldReticle* SpawnReticle(FVector Location ,FRotator Rotation);
	virtual  void DestroyedReticl();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ExposeOnSpawn=true),Category="GAS")
	float Range;
	FHitResult HitResult;

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	virtual void CancelTargeting() override;
	bool LineTrace(FHitResult &TraceHitResult);
	
};
