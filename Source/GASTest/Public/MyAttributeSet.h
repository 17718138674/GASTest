 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "MyAbilitySystemComponent.h"
#include "MyAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) 
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttrchangeDelegate,float, Attr, int32, stackCount);
struct FGameplayEffectModCallbackData;
UCLASS()
class GASTEST_API UMyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	UMyAttributeSet();
public:
	
	//在生命周期内将对象设置为网络复制
	virtual void  GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute",ReplicatedUsing=OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet,CurrentHealth);
	UPROPERTY(BlueprintReadOnly,Category="Attribute",ReplicatedUsing=OnRep_CurrentMana)
	FGameplayAttributeData CurrentMana;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet,CurrentMana);
	UPROPERTY(BlueprintReadOnly, Category = "Attribute",ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet,MaxHealth);
	UPROPERTY(BlueprintReadOnly,Category="Attribute",ReplicatedUsing=OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet,MaxMana);
	//属性更新时回调
	FAttrchangeDelegate HealthChange;
	FAttrchangeDelegate ManaChange;

	//属性同步时调用的方法
	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData &oldCurrentHealth);
	UFUNCTION()
	virtual void OnRep_CurrentMana(const FGameplayAttributeData &oldCurrentMana);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData &oldMaxHealth);
	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData &oldMaxMana);

	virtual void PostGameplayEffectExecute(const struct  FGameplayEffectModCallbackData& Data) override;
	
};
