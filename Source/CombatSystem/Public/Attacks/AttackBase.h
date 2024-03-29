// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"

#include "AttackBase.generated.h"

class UCombatComponent;

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class COMBATSYSTEM_API UAttackBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Tick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void Init(AActor* Owner, UCombatComponent* CombatComponent);

	UFUNCTION(BlueprintCallable)
	void StartAttack();

	UFUNCTION(BlueprintCallable)
	void EndAttack();

	UFUNCTION()
	void OnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	virtual class UWorld* GetWorld() const override;

protected:
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Init")
	void ReceiveInit();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnActorBeginOverlap")
	void ReceiveOnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnActorEndOverlap")
	void ReceiveOnActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "StartAttack")
	void ReceiveStartAttack();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "EndAttack")
	void ReceiveEndAttack();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AttackTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> AttackedActors;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Owner;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UCombatComponent> CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActive = false;
};