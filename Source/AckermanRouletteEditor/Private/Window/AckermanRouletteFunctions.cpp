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




void UAckermanRouletteFunctions::GetRandomNumber(uint8 min, uint8 max, FWebDelegate OnSuccess, FRouletteFailureDelegate OnFail)
{
	//GET Random number over HTTP
	FHttpModule* Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

	FString urlString = FString::Printf(TEXT("https://www.random.org/integers/?num=1&min=0&max=%d&col=1&base=10&format=plain&rnd=new"), max);
	Request->SetURL(urlString);
	Request->SetVerb("GET");
	Request->SetHeader("Content-Type", "text/plain");
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnFail](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		if(bConnectedSuccessfully)
		{
			if(Response->GetResponseCode() == 200)
			{
				const FString ResponseBody = Response->GetContentAsString();
				uint8 randIdx = FCString::Atoi(*ResponseBody);
				OnSuccess.Execute(randIdx);	
			}
			else
			{
				FString message = FString::Printf(TEXT("Failed Spinning Roulette: HTTP Response code: %i. Response body: %s"), Response->GetResponseCode(), *Response->GetContentAsString());
				UE_LOG(LogTemp, Error, TEXT("%s"), *message);
				OnFail.Execute(message);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed Spinning Roulette: Connection was not established. Check your internet connection"));
			OnFail.Execute("Failed Spinning Roulette: Connection was not established. Check your internet connection");
		}
	});
		
	Request->ProcessRequest();
}




FString UAckermanRouletteFunctions::SpinRoulette(URouletteDataAsset* RouletteDataAsset, uint8 MeshIndex, FRouletteDelegate OnSuccess, FRouletteFailureDelegate OnAsyncFail)
{
	if(!IsValid(RouletteDataAsset))
	{
		FString error = FString("RouletteDataAsset is not valid");
		UE_LOG(LogTemp, Error, TEXT("Failed Spinning Roulette: %s"), *error);
		return error;
	}
		
	int meshCount = RouletteDataAsset->RouletteMeshes.Num();
	if(meshCount <= 0)
	{
		FString error = FString("RouletteDataAsset is empty");
		UE_LOG(LogTemp, Error, TEXT("Failed Spinning Roulette: %s"), *error);
		return error;
	}
	
	if(MeshIndex >= meshCount)
	{
		FString error = FString("MeshIndex is out bounds of the array");
		UE_LOG(LogTemp, Error, TEXT("Failed Spinning Roulette: %s"), *error);
		return error;
	}
	
	const TSoftObjectPtr<UStaticMesh> softMeshPtr = RouletteDataAsset->RouletteMeshes[MeshIndex];
		
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(softMeshPtr.ToSoftObjectPath(),
	[softMeshPtr, MeshIndex, OnSuccess, OnAsyncFail]()
	{
		FRouletteResponseData data = FRouletteResponseData();
		UStaticMesh* mesh = softMeshPtr.Get();
		if(mesh)
		{
			data.StaticMesh = mesh;
			data.MeshIndex = MeshIndex;
			OnSuccess.Execute(data);
			UE_LOG(LogTemp, Log, TEXT("Successfully Spun Roulette: Returned mesh at index  %i!"), MeshIndex);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed Spinning Roulette: No static mesh found at element index %i!"), MeshIndex);
			OnAsyncFail.Execute("No static mesh found at element index");
		}
	});

	return "";
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
	
	meshActor->SetActorLabel(Mesh->GetName(), true);
	meshActor->SetMobility(EComponentMobility::Movable);
	meshActor->SetActorLocation(MeshSpawnPosition);
	UStaticMeshComponent* meshComponent = meshActor->GetStaticMeshComponent();
	if(meshComponent)
	{
		meshComponent->SetStaticMesh(Mesh);
	}
	return meshActor;
}
