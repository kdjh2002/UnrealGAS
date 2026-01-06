// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameAbilitySystem/GameAbilitySystemMacros.h"
#include "StatusAttributeSet.generated.h"

/**
 * 
 */
 // 어트리뷰트 Getter/Setter/Initializer를 쉽게 만들어주는 매크로

UCLASS()
class UNREALGAS_API UStatusAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UStatusAttributeSet();

	//Instant타입의 이팩트가 적용된 직후에만 호출(Duration이나 Ininite는 )
	//virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UStatusAttributeSet, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData JumpPower;
	ATTRIBUTE_ACCESSORS(UStatusAttributeSet, JumpPower)


};
