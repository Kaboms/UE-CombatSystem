// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"

#include "GameplayTagContainer.h"
#include "AnimNotifies/AnimNotify_ComboSection.h"
#include "Weapons/WeaponSlot.h"

#include "CombatComponent.generated.h"

class AWeapon;
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
	void StartWeaponAttack(FGameplayTag WeaponSlotTag, UAttackBase* Attack);

	UFUNCTION(BlueprintCallable)
	void EndWeaponAttack(FGameplayTag WeaponSlotTag, UAttackBase* Attack);

	UFUNCTION(BlueprintCallable)
	void OnAttackStarted(UAnimNotifyState_Attack* AttackNS);

	UFUNCTION(BlueprintCallable)
	void OnAttackEnded(UAnimNotifyState_Attack* AttackNS);

	UFUNCTION(BlueprintCallable)
	void OnWeaponAttackStarted(UAnimNotifyState_WeaponAttack* WeaponAttackNS);

	UFUNCTION(BlueprintCallable)
	void OnWeaponAttackEnded(UAnimNotifyState_WeaponAttack* WeaponAttackNS);

	UFUNCTION(BlueprintCallable)
	void StartMoveset(UAnimMontage* ComboMontage);

	UFUNCTION(BlueprintCallable)
	void StartMovesetByTag(FGameplayTag MovesetTag);

	UFUNCTION(BlueprintCallable)
	void StartRandomMoveset();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InterruptAttack();

	UFUNCTION(BlueprintCallable)
	void OnComboSectionEndedNotify(UAnimNotify_ComboSection* ComboSectionNotify);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	AWeapon* SpawnWeapon(TSubclassOf<AWeapon> WeaponClass, FGameplayTag WeaponSlotTag);

	UFUNCTION(BlueprintCallable)
	void SpawnDefaultWeapons();

	// Remove weapon from slot if exist and spawn new weapon. By default old weapon will be destroyed - to detach weapon use bDetachOldWeapon
	UFUNCTION(BlueprintCallable)
	AWeapon* ChangeWeaponAtSlot(TSubclassOf<AWeapon> NewWeaponClass, FGameplayTag WeaponSlotTag, bool bDetachOldWeapon = false);

	UFUNCTION(BlueprintGetter)
	ACharacter* GetCharacter() { return Character; }

	UFUNCTION(BlueprintPure)
	AWeapon* GetWeaponFromSlot(FGameplayTag WeaponSlotTag);

	UFUNCTION(BlueprintPure)
	bool HasActiveAttack();

	UFUNCTION(BlueprintCallable)
	void DetachAllWeapons();

protected:
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnComboSectionEndedNotify"))
	void ReceiveOnComboSectionEndedNotify(const UAnimNotify_ComboSection* ComboSectionNotify);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "StartMoveset"))
	void ReceiveStartMoveset(UAnimMontage* ComboMontage);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAttack = false;

	UPROPERTY(BlueprintReadOnly)
	TSet<UAttackBase*> ActiveAttacks;

	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, UAttackBase*> WeaponActiveAttacks;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintGetter = GetCharacter)
	TObjectPtr<ACharacter> Character;

	UPROPERTY(BlueprintReadWrite)
	bool bMakeNextAttack = false;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer NextAttackTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FWeaponSlot> WeaponSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, UAnimMontage*> AttackMovesets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TMap<FGameplayTag, UAttackBase*> Attacks;
};
