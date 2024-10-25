// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
#include "CombatComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Components/CapsuleComponent.h"

AWeapon::AWeapon() : Super()
{
    PrimaryActorTick.bStartWithTickEnabled = false;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
    RootComponent = CapsuleComponent;

    CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::BeginOverlap);
    CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AWeapon::EndOverlap);

    CombatComponent = NULL;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
    Super::BeginPlay();

    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

    for (UAttackBase* AttackBase : InstancedAttacks)
    {
        AttackBase->Init(this, CombatComponent);
        Attacks.Add(AttackBase->AttackTag, AttackBase);
    }

    if (IsValid(CombatComponent))
    {
        CombatComponent->GetCharacter()->OnDestroyed.AddDynamic(this, &AWeapon::OnCharacterDestroyes);
    }
}

void AWeapon::PlayAttackImpactSound(UAttackBase* Attack, FHitResult HitResult)
{
    TEnumAsByte<EPhysicalSurface> SurfaceType = EPhysicalSurface::SurfaceType_Default;
    if (UPhysicalMaterial* PhysicalMaterial = HitResult.PhysMaterial.Get())
    {
        SurfaceType = PhysicalMaterial->SurfaceType;
    }

    if (FImpactSoundTypes* ImpactSoundTypes = ImpactSounds.Find(SurfaceType))
    {
        if (FAttackImpactSoundSet* ImpactSoundSetPtr = ImpactSoundTypes->AttackSounds.Find(Attack->AttackType))
        {
            FAttackImpactSoundSet ImpactSoundSet = *ImpactSoundSetPtr;
            if (!ImpactSoundSet.ImpactSounds.IsEmpty())
            {
                int32 Index = FMath::RandRange(0, ImpactSoundSet.ImpactSounds.Num() - 1);
                FAttackImpactSound Sound = ImpactSoundSet.ImpactSounds[Index];
                if (Sound.PlayAtLocation)
                {
                    UGameplayStatics::PlaySoundAtLocation(this, Sound.SoundWave, HitResult.Location);
                }
                else
                {
                    UGameplayStatics::PlaySound2D(this, Sound.SoundWave);
                }
            }
        }
    }
}

void AWeapon::OnCharacterDestroyes(AActor* DestroyedActor)
{
    Destroy();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AWeapon::OnAttackStartedNotify(FGameplayTag AttackTag)
{
    bAttackStarted = true;

    if (UAttackBase** FindedAttack = Attacks.Find(AttackTag))
    {
        CombatComponent->StartWeaponAttack(SlotTag, *FindedAttack);
    }

    ReceiveOnAttackStartedNotify(AttackTag);

    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
}

void AWeapon::OnAttackEndedNotify(FGameplayTag AttackTag)
{
    bAttackStarted = false;

    if (UAttackBase** FindedAttack = Attacks.Find(AttackTag))
    {
        CombatComponent->EndWeaponAttack(SlotTag, *FindedAttack);
    }

    ReceiveOnAttackEndedNotify(AttackTag);

    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void AWeapon::BeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor == CombatComponent->GetCharacter())
        return;

    if (UAttackBase** AttackBasePtr = CombatComponent->WeaponActiveAttacks.Find(SlotTag))
    {
        (*AttackBasePtr)->BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    }
}

void AWeapon::EndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == CombatComponent->GetCharacter())
        return;

    if (UAttackBase** AttackBasePtr = CombatComponent->WeaponActiveAttacks.Find(SlotTag))
    {
        (*AttackBasePtr)->EndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
    }
}

void AWeapon::StartCombo(FGameplayTag ComboTag)
{
    if (UAnimMontage** ComboMontage = ComboMontages.Find(ComboTag))
    {
        CombatComponent->StartMoveset(*ComboMontage);
    }

    ReceiveStartCombo(ComboTag);
}

void AWeapon::Detach_Implementation()
{
    if (!bIsDetachable)
        return;

    FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
    DetachFromActor(DetachmentTransformRules);

    CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    CapsuleComponent->SetCollisionObjectType(ECC_PhysicsBody);
    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);

    FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](float DeltaTime)
        {
            CapsuleComponent->SetSimulatePhysics(true);
            return false;
        }));
}

void AWeapon::OnAttackHit_Implementation(UAttackBase* Attack, AActor* Target, FHitResult HitResult)
{
    PlayAttackImpactSound(Attack, HitResult);
}
