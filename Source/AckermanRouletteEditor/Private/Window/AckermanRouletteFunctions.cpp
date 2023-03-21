// Copyright Epic Games, Inc. All Rights Reserved.

#include "Window/AckermanRouletteFunctions.h"
#include "Window/RouletteDataAsset.h"
#include "Engine/AssetManager.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StreamableManager.h"

UAckermanRouletteFunctions::UAckermanRouletteFunctions(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}


void UAckermanRouletteFunctions::SpinRoulette(URouletteDataAsset* RouletteDataAsset, FRouletteDelegate OnSuccess)
{
	UAckermanRouletteFunctions* newRouletteNode = NewObject<UAckermanRouletteFunctions>();
	
	int meshCount = RouletteDataAsset->RouletteMeshes.Num();
	
	auto AsyncLoadMesh = [RouletteDataAsset, OnSuccess](uint8 meshIndex)
	{
		const TSoftObjectPtr<UStaticMesh> softMeshPtr = RouletteDataAsset->RouletteMeshes[meshIndex];
		
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(softMeshPtr.ToSoftObjectPath(),
		[this, softMeshPtr, meshIndex, OnSuccess]()
		{
			FRouletteResponseData data = FRouletteResponseData();
			UStaticMesh* mesh = softMeshPtr.Get();
			if(mesh)
			{
				data.StaticMesh = mesh;
				data.MeshIndex = meshIndex;
				OnSuccess.Execute(data);
			}
		});
	};

	if(meshCount == 1) //Skip random number generation if only 1 element
		AsyncLoadMesh(0);
	else
	{
		//GET Random number over HTTP
		FRandomStream RandomStream(FMath::Rand());
		RandomStream.GenerateNewSeed();
		uint8 randIdx = RandomStream.RandRange(0, meshCount-1);
		AsyncLoadMesh(randIdx);
	}
}

AStaticMeshActor* UAckermanRouletteFunctions::SpawnMesh(const UObject* WorldContextObject, UStaticMesh* Mesh, const FVector MeshSpawnPosition, FName FolderPath)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if(!World || !Mesh)
		return nullptr;

	AStaticMeshActor* meshActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());

#if WITH_EDITOR
	meshActor->SetFolderPath(FolderPath);
#endif 
	
	meshActor->SetActorLabel(Mesh->GetName()/*.TrimChar('_')*/, true);
	meshActor->SetMobility(EComponentMobility::Movable);
	meshActor->SetActorLocation(MeshSpawnPosition);
	UStaticMeshComponent* meshComponent = meshActor->GetStaticMeshComponent();
	if(meshComponent)
	{
		meshComponent->SetStaticMesh(Mesh);
	}
	return meshActor;
}
