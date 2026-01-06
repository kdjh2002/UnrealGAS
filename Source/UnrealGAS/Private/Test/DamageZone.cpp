// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/DamageZone.h"

// Sets default values
ADamageZone::ADamageZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADamageZone::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ADamageZone::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ADamageZone::OnEndOverlap);
	
}

void ADamageZone::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UAbilitySystemBlueprintLibrary* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (ASC)
	{
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffect
	}

}

// Called every frame
void ADamageZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

