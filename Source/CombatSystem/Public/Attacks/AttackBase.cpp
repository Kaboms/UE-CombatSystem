#include "AttackBase.h"

void UAttackBase::Init(AActor* InOwner, UCombatComponent* InCombatComponent)
{
    Owner = InOwner;
    CombatComponent = InCombatComponent;

    Owner->OnActorBeginOverlap.AddDynamic(this, &UAttackBase::OnActorBeginOverlap);
    Owner->OnActorEndOverlap.AddDynamic(this, &UAttackBase::OnActorEndOverlap);

	ReceiveInit();
}

void UAttackBase::StartAttack()
{
	bIsActive = true;
	ReceiveStartAttack();

	TArray<AActor*> OverlappingActors;
	Owner->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		OnActorBeginOverlap(Owner, Actor);
	}
}

void UAttackBase::EndAttack()
{
	bIsActive = false;
	ReceiveEndAttack();
}

void UAttackBase::OnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (bIsActive)
	{
		ReceiveOnActorBeginOverlap(OverlappedActor, OtherActor);
	}
}

void UAttackBase::OnActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (bIsActive)
	{
		ReceiveOnActorEndOverlap(OverlappedActor, OtherActor);
	}
}

UWorld* UAttackBase::GetWorld() const
{
	if (GetOuter() == nullptr)
	{
		return nullptr;
	}

	// Special case for behavior tree nodes in the editor
	if (Cast<UPackage>(GetOuter()) != nullptr)
	{
		// GetOuter should return a UPackage and its Outer is a UWorld
		return Cast<UWorld>(GetOuter()->GetOuter());
	}

	// In all other cases...
	return GetOuter()->GetWorld();
}
