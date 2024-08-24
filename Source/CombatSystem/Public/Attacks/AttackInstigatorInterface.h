#pragma once

#include "CoreMinimal.h"

#include "AttackInstigatorInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UAttackInstigatorInterface : public UInterface
{
	GENERATED_BODY()
};

class IAttackInstigatorInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnAttackHit(UAttackBase* Attack, AActor* Target, FHitResult HitResult);
};