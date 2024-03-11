// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"

#include "GameplayTagContainer.h"
#include "AnimNotifies/AnimNotifyState_ComboSection.h"

#include "CombatComponent.generated.h"

class AWeapon;
class UWeaponSlot;
class ACharacterBase;
class UAnimNotifyState_Attack;
class UAnimNotifyState_WeaponAttack;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATSYSTEM_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartWeaponCombo(FGameplayTag SlotTag, FGameplayTag ComboTag);

	UFUNCTION(BlueprintCallable)
	void StartAttack(UAttackBase* Attack);

	UFUNCTION(BlueprintCallable)
	void EndAttack(UAttackBase* Attack);

	UFUNCTION(BlueprintCallable)
	void StartWeaponAttack(FGameplayTag WeaponTag, UAttackBase* Attack);

	UFUNCTION(BlueprintCallable)
	void EndWeaponAttack(FGameplayTag WeaponTag, UAttackBase* Attack);

	UFUNCTION(BlueprintCallable)
	void OnAttackStarted(UAnimNotifyState_Attack* AttackNS);

	UFUNCTION(BlueprintCallable)
	void OnAttackEnded(UAnimNotifyState_Attack* AttackNS);

	UFUNCTION(BlueprintCallable)
	void OnWeaponAttackStarted(UAnimNotifyState_WeaponAttack* WeaponAttackNS);

	UFUNCTION(BlueprintCallable)
	void OnWeaponAttackEnded(UAnimNotifyState_WeaponAttack* WeaponAttackNS);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartMoveset(UAnimMontage* ComboMontage);

	UFUNCTION(BlueprintCallable)
	void StartRandomMoveset();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InterruptAttack();

	UFUNCTION(BlueprintCallable)
	void OnComboSectionStartedNotify(UAnimNotifyState_ComboSection* ComboSectionNS);

	UFUNCTION(BlueprintCallable)
	void OnComboSectionEndedNotify(UAnimNotifyState_ComboSection* ComboSectionNS);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	AWeapon* SpawnWeapon(TSubclassOf<AWeapon> WeaponClass, FGameplayTag WeaponSlotTag);

	UFUNCTION(BlueprintGetter)
	ACharacter* GetCharacter() { return Character; }

	UFUNCTION(BlueprintPure)
	AWeapon* GetWeaponFromSlot(FGameplayTag WeaponSlot);

	UFUNCTION(BlueprintPure)
	bool HasActiveAttack();

protected:
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnComboSectionStartedNotify"))
	void ReceiveOnComboSectionStartedNotify(const UAnimNotifyState_ComboSection* ComboSectionNS);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnComboSectionEndedNotify"))
	void ReceiveOnComboSectionEndedNotify(const UAnimNotifyState_ComboSection* ComboSectionNS);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAttack = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bEnabled = true;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintGetter = GetCharacter)
	TObjectPtr<ACharacter> Character;

	UPROPERTY(BlueprintReadWrite)
	bool bMakeNextAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TMap<FGameplayTag, UWeaponSlot*> WeaponSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UAnimMontage*> AttackMovesets;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<UAttackBase*> InstancedAttacks;

	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, UAttackBase*> Attacks;

	UPROPERTY(BlueprintReadOnly)
	TSet<UAttackBase*> ActiveAttacks;

	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, UAttackBase*> WeaponActiveAttacks;
};
