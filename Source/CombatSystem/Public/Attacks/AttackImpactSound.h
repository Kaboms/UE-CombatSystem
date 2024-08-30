#pragma once

#include "CoreMinimal.h"

#include "AttackImpactSound.generated.h"

USTRUCT(BlueprintType)
struct FAttackImpactSound
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundWave* SoundWave = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool PlayAtLocation = true;
};

USTRUCT(BlueprintType)
struct FAttackImpactSoundSet
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FAttackImpactSound> ImpactSounds;
};

USTRUCT(BlueprintType)
struct FImpactSoundTypes
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FGameplayTag, FAttackImpactSoundSet> AttackSounds;
};