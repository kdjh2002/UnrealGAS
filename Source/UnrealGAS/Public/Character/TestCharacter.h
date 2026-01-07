// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "TestCharacter.generated.h"

class UResourceAttributeSet;
class UStatusAttributeSet;
class UWidgetComponent;
class UGameplayEffect;


UCLASS()
class UNREALGAS_API ATestCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {
		return AbilitySystemComponent;
	}

	UFUNCTION(BlueprintCallable)
	void TestHealthChange(float Amount);
	UFUNCTION(BlueprintCallable)
	void TestSetByCaller(float Amount);
	UFUNCTION(BlueprintCallable)
	void TestAddInfiniteEffect();
	UFUNCTION(BlueprintCallable)
	void TestRemoveInfiniteEffect();
	UFUNCTION(BlueprintCallable)
	void TestAbility();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnMaxHealthChange(const FOnAttributeChangeData& InData);
	void OnHealthChange(const FOnAttributeChangeData& InData);
	void OnMaxManaChange(const FOnAttributeChangeData& InData);
	void OnManaChange(const FOnAttributeChangeData& InData);
	//void OnJumpPowerChange(const FOnAttributeChangeData& InData);
	//void OnSpeedChange(const FOnAttributeChangeData& InData);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	float TestValue = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	TSubclassOf<UGameplayEffect> TestEffectClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	TSubclassOf<UGameplayEffect> TestInfiniteEffectClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	TSubclassOf<UGameplayEffect> InitializeEffectClass = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayAbility> HasteClass = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UWidgetComponent> BarWidgetComponent = nullptr;


private:
	UPROPERTY()
	TObjectPtr<UResourceAttributeSet> ResourceAttributeSet = nullptr;
	UPROPERTY()
	TObjectPtr<UStatusAttributeSet> StatusAttributeSet = nullptr;


	FGameplayTag Tag_EffectDamage;

	//±¸Á¶Ã¼
	FActiveGameplayEffectHandle TestInfinite;
};