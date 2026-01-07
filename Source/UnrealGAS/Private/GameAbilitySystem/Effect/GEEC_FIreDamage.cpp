// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/Effect/GEEC_FIreDamage.h"
#include "GameAbilitySystem/AttributeSet/ResourceAttributeSet.h"
#include "GameAbilitySystem/AttributeSet/StatusAttributeSet.h"

//UGEEC_FIreDamage 데미지의 계산에 필요한 어트리뷰트를 캡처하기 위한 구조체(이 계산이 어디에 영향을 줄것인가)
struct FFireDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);	//Health 어트리뷰트를 캡처할 것이라고 정의
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);	//Health 어트리뷰트를 캡처할 것이라고 정의


	FFireDamageStatics()
	{
		// UResourceAttributeSet의 Health를 캡쳐하는데, Target으로 부터 캡쳐, 스냅샷은 사용하지 않음
		DEFINE_ATTRIBUTE_CAPTUREDEF(UResourceAttributeSet, Health, Target, false);
		
		// UStatusAttributeSet의 AttackPower를 캡쳐하는데, Source로 부터 캡쳐, 공격시점의 값을 가져오기스냅샷을 사용
		DEFINE_ATTRIBUTE_CAPTUREDEF(UStatusAttributeSet, AttackPower, Source, true);
	}
};

//FFireDamageStatics의 싱글톤 인스턴스를 반환하는 함수
static FFireDamageStatics& FireDamageStatics()
{
	static FFireDamageStatics Statics;	
	return Statics;	
}

UGEEC_FIreDamage::UGEEC_FIreDamage()
{
	RelevantAttributesToCapture.Add(FireDamageStatics().HealthDef);		//캡쳐할 어트리뷰트 목록에 추가
	RelevantAttributesToCapture.Add(FireDamageStatics().AttackPowerDef);		//캡쳐할 어트리뷰트 목록에 추가

	Tag_DebuffBurn = FGameplayTag::RequestGameplayTag(FName("Status.Debuff.Burn"));
	Tag_ElementFire = FGameplayTag::RequestGameplayTag(FName("Element.Fire"));
	//Tag_EffectDamage = FGameplayTag::RequestGameplayTag(FName("Effect.Damage"));
}

void UGEEC_FIreDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//이팩트를 주고 받는 ASC를 찾아 놓기
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	if (TargetASC && SourceASC)
	{
		const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

		const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();	//소스의 태그 가져오기
		const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();	//타겟의 태그 가져오기

		float Damage = MinimumDamage;

		//커브테이블에서 값 가져오기
		if (DamageTable)
		{
			float EffectLevel = Spec.GetLevel(); //이팩트 레벨 가져오기

			//커브테이블에서 커브 가져오기
			FRealCurve* DamageCurve = DamageTable->FindCurve(FName("Damage"), TEXT("UGEEC_FireDamage"));
			if (DamageCurve)
			{
				Damage = DamageCurve->Eval(EffectLevel);	//커브에서 레벨에 해당하는 값 가져오기	
			}
		}
		
		FAggregatorEvaluateParameters EvaluateParameters;
		EvaluateParameters.SourceTags = SourceTags;
		EvaluateParameters.TargetTags = TargetTags;
		float AttackPower = 0.0f;
		bool Result = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			FireDamageStatics().AttackPowerDef,
			EvaluateParameters,
			AttackPower);
		if (Result)
		{
			//정상적으로 값을 가지고 왔다.
			Damage += AttackPower; //공격력만큼 데미지 증가
		}
		else
		{
			//값을 가져오지 못했다.
		}
		//// SetByCaller 밖에서 설정한 데미지값 가져오기
		//float Damage = Spec.GetSetByCallerMagnitude(Tag_EffectDamage, false, 1.0f);		//Tag_DataDamage에 설정된 값을 가져오기(못가져오면 )
		//if (Damage <= 0.0f)
		//{
		//	Damage = 1.0f;	//혹시 음수면 최소값이 1.0f로 설정
		//}

		//조건 여러개 태그할것임
		if (SourceTags && SourceTags->HasTag(Tag_ElementFire)			//공격 데미지가 불속성이고
			&& TargetTags && TargetTags->HasTag(Tag_DebuffBurn))		//피격자가 화상 디버프를 가지고 있으면
		{
			Damage *= 2.0f; // 그러면 데미지 2배
			Damage *= DamageMultiplier; // 추가 데미지 배율 곱하기
		}

		if (Damage > 0.0f)
		{
			OutExecutionOutput.AddOutputModifier(
				FGameplayModifierEvaluatedData(
					FireDamageStatics().HealthProperty,		//Health 어트리뷰트를 변경한다.
					EGameplayModOp::Additive,				//더해 줄거다.
					-Damage									//-Damage만큼 처리
				)
			);
		}
	}
}
