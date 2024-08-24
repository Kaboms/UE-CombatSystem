// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "AttackImpactSound.h"
#include "AttackInstigatorInterface.h"

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

	virtual class UWorld* GetWorld() const override;

	UFUNCTION(BlueprintNativeEvent)
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Init")
	void ReceiveInit();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "StartAttack")
	void ReceiveStartAttack();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "EndAttack")
	void ReceiveEndAttack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ApplyAttackToTarget(AActor* Target, FHitResult HitResult);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AttackTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> AttackedActors;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Owner;

	IAttackInstigatorInterface* AttackInstigator;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UCombatComponent> CombatComponent;

	UPROPERTY(BlueprintReadWrite)
	bool bIsActive = false;
};