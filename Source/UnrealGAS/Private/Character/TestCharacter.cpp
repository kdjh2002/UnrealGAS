// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestCharacter.h"
#include "AbilitySystemComponent.h"
#include "Components/WidgetComponent.h"
#include "GameAbilitySystem/AttributeSet/ResourceAttributeSet.h"
#include "GameAbilitySystem/AttributeSet/StatusAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/TwinResource.h"
#include "UI/BarWidget.h"

// Sets default values
ATestCharacter::ATestCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	//위젯 컴포넌트 생성
	BarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	BarWidgetComponent->SetupAttachment(RootComponent); //루트컴포넌트 밑에 붙이기

	// 컴포넌트 생성
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// 어트리뷰트 셋 생성
	ResourceAttributeSet = CreateDefaultSubobject<UResourceAttributeSet>(TEXT("Resource"));
	StatusAttributeSet = CreateDefaultSubobject<UStatusAttributeSet>(TEXT("Status"));
}

void ATestCharacter::TestHealthChange(float Amount)
{
	if (ResourceAttributeSet)
	{
		float CurrentValue = ResourceAttributeSet->GetHealth();
		ResourceAttributeSet->SetHealth(CurrentValue + Amount);
	}
}

void ATestCharacter::TestSetByCaller(float Amount)
{
	if (AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(TestEffectClass, 0, EffectContext);

		if (SpecHandle.IsValid())
		{
			{
				SpecHandle.Data->SetSetByCallerMagnitude(Tag_EffectDamage, Amount);
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}

void ATestCharacter::TestAddInfiniteEffect()
{
	if (TestInfiniteEffectClass && AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddInstigator(this, this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(TestInfiniteEffectClass, 0, EffectContext);

		if (SpecHandle.IsValid())
		{
			TestInfinite = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ATestCharacter::TestRemoveInfiniteEffect()
{
	if (TestInfinite.IsValid())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(TestInfinite);
	}
}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);	// 어빌리티 시스템 컴포넌트 초기화

		//초기화 이후에만 가능
		FOnGameplayAttributeValueChange& onHealthChange =
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UResourceAttributeSet::GetHealthAttribute());
		onHealthChange.AddUObject(this, &ATestCharacter::OnHealthChange);	//Health가 변경되었을떄 실행될 함수 바인딩

		FOnGameplayAttributeValueChange& onMaxHealthChange =
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UResourceAttributeSet::GetMaxHealthAttribute());
		onMaxHealthChange.AddUObject(this, &ATestCharacter::OnMaxHealthChange);	//Health가 변경되었을떄 실행될 함수 바인딩

		FOnGameplayAttributeValueChange& onManaChange =
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UResourceAttributeSet::GetManaAttribute());
		onManaChange.AddUObject(this, &ATestCharacter::OnManaChange);	//Health가 변경되었을떄 실행될 함수 바인딩

	//	FOnGameplayAttributeValueChange& onJumpPowerChange =
	//		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStatusAttributeSet::GetJumpPowerAttribute());
	//	onJumpPowerChange.AddUObject(this, &ATestCharacter::OnJumpPowerChange);	//Health가 변경되었을떄 실행될 함수 바인딩

	//	FOnGameplayAttributeValueChange& onSpeedChange =
	//		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStatusAttributeSet::GetSpeedAttribute());
	//	onSpeedChange.AddUObject(this, &ATestCharacter::OnSpeedChange);	//Health가 변경되었을떄 실행될 함수 바인딩

	//	if (AbilitySystemComponent && StatusAttributeSet)
	//	{
	//		// 실제 Movement에 설정된 값을 GAS 수치로 가져오기
	//		StatusAttributeSet->InitSpeed(GetCharacterMovement()->MaxWalkSpeed);
	//		StatusAttributeSet->InitJumpPower(GetCharacterMovement()->JumpZVelocity);
	//	}
	}

	if (ResourceAttributeSet)
	{
		if (BarWidgetComponent && BarWidgetComponent->GetWidget())
		{
			if (BarWidgetComponent->GetWidget()->Implements<UTwinResource>())
			{
				ITwinResource::Execute_UpdateMaxHealth(BarWidgetComponent->GetWidget(), ResourceAttributeSet->GetMaxHealth());
				ITwinResource::Execute_UpdateMaxMana(BarWidgetComponent->GetWidget(), ResourceAttributeSet->GetMaxMana());

				ITwinResource::Execute_UpdateCurrentHealth(BarWidgetComponent->GetWidget(), ResourceAttributeSet->GetHealth());
				ITwinResource::Execute_UpdateCurrentMana(BarWidgetComponent->GetWidget(), ResourceAttributeSet->GetMana());
			}
		}
		//ResourceAttributeSet->Health = 50.0f;	// 절대 안됨
		//ResourceAttributeSet->SetHealth(50.0f);	// 무조건 Setter로 변경해야 한다.
	}
	Tag_EffectDamage = FGameplayTag::RequestGameplayTag(FName("Effect.Damage"));

	if (InitializeEffectClass && AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(InitializeEffectClass, 0, EffectContext);
		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}


// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (ResourceAttributeSet)
	{
	FString healthString = FString::Printf(TEXT("%.1f / %.1f"),
		ResourceAttributeSet->GetHealth(), ResourceAttributeSet->GetMaxHealth());

	}
	//UE_LOG(LogTemp, Log, TEXT("%s"), *healthString);
	//	DrawDebugString(GetWorld(), GetActorLocation(), healthString, nullptr, FColor::White, 0, true);
}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATestCharacter::OnMaxHealthChange(const FOnAttributeChangeData& InData)
{
	ITwinResource::Execute_UpdateMaxHealth(BarWidgetComponent->GetWidget(), ResourceAttributeSet->GetMana());

}

void ATestCharacter::OnHealthChange(const FOnAttributeChangeData& InData)
{
	UE_LOG(LogTemp, Log, TEXT("Health Change : %.1f->%.1f"), InData.OldValue, InData.NewValue);
	ITwinResource::Execute_UpdateCurrentHealth(BarWidgetComponent->GetWidget(), ResourceAttributeSet->GetHealth());
}

void ATestCharacter::OnMaxManaChange(const FOnAttributeChangeData& InData)
{
	ITwinResource::Execute_UpdateMaxMana(BarWidgetComponent->GetWidget(), ResourceAttributeSet->GetMaxMana());
}

void ATestCharacter::OnManaChange(const FOnAttributeChangeData& InData)
{
	UE_LOG(LogTemp, Log, TEXT("On Mana Change : %.1f -> %.1f"), InData.OldValue, InData.NewValue);
	ITwinResource::Execute_UpdateCurrentMana(BarWidgetComponent->GetWidget(), ResourceAttributeSet->GetMana());
}

//void ATestCharacter::OnJumpPowerChange(const FOnAttributeChangeData& InData)
//{
//	// 수치가 변하면 즉시 캐릭터 속도에 반영 (PostGameplayEffectExecute와 중복이지만 안전함)
//	GetCharacterMovement()->MaxWalkSpeed = InData.NewValue;
//}
//
//void ATestCharacter::OnSpeedChange(const FOnAttributeChangeData& InData)
//{
//	GetCharacterMovement()->JumpZVelocity = InData.NewValue;
//}
//
//
//
