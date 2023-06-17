/*#include <gtest/gtest.h>
#include "AckermanRouletteEditor/Public/Window/AckermanRouletteFunctions.h"

class SpinRouletteTest : public testing::Test {
protected:
	void SetUp() override {
		// Set up any objects or variables needed for the tests
	}
    
	void TearDown() override {
		// Clean up any objects or variables used for the tests
	}
};

TEST_F(SpinRouletteTest, TestWithEmptyRouletteDataAsset) {
	// Create a URouletteDataAsset with an empty array
	URouletteDataAsset* RouletteDataAsset = NewObject<URouletteDataAsset>();
    
	// Call the SpinRoulette function with the OnFail callback
	FString ResponseMessage = UAckermanRouletteFunctions::SpinRoulette(RouletteDataAsset, 2);
    
	// Check if the error message matches the expected output
	ASSERT_EQ(ResponseMessage, FString("RouletteDataAsset is empty"));
}

// Add more tests here if needed





int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}*/