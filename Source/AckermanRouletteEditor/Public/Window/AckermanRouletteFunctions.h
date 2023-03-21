// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Window/RouletteDataAsset.h"
#include "AckermanRouletteFunctions.generated.h"

class AStaticMeshActor;

USTRUCT(BlueprintType)
struct FRouletteResponseData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UStaticMesh* StaticMesh;
	UPROPERTY(BlueprintReadWrite)
	uint8 MeshIndex;
	UPROPERTY(BlueprintReadWrite)
	FString Message;
};



DECLARE_DELEGATE_OneParam(FRouletteDelegate, FRouletteResponseData /* Data */)
DECLARE_DELEGATE_OneParam(FRouletteFailureDelegate, FString /* Message */)

UCLASS()
class UAckermanRouletteFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	//UFUNCTION(Category = "Ackerman Roulette", meta = (DisplayName = "Async Spin Roulette", Keywords = "ackermanroulette ackerman roulette"))
	static void SpinRoulette(URouletteDataAsset* RouletteDataAsset, FRouletteDelegate OnSuccess = FRouletteDelegate(), FRouletteFailureDelegate OnFail = FRouletteFailureDelegate());
	
	//UFUNCTION(BlueprintCallable, Category = "Ackerman Roulette", meta = (DisplayName = "Spawn Mesh", Keywords = "ackermanroulette ackerman roulette", WorldContext="WorldContextObject"))
	static AStaticMeshActor* SpawnMesh(const UObject* WorldContextObject, UStaticMesh* Mesh, const FVector MeshSpawnPosition, FName FolderPath = FName(TEXT("")));
	
};
