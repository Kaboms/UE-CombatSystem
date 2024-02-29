// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotifyState_WeaponAttack.h"
#include "CombatComponent.h"

void UAnimNotifyState_WeaponAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    UCombatComponent* CombatComponent = Cast<UCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UCombatComponent::StaticClass()));
    if (IsValid(CombatComponent))
    {
        CombatComponent->OnWeaponAttackStarted(this);
    }
}

void UAnimNotifyState_WeaponAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    UCombatComponent* CombatComponent = Cast<UCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UCombatComponent::StaticClass()));
    if (IsValid(CombatComponent))
    {
        CombatComponent->OnWeaponAttackEnded(this);
    }
}
