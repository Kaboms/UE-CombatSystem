// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "AnimNotify_ComboSection.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Meta = (DisplayName = "ComboSection"))

class COMBATSYSTEM_API UAnimNotify_ComboSection : public UAnimNotify
{
	GENERATED_BODY()

public:
	TMap<FName, FGameplayTagContainer> GetNextAttackSections() { return NextAttackSections; }

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, FGameplayTagContainer> NextAttackSections;
};
