// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/Effect/GEEC_WaterDamage.h"
#include "GameAbilitySystem/AttributeSet/ResourceAttributeSet.h"
#include "GameAbilitySystem/AttributeSet/StatusAttributeSet.h"

//UGEEC_WaterDamage 데미지의 계산에 필요한 어트리뷰트를 캡처하기 위한 구조체(이 계산이 어디에 영향을 줄것인가)
struct FWaterDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);

	FWaterDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UResourceAttributeSet, Health, Target, false);
	
		DEFINE_ATTRIBUTE_CAPTUREDEF(UStatusAttributeSet, AttackPower, Source, true);
	}
};

static FWaterDamageStatics& WaterDamageStatics()
{
	static FWaterDamageStatics Statics;
	return Statics;
}

UGEEC_WaterDamage::UGEEC_WaterDamage()
{
	RelevantAttributesToCapture.Add(WaterDamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(WaterDamageStatics().AttackPowerDef);

	Tag_DebuffWater = FGameplayTag::RequestGameplayTag(FName("Status.Debuff.Water"));
	Tag_ElementWater = FGameplayTag::RequestGameplayTag(FName("Element.Water"));
}

void UGEEC_WaterDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//이팩트를 주고 받는 ASC를 찾아 놓기
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	if (TargetASC && SourceASC)
	{
		const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

		const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
		//소스의 태그 가져오기
		const FGameplayTagContainer* TargetTags = Spec.CapturedSourceTags.GetAggregatedTags();
	
		float Damage = MinimumDamage;

		if (DamageTable)
		{
			float EffectLevel = Spec.GetLevel(); //이팩트 레벨 가져오기

			FRealCurve* DamageCurve = DamageTable->FindCurve(FName("Damage"), TEXT("UGEEC_FireDamage"));
			if (DamageCurve)
			{
				//커브에서 레벨에 해당하는 데미지 값 가져오기
				Damage = DamageCurve->Eval(EffectLevel);
			}
		}

		FAggregatorEvaluateParameters EvaluateParameters;
		EvaluateParameters.SourceTags = SourceTags;
		EvaluateParameters.TargetTags = TargetTags;
		float AttackPower = 0.0f;
		bool Result = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			WaterDamageStatics().HealthDef,
			EvaluateParameters,
			AttackPower);

		if (Result)
		{
			Damage += AttackPower;
		}
		else
		{

		}

		if (SourceTags && SourceTags->HasTag(Tag_ElementWater)
			&& TargetTags && TargetTags->HasTag(Tag_DebuffWater))
		{
			Damage *= 2.0f;
			Damage *= DamageMultiplier;
		}

		if (Damage > 0.0f)
		{
			OutExecutionOutput.AddOutputModifier(
				FGameplayModifierEvaluatedData(
					WaterDamageStatics().HealthProperty,
					EGameplayModOp::Additive,
					-Damage
				)
			);
		}
	}
}