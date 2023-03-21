// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Window/RouletteDataAsset.h"
#include "AckermanRouletteToolSettings.generated.h"


/**
 * 
 */
UCLASS(BlueprintType, config = EditorPerProjectUserSettings)
class ACKERMANROULETTEEDITOR_API UAckermanRouletteToolSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Default", Config)
	TSoftObjectPtr<URouletteDataAsset> DataAsset = TSoftObjectPtr<URouletteDataAsset>(FSoftObjectPath("/AckermanRoulette/DA_DefaultRouletteMeshes.DA_DefaultRouletteMeshes"));
	UPROPERTY(EditAnywhere, Category="Default", Config)
	FName SpawnFolder = FName("RouletteMeshes");
	UPROPERTY(EditAnywhere, Category="Default", Config)
	FVector SpawnLocation = FVector(0.0f, 0.0f, 100.0f);
	
};
