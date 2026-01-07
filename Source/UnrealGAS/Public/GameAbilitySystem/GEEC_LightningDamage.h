// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_LightningDamage.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAS_API UGEEC_LightningDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UGEEC_LightningDamage();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CustomData")
	float MinimumDamage = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CustomData")
	TObjectPtr<UCurveTable> DataCurveTable = nullptr;


private:
	FGameplayTag Tag_DebuffShock;
	FGameplayTag Tag_ElementLightning;
};
