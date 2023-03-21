// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Window/AckermanRouletteFunctions.h"
#include "Window/SAckermanRouletteWidget.h"

class FAckermanRouletteEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	//Function called bound to toolbar button
	void RouletteButtonClicked();
	
private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<SAckermanRouletteWidget> RouletteWidget;
	
	void RegisterMenus();
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	FReply OnRouletteButtomClicked();
    void OnRouletteMeshLoaded(FRouletteResponseData Data);
};
