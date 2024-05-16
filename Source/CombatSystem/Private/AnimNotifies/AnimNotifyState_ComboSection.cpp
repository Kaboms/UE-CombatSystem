// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_ComboSection.h"
#include "CombatComponent.h"

void UAnimNotify_ComboSection::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    UCombatComponent* CombatComponent = Cast<UCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UCombatComponent::StaticClass()));
    if (IsValid(CombatComponent))
    {
        CombatComponent->OnComboSectionEndedNotify(this);
    }
}