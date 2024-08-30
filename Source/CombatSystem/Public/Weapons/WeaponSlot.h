// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapon.h"

#include "WeaponSlot.generated.h"

USTRUCT(BlueprintType)
struct FWeaponSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MeshSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWeapon> DefaultWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AWeapon* Weapon = nullptr;
};
