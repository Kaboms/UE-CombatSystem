// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotifyState_Attack.h"
#include "AnimNotifyState_WeaponAttack.generated.h"

/**
 * 
 */
UCLASS(Meta=(DisplayName="Weapon Attack"))
class COMBATSYSTEM_API UAnimNotifyState_WeaponAttack : public UAnimNotifyState_Attack
{
	GENERATED_BODY()

public:
	FGameplayTag GetSlotTag() { return SlotTag; }

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag SlotTag;
};
