// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RouletteDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ACKERMANROULETTEEDITOR_API URouletteDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta=(NoElementDuplicate))
	TArray<TSoftObjectPtr<UStaticMesh>> RouletteMeshes;
};
