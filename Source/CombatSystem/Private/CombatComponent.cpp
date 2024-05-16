// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Weapons/Weapon.h"
#include "AnimNotifies/AnimNotifyState_Attack.h"
#include "AnimNotifies/AnimNotifyState_WeaponAttack.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
    Character = Cast<ACharacter>(GetOwner());

    Super::BeginPlay();

    for (auto& AttackBase : Attacks)
    {
        if (UAttackBase* Attack = AttackBase.Value)
        {
            Attack->AttackTag = AttackBase.Key;
            Attack->Init(Character, this);
        }
    }

    for (auto WeaponSlotPair : WeaponSlots)
    {
        if (!WeaponSlotPair.Value->DefaultWeapons.IsEmpty())
        {
            SpawnWeapon(WeaponSlotPair.Value->DefaultWeapons[0], WeaponSlotPair.Key);
        }
    }
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (HasActiveAttack())
    {
        for (UAttackBase* Attack : ActiveAttacks)
        {
            Attack->Tick(DeltaTime);
        }
    }
}

void UCombatComponent::StartWeaponCombo(FGameplayTag SlotTag, FGameplayTag ComboTag)
{
    if (!bEnabled)
        return;

    if (bIsAttack)
    {
        bMakeNextAttack = true;

        NextAttackTags.AddTag(ComboTag);
        NextAttackTags.AddTag(SlotTag);

    }
    else if (UWeaponSlot** WeaponSlotPtr = WeaponSlots.Find(SlotTag))
    {
        UWeaponSlot* WeaponSlot = *WeaponSlotPtr;
        AWeapon* Weapon = WeaponSlot->GetCurrentWeapon();
        if (IsValid(Weapon))
        {
            Weapon->StartCombo(ComboTag);
        }
    }
}

void UCombatComponent::StartAttack(UAttackBase* Attack)
{
    ActiveAttacks.Add(Attack);
    Attack->StartAttack();
}

void UCombatComponent::EndAttack(UAttackBase* Attack)
{
    ActiveAttacks.Remove(Attack);
    Attack->EndAttack();
}

void UCombatComponent::StartWeaponAttack(FGameplayTag WeaponTag, UAttackBase* Attack)
{
    WeaponActiveAttacks.Add(WeaponTag, Attack);
    StartAttack(Attack);
}

void UCombatComponent::EndWeaponAttack(FGameplayTag WeaponTag, UAttackBase* Attack)
{
    WeaponActiveAttacks.Remove(WeaponTag);
    EndAttack(Attack);
}

void UCombatComponent::OnAttackStarted(UAnimNotifyState_Attack* AttackNS)
{
    if (UAttackBase** FindedAttack = Attacks.Find(AttackNS->GetAttackTag()))
    {
        StartAttack(*FindedAttack);
    }
}

void UCombatComponent::OnAttackEnded(UAnimNotifyState_Attack* AttackNS)
{
    if (UAttackBase** FindedAttack = Attacks.Find(AttackNS->GetAttackTag()))
    {
        EndAttack(*FindedAttack);
    }
}

void UCombatComponent::OnWeaponAttackStarted(UAnimNotifyState_WeaponAttack* WeaponAttackNS)
{
    AWeapon* Weapon = GetWeaponFromSlot(WeaponAttackNS->GetSlotTag());
    if (IsValid(Weapon))
    {
        Weapon->OnAttackStartedNotify(WeaponAttackNS->GetAttackTag());
    }
}

void UCombatComponent::OnWeaponAttackEnded(UAnimNotifyState_WeaponAttack* WeaponAttackNS)
{
    AWeapon* Weapon = GetWeaponFromSlot(WeaponAttackNS->GetSlotTag());
    if (IsValid(Weapon))
    {
        Weapon->OnAttackEndedNotify(WeaponAttackNS->GetAttackTag());
    }
}

void UCombatComponent::StartMoveset(UAnimMontage* ComboMontage)
{
    if (IsValid(ComboMontage))
    {
        if (bIsAttack)
        {
            bMakeNextAttack = true;
        }
        else
        {
            NextAttackTags.Reset();

            bIsAttack = true;
            ReceiveStartMoveset(ComboMontage);
        }
    }
}

void UCombatComponent::StartMovesetByTag(FGameplayTag MovesetTag)
{
    if (UAnimMontage** MovesetPtr = AttackMovesets.Find(MovesetTag))
    {
        if (bIsAttack)
        {
            NextAttackTags.AddTag(MovesetTag);
        }
        StartMoveset(*MovesetPtr);
    }
}

void UCombatComponent::StartRandomMoveset()
{
    if (!AttackMovesets.IsEmpty())
    {
        int32 Index = FMath::RandRange(0, AttackMovesets.Num() - 1);
        TArray<FGameplayTag> Keys;
        AttackMovesets.GetKeys(Keys);

        StartMoveset(AttackMovesets[Keys[Index]]);
    }
}

void UCombatComponent::OnComboSectionEndedNotify(UAnimNotify_ComboSection* ComboSectionNotify)
{
    if (!bEnabled)
        return;

    if (bMakeNextAttack)
    {
        UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
        TMap<FName, FGameplayTagContainer> NextAttackSections = ComboSectionNotify->GetNextAttackSections();
        for (const auto& SectionCandidate : NextAttackSections)
        {
            if (NextAttackTags.HasAllExact(SectionCandidate.Value))
            {
                AnimInstance->Montage_JumpToSection(SectionCandidate.Key, AnimInstance->GetCurrentActiveMontage());
                bMakeNextAttack = false;
                break;
            }
        }
    }

    ReceiveOnComboSectionEndedNotify(ComboSectionNotify);
}

AWeapon* UCombatComponent::GetWeaponFromSlot(FGameplayTag WeaponSlot)
{
    if (UWeaponSlot** WeaponSlotPtr = WeaponSlots.Find(WeaponSlot))
    {
        AWeapon* Weapon = (*WeaponSlotPtr)->GetCurrentWeapon();
        return Weapon;
    }
    return nullptr;
}

bool UCombatComponent::HasActiveAttack()
{
    return !ActiveAttacks.IsEmpty() || !WeaponActiveAttacks.IsEmpty();
}
