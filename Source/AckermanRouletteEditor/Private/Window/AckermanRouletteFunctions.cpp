// Copyright Epic Games, Inc. All Rights Reserved.

#include "Window/AckermanRouletteFunctions.h"

#include "HttpModule.h"
#include "Window/RouletteDataAsset.h"
#include "Engine/AssetManager.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StreamableManager.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

UAckermanRouletteFunctions::UAckermanRouletteFunctions(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}


void UAckermanRouletteFunctions::SpinRoulette(URouletteDataAsset* RouletteDataAsset, FRouletteDelegate OnSuccess)
{
	UAckermanRouletteFunctions* newRouletteNode = NewObject<UAckermanRouletteFunctions>();
	
	if(!IsValid(RouletteDataAsset))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed Spinning Roulette: %s"), *FString("RouletteDataAsset is not valid."));
		return;
	}
		
	int meshCount = RouletteDataAsset->RouletteMeshes.Num();
	if(meshCount <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed Spinning Roulette: %s"), *FString("RouletteDataAsset is empty!."));
		return;
	}

	
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
				UE_LOG(LogTemp, Log, TEXT("Successfully Spun Roulette: %s %i!"), *FString("Returned mesh at index "), meshIndex);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed Spinning Roulette: %s %i!"), *FString("No static mesh found at element index"), meshIndex);
			}
		});
	};

	if(meshCount == 1) //Skip random number generation if only 1 element
		AsyncLoadMesh(0);
	else
	{
		//GET Random number over HTTP
		FHttpModule* Http = &FHttpModule::Get();
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

		FString urlString = FString::Printf(TEXT("https://www.random.org/integers/?num=1&min=0&max=%d&col=1&base=10&format=plain&rnd=new"), (meshCount-1));
		Request->SetURL(urlString);
		Request->SetVerb("GET");
		Request->SetHeader("Content-Type", "text/plain");
		Request->OnProcessRequestComplete().BindLambda([AsyncLoadMesh](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
		{
			if(bConnectedSuccessfully)
			{
				if(Response->GetResponseCode() == 200)
				{
					const FString ResponseBody = Response->GetContentAsString();
					uint8 randIdx = FCString::Atoi(*ResponseBody);
					AsyncLoadMesh(randIdx);	
				}
				else
					UE_LOG(LogTemp, Error, TEXT("Failed Spinning Roulette: HTTP Response code: %i. Response body: %s"), Response->GetResponseCode(), *Response->GetContentAsString());
			}
			else
				UE_LOG(LogTemp, Error, TEXT("Failed Spinning Roulette: %s"), *FString("Connection was not established. Check your internet connection"));			
		});
		
		Request->ProcessRequest();
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
