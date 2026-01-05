// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestCharacter.h"
#include "AbilitySystemComponent.h"
#include "Components/WidgetComponent.h"
#include "GameAbilitySystem/ResourceAttributeSet.h"

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
	StatusAttributeSet = CreateDefaultSubobject<UResourceAttributeSet>(TEXT("Status"));
}

void ATestCharacter::TestHealthChange(float Amount)
{
	if (StatusAttributeSet)
	{
		float CurrentValue = StatusAttributeSet->GetHealth();
		StatusAttributeSet->SetHealth(CurrentValue + Amount);
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

	}

	if (StatusAttributeSet)
	{
		if (BarWidgetComponent && BarWidgetComponent->GetWidget())
		{
			if (BarWidgetComponent->GetWidget()->Implements<UTwinResource>())
			{
				ITwinResource::Execute_UpdateMaxHealth(BarWidgetComponent->GetWidget(), StatusAttributeSet->GetMaxHealth());
				ITwinResource::Execute_UpdateMaxMana(BarWidgetComponent->GetWidget(), StatusAttributeSet->GetMaxMana());

				ITwinResource::Execute_UpdateCurrentHealth(BarWidgetComponent->GetWidget(), StatusAttributeSet->GetHealth());
				ITwinResource::Execute_UpdateCurrentMana(BarWidgetComponent->GetWidget(), StatusAttributeSet->GetMana());
			}

		//StatusAttributeSet->Health = 50.0f;	// 절대 안됨
		//StatusAttributeSet->SetHealth(50.0f);	// 무조건 Setter로 변경해야 한다.
	
		}
	}
}

// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString healthString = FString::Printf(TEXT("%.1f / %.1f"),
		StatusAttributeSet->GetHealth(), StatusAttributeSet->GetMaxHealth());
	UE_LOG(LogTemp, Log, TEXT("%s"), *healthString);
		DrawDebugString(GetWorld(), GetActorLocation(), healthString, nullptr, FColor::White, 0, true);


		////컴포넌트에서 UUserWidget을 가져와 클래스 캐스팅
		//UBarWidget* HealthUI = Cast<UBarWidget>(HealthWidgetComponent->GetWidget());

		//float CurrentHP = StatusAttributeSet->GetHealth();
		//float MaxHP = StatusAttributeSet->GetMaxHealth();

		////위젯 업데이트함수 호출
		//HealthUI->UpdateHealth(CurrentHP, MaxHP);

}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATestCharacter::OnHealthChange(const FOnAttributeChangeData& InData)
{
	UE_LOG(LogTemp, Log, TEXT("Health Change : %.1f->%.1f"), InData.OldValue, InData.NewValue);
	ITwinResource::Execute_UpdateCurrentHealth(BarWidgetComponent->GetWidget(), StatusAttributeSet->GetHealth());
}

void ATestCharacter::OnManaChange(const FOnAttributeChangeData& InData)
{
	UE_LOG(LogTemp, Log, TEXT("On Mana Change : %.1f -> %.1f"), InData.OldValue, InData.NewValue);
	ITwinResource::Execute_UpdateCurrentMana(BarWidgetComponent->GetWidget(), StatusAttributeSet->GetMana());
}



