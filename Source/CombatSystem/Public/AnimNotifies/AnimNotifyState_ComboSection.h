// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "AnimNotifyState_ComboSection.generated.h"

/**
 * 
 */
UCLASS(Meta = (DisplayName = "ComboSection"))

class COMBATSYSTEM_API UAnimNotifyState_ComboSection : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	FName GetNextAttackSection() { return NextAttackSection; }

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextAttackSection;
};
