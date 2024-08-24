#include "AttackBase.h"

void UAttackBase::Init(AActor* InOwner, UCombatComponent* InCombatComponent)
{
    Owner = InOwner;
	AttackInstigator = Cast<IAttackInstigatorInterface>(Owner);

    CombatComponent = InCombatComponent;

	ReceiveInit();
}

void UAttackBase::StartAttack()
{
	bIsActive = true;
	ReceiveStartAttack();
}

void UAttackBase::EndAttack()
{
	bIsActive = false;
	ReceiveEndAttack();
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

void UAttackBase::BeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void UAttackBase::EndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void UAttackBase::ApplyAttackToTarget_Implementation(AActor* Target, FHitResult HitResult)
{
	if (AttackInstigator)
	{
		IAttackInstigatorInterface::Execute_OnAttackHit(AttackInstigator->_getUObject(), this, Target, HitResult);
	}
}
