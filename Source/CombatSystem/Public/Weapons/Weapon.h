// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"

#include "Attacks/AttackBase.h"
#include "AnimNotifies/AnimNotifyState_Attack.h"
#include "Attacks/AttackInstigatorInterface.h"

#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackEnded);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class COMBATSYSTEM_API AWeapon : public AActor, public IAttackInstigatorInterface
{
	GENERATED_BODY()

public:
	AWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnAttackStartedNotify(FGameplayTag AttackTag);

	UFUNCTION(BlueprintCallable)
	void OnAttackEndedNotify(FGameplayTag AttackTag);

	UFUNCTION(BlueprintNativeEvent)
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void StartCombo(FGameplayTag ComboTag);

	FGameplayTagContainer GetWeaponTags() { return WeaponTags; }

	UFUNCTION(BlueprintCallable)
	void Detach();

	virtual void OnAttackHit_Implementation(UAttackBase* Attack, AActor* Target, FHitResult HitResult) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "Detach"))
	void ReceiveDetach();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnAttackStartedNotify"))
	void ReceiveOnAttackStartedNotify(FGameplayTag AttackTag);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnAttackEndedNotify"))
	void ReceiveOnAttackEndedNotify(FGameplayTag AttackTag);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "StartCombo"))
	void ReceiveStartCombo(FGameplayTag ComboTag);

	// Play random impact sound from ImpactSounds by this HitResult
	UFUNCTION(BlueprintCallable)
	void PlayAttackImpactSound(UAttackBase* Attack, FHitResult HitResult);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	FGameplayTag SlotTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	bool bIsDetachable = true;

protected:
	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = "true"))
	class UCombatComponent* CombatComponent;

	UPROPERTY(BlueprintReadWrite)
	bool bAttackStarted = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<UAttackBase*> InstancedAttacks;

	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, UAttackBase*> Attacks;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, UAnimMontage*> ComboMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	FGameplayTagContainer WeaponTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<EPhysicalSurface>, FImpactSoundTypes> ImpactSounds;
};
