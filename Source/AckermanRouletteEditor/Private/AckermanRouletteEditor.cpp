// Copyright Epic Games, Inc. All Rights Reserved.


#include "AckermanRouletteEditor.h"
#include "AckermanRouletteCommands.h"
#include "AckermanRouletteStyle.h"
#include "ISettingsModule.h"
#include "ToolMenus.h"
#include "Subsystems/UnrealEditorSubsystem.h"
#include "Window/AckermanRouletteToolSettings.h"
#include "Window/SAckermanRouletteWidget.h"

static const FName AckermanRouletteName("Ackerman's Roulette");

#define LOCTEXT_NAMESPACE "FAckermanRouletteModule"

void FAckermanRouletteEditorModule::StartupModule()
{
	//Register settings
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule)
	{
		//Load settings from .ini file
		UAckermanRouletteToolSettings* settings = GetMutableDefault<UAckermanRouletteToolSettings>();
		settings->LoadConfig();
		
		SettingsModule->RegisterSettings("Project", "Plugins", "Ackerman's Roulette",
			FText::FromString("Ackerman's Roulette"),
			FText::FromString("Configure Ackerman's Roulette Settings"),
			settings
		);
	}
	
	//Initialize style
	FAckermanRouletteStyle::Initialize();
	FAckermanRouletteStyle::ReloadTextures();
	
	//Register Commands
	FAckermanRouletteCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);
	
	PluginCommands->MapAction(
		FAckermanRouletteCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FAckermanRouletteEditorModule::RouletteButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAckermanRouletteEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AckermanRouletteName,
		FOnSpawnTab::CreateRaw(this, &FAckermanRouletteEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FAckermanRouletteImporterTabTitle", "Ackerman's Roulette"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FAckermanRouletteEditorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	//Shutdown style
	FAckermanRouletteStyle::Shutdown();
	
	//Unregister commands
	FAckermanRouletteCommands::Unregister();

	//Unregister settings
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule)
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Ackerman's Roulette");
	}
}

void FAckermanRouletteEditorModule::RouletteButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(AckermanRouletteName);
	UE_LOG(LogTemp, Display, TEXT("Open Ackerman's Roulette"));
}

void FAckermanRouletteEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FAckermanRouletteCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FAckermanRouletteCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

TSharedRef<SDockTab> FAckermanRouletteEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	//Make Roulette widget
	RouletteWidget = SNew(SAckermanRouletteWidget);
	
	//Bind event OnClicked SButton
	RouletteWidget->SpinButton->SetOnClicked(FOnClicked::CreateRaw(this, &FAckermanRouletteEditorModule::OnRouletteButtomClicked));
	
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[ RouletteWidget.ToSharedRef() ];
}

FReply FAckermanRouletteEditorModule::OnRouletteButtomClicked()
{
	UE_LOG(LogTemp, Display, TEXT("Spin Ackerman's Roulette!"));
	//Syncronously load Roulette Widget's selected data asset
	TObjectPtr<URouletteDataAsset> dataAsset = RouletteWidget->GetCustomSettings()->DataAsset.LoadSynchronous();

	//Get random number with web request
	uint8 maxInt = dataAsset->RouletteMeshes.Num() - 1;
	UAckermanRouletteFunctions::GetRandomNumber(0, maxInt,
		FWebDelegate::CreateLambda([dataAsset, this](uint8 meshIndex)
		{
			//Spin roulette and call OnRouletteMeshLoaded on success
			UAckermanRouletteFunctions::SpinRoulette(dataAsset, meshIndex,
				FRouletteDelegate::CreateRaw(this, &FAckermanRouletteEditorModule::OnRouletteMeshLoaded));	
		}));
	
	return FReply::Handled();
}

void FAckermanRouletteEditorModule::OnRouletteMeshLoaded(FRouletteResponseData Data)
{
	UUnrealEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>();
	UObject* worldContext = EditorSubsystem->GetEditorWorld();
	UAckermanRouletteToolSettings* toolSettings = RouletteWidget->GetCustomSettings();
	UAckermanRouletteFunctions::SpawnMesh(worldContext, Data.StaticMesh, toolSettings->SpawnLocation, toolSettings->SpawnFolder);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAckermanRouletteEditorModule, AckermanRouletteEditor)