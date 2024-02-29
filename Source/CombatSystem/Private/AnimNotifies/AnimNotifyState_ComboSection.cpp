// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotifyState_ComboSection.h"
#include "CombatComponent.h"

void UAnimNotifyState_ComboSection::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    UCombatComponent* CombatComponent = Cast<UCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UCombatComponent::StaticClass()));
    if (IsValid(CombatComponent))
    {
        CombatComponent->OnComboSectionStartedNotify(this);
    }
}

void UAnimNotifyState_ComboSection::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    UCombatComponent* CombatComponent = Cast<UCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UCombatComponent::StaticClass()));
    if (IsValid(CombatComponent))
    {
        CombatComponent->OnComboSectionEndedNotify(this);
    }
}