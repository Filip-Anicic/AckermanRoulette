#pragma once

class FAckermanRouletteStyle
{
public:

	static void Initialize();
	static void Shutdown();
	//Reload slate style textures
	static void ReloadTextures();

	/** @return The Slate style for the Ackerman's Roulette Plugin */
	static const ISlateStyle& Get();
	static const FName GetStyleSetName();

private:
	static TSharedRef<class FSlateStyleSet> Create();
	
	static TSharedPtr<class FSlateStyleSet> StyleInstance;
	
};
