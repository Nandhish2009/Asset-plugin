// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UEAsset.h"
#include "UEAssetStyle.h"
#include "UEAssetCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "SlateApplication.h"
#include "PropertyCustomizationHelpers.h"
#include "IPluginManager.h"
#include "Materials/Material.h"
#include "PropertyEditor/Private/UserInterface/PropertyDetails/PropertyDetailsUtilities.h"

static const FName UEAssetTabName("UEAsset");

#define LOCTEXT_NAMESPACE "FUEAssetModule"

void FUEAssetModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FUEAssetStyle::Initialize();
	FUEAssetStyle::ReloadTextures();

	FUEAssetCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FUEAssetCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FUEAssetModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FUEAssetModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FUEAssetModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(UEAssetTabName, FOnSpawnTab::CreateRaw(this, &FUEAssetModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FUEAssetTabTitle", "UEAsset"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FUEAssetModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FUEAssetStyle::Shutdown();

	FUEAssetCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(UEAssetTabName);
}

TSharedRef<SDockTab> FUEAssetModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FUEAssetModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("UEAsset.cpp"))
	);

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FAssetThumbnailPool> ThumbnailPool = LevelEditorModule.GetFirstLevelEditor()->GetThumbnailPool();

	auto ret = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
		[
			SNew(SObjectPropertyEntryBox)
			.ObjectPath_Lambda([this]()->FString
			{
				return ObjPath;
			})

			.AllowedClass(UMaterial::StaticClass())
				.OnObjectChanged_Lambda([this](const FAssetData& asset)
			{
				ObjPath = asset.ObjectPath.ToString();
			})
			.DisplayBrowse(true)
			.PropertyHandle(PHandle)
			.DisplayThumbnail(true)
			.ThumbnailPool(ThumbnailPool)

		]
		];
	return ret;
}

void FUEAssetModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(UEAssetTabName);
}

void FUEAssetModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FUEAssetCommands::Get().OpenPluginWindow);
}

void FUEAssetModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FUEAssetCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUEAssetModule, UEAsset)