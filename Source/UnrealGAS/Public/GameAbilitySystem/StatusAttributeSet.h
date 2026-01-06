// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "StatusAttributeSet.generated.h"

/**
 * 
 */
 // 어트리뷰트 Getter/Setter/Initializer를 쉽게 만들어주는 매크로
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) 


UCLASS()
class UNREALGAS_API UStatusAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UStatusAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	//Instant타입의 이팩트가 적용된 직후에만 호출(Duration이나 Ininite는 )
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData JumpPower;
	ATTRIBUTE_ACCESSORS(UStatusAttributeSet, JumpPower)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UStatusAttributeSet, Speed)

};
