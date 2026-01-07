// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_FIreDamage.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAS_API UGEEC_FIreDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGEEC_FIreDamage();
	
	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomData")
	float MinimumDamage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomData")
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CustomData")
	 TObjectPtr<UCurveTable> DamageTable = nullptr;


private:
	FGameplayTag Tag_DebuffBurn;
	FGameplayTag Tag_ElementFire;
	//FGameplayTag Tag_EffectDamage;	//SetByCaller¿ë
};
