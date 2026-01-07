// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/StatusAttributeSet.h"
#include "GameplayEffectExtension.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UStatusAttributeSet::UStatusAttributeSet()
{
	//초기값 설정
	InitMoveSpeed(500.0f);
	InitJumpPower(700.0f);
}

void UStatusAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMoveSpeedAttribute())
	{
		AActor* OwningActor = GetOwningActor();	//이 어트리뷰트를 가지고있는 액터
		ACharacter* OwningChar = Cast<ACharacter>(OwningActor);
		if (OwningChar)
		{
			OwningChar->GetCharacterMovement()->MaxWalkSpeed = NewValue;
		}
	}

		if (Attribute == GetJumpPowerAttribute())
		{
			AActor* OwningActor = GetOwningActor();	//이 어트리뷰트를 가지고있는 액터
			ACharacter* OwningChar = Cast<ACharacter>(OwningActor);
			if (OwningChar)
			{
				OwningChar->GetCharacterMovement()->JumpZVelocity = NewValue;
			}
		}
}

//void UStatusAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
//{
//	Super::PostGameplayEffectExecute(Data);
//
//	//액터
//	// 수치가 변한 캐릭터
//	AActor* TargetActor = Data.Target.GetAvatarActor();
//	ACharacter* Character = Cast<ACharacter>(TargetActor);
//
//	if (!Character) return;
//
//	// Speed 속성이 변경되었다면 실제 이동 속도에 적용
//	if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
//	{
//		Character->GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
//	}
//
//	// JumpPower 속성이 변경되었다면 실제 점프력에 적용
//	if (Data.EvaluatedData.Attribute == GetJumpPowerAttribute())
//	{
//		Character->GetCharacterMovement()->JumpZVelocity = GetJumpPower();
//	}
//}
