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
DECLARE_DELEGATE_OneParam(FWebDelegate, uint8 /* Data */)
DECLARE_DELEGATE_OneParam(FRouletteFailureDelegate, FString /* Message */)

UCLASS()
class UAckermanRouletteFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	
	static void GetRandomNumber(uint8 min, uint8 max, FWebDelegate OnSuccess = FWebDelegate(), FRouletteFailureDelegate OnFail = FRouletteFailureDelegate());

	static FString SpinRoulette(URouletteDataAsset* RouletteDataAsset, uint8 MeshIndex, FRouletteDelegate OnSuccess = FRouletteDelegate(), FRouletteFailureDelegate OnAsyncFail = FRouletteFailureDelegate());
	
	static AStaticMeshActor* SpawnMesh(const UObject* WorldContextObject, UStaticMesh* Mesh, const FVector MeshSpawnPosition, FName FolderPath = FName(TEXT("")));
	
};
