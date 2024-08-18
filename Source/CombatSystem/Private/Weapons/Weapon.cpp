// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
#include "CombatComponent.h"

#include "Components/CapsuleComponent.h"

AWeapon::AWeapon() : Super()
{
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
    RootComponent = CapsuleComponent;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

    for (UAttackBase* AttackBase : InstancedAttacks)
    {
        AttackBase->Init(this, CombatComponent);
        Attacks.Add(AttackBase->AttackTag, AttackBase);
    }

    Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AWeapon::OnAttackStartedNotify(FGameplayTag AttackTag)
{
    bAttackStarted = true;

    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);

    if (UAttackBase** FindedAttack = Attacks.Find(AttackTag))
    {
        CombatComponent->StartWeaponAttack(SlotTag, *FindedAttack);
    }

    ReceiveOnAttackStartedNotify(AttackTag);
}

void AWeapon::OnAttackEndedNotify(FGameplayTag AttackTag)
{
    bAttackStarted = false;

    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

    if (UAttackBase** FindedAttack = Attacks.Find(AttackTag))
    {
        CombatComponent->EndWeaponAttack(SlotTag, *FindedAttack);
    }

    ReceiveOnAttackEndedNotify(AttackTag);
}

void AWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
}

void AWeapon::NotifyActorEndOverlap(AActor* OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);
}

void AWeapon::StartCombo(FGameplayTag ComboTag)
{
    if (UAnimMontage** ComboMontage = ComboMontages.Find(ComboTag))
    {
        CombatComponent->StartMoveset(*ComboMontage);
    }

    ReceiveStartCombo(ComboTag);
}

void AWeapon::Detach()
{
    if (!bIsDetachable)
        return;

    ReceiveDetach();
}
