// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttributeSet.h"
#include "GameplayEffectExtension.h"

#include "Net/UnrealNetwork.h"

UMyAttributeSet::UMyAttributeSet()
{
}


void UMyAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& oldCurrentHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet,CurrentHealth,oldCurrentHealth);
}

void UMyAttributeSet::OnRep_CurrentMana(const FGameplayAttributeData& oldCurrentMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet,CurrentMana,oldCurrentMana);
}

void UMyAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& oldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet,MaxHealth,oldMaxHealth);
}

void UMyAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& oldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet,MaxMana,oldMaxMana);
}

void UMyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute==GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(),0,GetMaxHealth()));
		HealthChange.Broadcast(GetCurrentHealth(),Data.EffectSpec.StackCount);
	}
	if (Data.EvaluatedData.Attribute==GetCurrentManaAttribute())
	{
		SetCurrentMana(FMath::Clamp(GetCurrentMana(),0,GetMaxMana()));
		ManaChange.Broadcast(GetCurrentMana(),Data.EffectSpec.StackCount);
	}
}

void UMyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet,CurrentHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet,CurrentMana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
}