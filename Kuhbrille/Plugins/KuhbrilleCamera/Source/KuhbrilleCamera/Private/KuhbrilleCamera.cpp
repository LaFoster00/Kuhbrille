// Copyright Epic Games, Inc. All Rights Reserved.

#include "KuhbrilleCamera.h"
#include "KuhbrilleCameraStyle.h"
#include "KuhbrilleCameraCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName KuhbrilleCameraTabName("KuhbrilleCamera");

#define LOCTEXT_NAMESPACE "FKuhbrilleCameraModule"

void FKuhbrilleCameraModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FKuhbrilleCameraStyle::Initialize();
	FKuhbrilleCameraStyle::ReloadTextures();

	FKuhbrilleCameraCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FKuhbrilleCameraCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FKuhbrilleCameraModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FKuhbrilleCameraModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(KuhbrilleCameraTabName, FOnSpawnTab::CreateRaw(this, &FKuhbrilleCameraModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FKuhbrilleCameraTabTitle", "KuhbrilleCamera"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FKuhbrilleCameraModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FKuhbrilleCameraStyle::Shutdown();

	FKuhbrilleCameraCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(KuhbrilleCameraTabName);
}

TSharedRef<SDockTab> FKuhbrilleCameraModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FKuhbrilleCameraModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("KuhbrilleCamera.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FKuhbrilleCameraModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(KuhbrilleCameraTabName);
}

void FKuhbrilleCameraModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FKuhbrilleCameraCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FKuhbrilleCameraCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FKuhbrilleCameraModule, KuhbrilleCamera)
