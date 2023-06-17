/*#include "Engine/AssetManager.h"
#include "Misc/AutomationTest.h"
#include "Window/AckermanRouletteFunctions.h"
#include "Window/RouletteDataAsset.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SpinRouletteEmptyTest, "AckermanRoulette.RouletteFunctionsTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)


bool SpinRouletteEmptyTest::RunTest(const FString& Parameters)
{
	// Create a URouletteDataAsset with an empty array
	URouletteDataAsset* RouletteDataAsset = NewObject<URouletteDataAsset>();
    
	// Call the SpinRoulette function with the OnFail callback
	FString ResponseMessage = UAckermanRouletteFunctions::SpinRoulette(RouletteDataAsset, 2);
    
	// Check if the error message matches the expected output
	return ResponseMessage == FString("RouletteDataAsset is empty");
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(SpinRouletteMeshReturnTest, "AckermanRoulette.SpinRouletteMeshReturnTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool SpinRouletteMeshReturnTest::RunTest(const FString& Parameters)
{
	// Create a URouletteDataAsset with an empty array
	URouletteDataAsset* RouletteDataAsset = NewObject<URouletteDataAsset>();
	UStaticMesh* mockMesh0 = NewObject<UStaticMesh>();
	UStaticMesh* mockMesh1 = NewObject<UStaticMesh>();
	TObjectPtr<UStaticMesh> mockPtr = mockMesh1;

	RouletteDataAsset->RouletteMeshes.Add(mockMesh0);
	RouletteDataAsset->RouletteMeshes.Add(mockMesh1);
	

	// Define the OnSuccess callback function
	TObjectPtr<UStaticMesh> returnedMeshPtr;
	auto OnSuccessCallback = [&](const FRouletteResponseData& data) {
		returnedMeshPtr = data.StaticMesh;
	};
    
	// Call the SpinRoulette function with the OnFail callback
	FString ResponseMessage = UAckermanRouletteFunctions::SpinRoulette(RouletteDataAsset, 1,
		FRouletteDelegate::CreateLambda(OnSuccessCallback));

	
	// Check if the error message matches the expected output
	return returnedMeshPtr == mockPtr;
}

#include "Misc/AutomationTest.h"
#include "Http.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHttpTest, "Game.Test.HttpRequest", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FHttpTest::RunTest(const FString& Parameters)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL("http://example.com");
	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSuccess) {
		TestEqual("HTTP Response Code", HttpResponse->GetResponseCode(), 200);
		TestTrue("HTTP Request was successful", bSuccess);
		//SetTestPhaseCompleted();
	});

	Request->ProcessRequest();

	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND(FHttpTestLatent);

bool FHttpTestLatent::Update()
{
	return false;
}*/


/*TEST(SpinRouletteTest, TestWithEmptyRouletteDataAsset)
{
	// Create a URouletteDataAsset with an empty array
	URouletteDataAsset* RouletteDataAsset = NewObject<URouletteDataAsset>();
    
	// Define the OnFail callback function
	FString ErrorMessage;
	auto OnFailCallback = [&](const FString& Error) {
		ErrorMessage = Error;
	};
    
	// Call the SpinRoulette function with the OnFail callback
	UAckermanRouletteFunctions::SpinRoulette(RouletteDataAsset, FRouletteDelegate(),
		FRouletteFailureDelegate::CreateLambda(OnFailCallback));
    
	// Check if the error message matches the expected output
	ASSERT_EQ(ErrorMessage, FString("RouletteDataAsset is empty"));
}*/
