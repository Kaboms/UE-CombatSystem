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

    bAutoActivate = true;
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

    for (const auto& WeaponSlotPair : WeaponSlots)
    {
        if (IsValid(WeaponSlotPair.Value.DefaultWeapon))
        {
            SpawnWeapon(WeaponSlotPair.Value.DefaultWeapon, WeaponSlotPair.Key);
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
    if (!IsActive())
        return;

    if (bIsAttack)
    {
        bMakeNextAttack = true;

        NextAttackTags.AddTag(ComboTag);
        NextAttackTags.AddTag(SlotTag);

    }
    else if (FWeaponSlot* WeaponSlotPtr = WeaponSlots.Find(SlotTag))
    {
        AWeapon* Weapon = WeaponSlotPtr->Weapon;
        if (IsValid(Weapon))
        {
            Weapon->StartCombo(ComboTag);
        }
    }
}

void UCombatComponent::StartAttack(UAttackBase* Attack)
{
    if (!IsActive())
        return;

    ActiveAttacks.Add(Attack);
    Attack->StartAttack();
}

void UCombatComponent::EndAttack(UAttackBase* Attack)
{
    ActiveAttacks.Remove(Attack);
    Attack->EndAttack();
}

void UCombatComponent::StartWeaponAttack(FGameplayTag WeaponSlotTag, UAttackBase* Attack)
{
    if (!IsActive())
        return;

    WeaponActiveAttacks.Add(WeaponSlotTag, Attack);
    StartAttack(Attack);
}

void UCombatComponent::EndWeaponAttack(FGameplayTag WeaponSlotTag, UAttackBase* Attack)
{
    WeaponActiveAttacks.Remove(WeaponSlotTag);
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
    if (!IsActive())
        return;

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
    if (!IsActive())
        return;

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
    if (!IsActive())
        return;

    if (!AttackMovesets.IsEmpty())
    {
        int32 Index = FMath::RandRange(0, AttackMovesets.Num() - 1);
        TArray<FGameplayTag> Keys;
        AttackMovesets.GetKeys(Keys);

        StartMoveset(AttackMovesets[Keys[Index]]);
    }
}

void UCombatComponent::InterruptAttack_Implementation()
{
    TMap<FGameplayTag, UAttackBase*> WeaponActiveAttacksCached = WeaponActiveAttacks;
    for (const auto& ActiveWeaponAttack : WeaponActiveAttacksCached)
    {
        EndWeaponAttack(ActiveWeaponAttack.Key, ActiveWeaponAttack.Value);
    }

    TSet<UAttackBase*> ActiveAttacksCache = ActiveAttacks;
    for (const auto& ActiveAttack : ActiveAttacksCache)
    {
        EndAttack(ActiveAttack);
    }
}

void UCombatComponent::OnComboSectionEndedNotify(UAnimNotify_ComboSection* ComboSectionNotify)
{
    if (!IsActive())
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

AWeapon* UCombatComponent::GetWeaponFromSlot(FGameplayTag WeaponSlotTag)
{
    if (FWeaponSlot* WeaponSlotPtr = WeaponSlots.Find(WeaponSlotTag))
    {
        return WeaponSlotPtr->Weapon;
    }
    return nullptr;
}

bool UCombatComponent::HasActiveAttack()
{
    return !ActiveAttacks.IsEmpty() || !WeaponActiveAttacks.IsEmpty();
}

void UCombatComponent::DetachAllWeapons()
{
    for (auto WeaponSlotPair : WeaponSlots)
    {
        if (AWeapon* Weapon = WeaponSlotPair.Value.Weapon)
        {
            Weapon->Detach();
        }
    }
}
