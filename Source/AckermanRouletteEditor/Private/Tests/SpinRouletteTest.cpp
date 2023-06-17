#include "AITestsCommon.h"
#include "Misc/AutomationTest.h"
#include "Window/RouletteDataAsset.h"
#include "Window/AckermanRouletteFunctions.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SpinRouletteTest, "SpinRouletteTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool SpinRouletteTest::RunTest(const FString& Parameters)
{
	// Make the test pass by returning true, or fail by returning false.
	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpinRouletteTest, "SpinRouletteTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool WaitForCallback(TFunction<void()> Callback, float TimeToWait)
{
	const float StartTime = FPlatformTime::Seconds();
	while (FPlatformTime::Seconds() - StartTime < TimeToWait)
	{
		if (Callback)
		{
			Callback();
			return true;
		}
		FPlatformProcess::Sleep(0.01f);
	}
	return false;
}

bool RouletteSuccessCalled = false;
void OnSuccessCallback(FRouletteResponseData data)
{
	RouletteSuccessCalled = true;
}

bool RouletteFailCalled = false;
void OnFailCallback(FString message)
{
	RouletteFailCalled = true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FSpinRouletteCommand, URouletteDataAsset*, RouletteDataAsset, uint8, meshIndex);
bool FSpinRouletteCommand::Update()
{
	FString Result = UAckermanRouletteFunctions::SpinRoulette(RouletteDataAsset, meshIndex,
		FRouletteDelegate::CreateStatic(OnSuccessCallback),
		FRouletteFailureDelegate::CreateStatic(OnFailCallback));
	return WaitForCallback([&]()
	{
		//FTestHelpers::TestEqual("OnSuccess callback was called", RouletteSuccessCalled, true);
		//FTestHelpers::TestEqual("OnFail callback was not called", RouletteFailCalled, false);
		return RouletteFailCalled;
	}, 5.0f);
}

//IMPLEMENT_CUSTOM_LATENT_AUTOMATION_COMMAND(FSpinRouletteCommand, "SpinRoulette", "Spin the roulette", )

bool FSpinRouletteTest::RunTest(const FString& Parameters)
{
	RouletteSuccessCalled = false;
	RouletteFailCalled = false;
	URouletteDataAsset* rouletteDataAsset = NewObject<URouletteDataAsset>();

	ADD_LATENT_AUTOMATION_COMMAND(FSpinRouletteCommand(rouletteDataAsset, 0));

	return RouletteFailCalled;
}
