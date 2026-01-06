// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/StatusAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UStatusAttributeSet::UStatusAttributeSet()
{
	//초기값 설정
	InitJumpPower(600.0f);
	InitSpeed(600.0f);
}

void UStatusAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UStatusAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//액터
	// 수치가 변한 캐릭터
	AActor* TargetActor = Data.Target.GetAvatarActor();
	ACharacter* Character = Cast<ACharacter>(TargetActor);

	if (!Character) return;

	// Speed 속성이 변경되었다면 실제 이동 속도에 적용
	if (Data.EvaluatedData.Attribute == GetSpeedAttribute())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = GetSpeed();
	}

	// JumpPower 속성이 변경되었다면 실제 점프력에 적용
	if (Data.EvaluatedData.Attribute == GetJumpPowerAttribute())
	{
		Character->GetCharacterMovement()->JumpZVelocity = GetJumpPower();
	}
}
