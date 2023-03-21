#include "AckermanRouletteStyle.h"
#include "Slate/SlateGameResources.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyleMacros.h"
#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IPluginManager.h"

//#include "Styling/SlateStyle.h"
//#include "CoreMinimal.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FAckermanRouletteStyle::StyleInstance = nullptr;

void FAckermanRouletteStyle::Initialize()
{
	if(!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FAckermanRouletteStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

void FAckermanRouletteStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FAckermanRouletteStyle::Get()
{
	return *StyleInstance;
}

const FName FAckermanRouletteStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("AckermanRouletteStyle"));
	return StyleSetName;
}

TSharedRef<FSlateStyleSet> FAckermanRouletteStyle::Create()
{
	const FVector2D Icon32x32(32.0f, 32.0f);
	const FVector2D Icon16x16(16.0f, 16.0f);
	
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("AckermanRouletteStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("AckermanRoulette")->GetBaseDir() / TEXT("Resources"));
	FVector2D Icon20x20(20.0f, 20.0f);
	Style->Set("AckermanRoulette.PluginAction", new IMAGE_BRUSH_SVG(TEXT("Icon32"), Icon32x32));
	Style->Set("AckermanRoulette.TabIcon", new IMAGE_BRUSH_SVG( "Icon32", Icon16x16));
	
	return Style;
}
