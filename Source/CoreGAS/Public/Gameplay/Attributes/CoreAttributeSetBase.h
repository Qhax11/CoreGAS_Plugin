// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CoreAttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class COREGAS_API UCoreAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UCoreAttributeSetBase();

	virtual void InitFromMetaDataTable(const UDataTable* DataTable) override;
	// Called when GAS data is fully initialized (bound to UCoreASCBase::OnGASDataInitialized).
    // Use this for any initialization logic that requires attributes and ASC to be ready,
    // such as setting initial tags and syncing attribute-driven systems.
	UFUNCTION()
	void InitializeAttributes();

	// Used for clamping attribute values before any change is applied.
	// Fires on every attribute change (GE modifiers, direct sets, etc).
	// Only use for clamping, never send events from here.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// Used for reacting to instant GE executions only (damage, heal, death).
	// Does NOT fire for duration/infinite GE modifiers.
	// Use for game events, tag changes, and logic that should only react to GE execution.
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSetBase, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSetBase, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSetBase, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSetBase, MaxMana)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSetBase, Armor)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData PhysicalDamage;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSetBase, PhysicalDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSetBase, MovementSpeed)

};
