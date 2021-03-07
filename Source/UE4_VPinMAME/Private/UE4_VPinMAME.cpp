// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE4_VPinMAME.h"
//#include "VPinMAMEMenu/Public/VPinMAMEMenu.h"
#include "VPinMAMEMenu.h"

#define LOCTEXT_NAMESPACE "FUE4_VPinMAMEModule"

TSharedRef<FWorkspaceItem> FUE4_VPinMAMEModule::MenuRoot = FWorkspaceItem::NewGroup(FText::FromString("Menu Root"));

void FUE4_VPinMAMEModule::AddModuleListeners()
{
	ModuleListeners.Add(MakeShareable(new VPinMAMEMenu));
}

void FUE4_VPinMAMEModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (!IsRunningCommandlet())
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		LevelEditorMenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();
		MenuExtender = MakeShareable(new FExtender);
		MenuExtender->AddMenuBarExtension("Window", EExtensionHook::After, NULL, FMenuBarExtensionDelegate::CreateRaw(this, &FUE4_VPinMAMEModule::MakePulldownMenu));
		LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);
	}

		if (!IsRunningCommandlet())
		{
			AddModuleListeners();
			for (int32 i = 0; i < ModuleListeners.Num(); ++i)
			{
				ModuleListeners[i]->OnStartupModule();
			}
		}
	
}

void FUE4_VPinMAMEModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
		for (int32 i = 0; i < ModuleListeners.Num(); ++i)
		{
			ModuleListeners[i]->OnShutdownModule();
		}
	
}

void FUE4_VPinMAMEModule::AddMenuExtension(const FMenuExtensionDelegate &extensionDelegate, FName extensionHook, const TSharedPtr<FUICommandList> &CommandList, EExtensionHook::Position position)
{
	MenuExtender->AddMenuExtension(extensionHook, position, CommandList, extensionDelegate);
}

void FUE4_VPinMAMEModule::MakePulldownMenu(FMenuBarBuilder &menuBuilder)
{
	menuBuilder.AddPullDownMenu(
		FText::FromString("VPinMAME"),
		FText::FromString("Open the VPinMAME options menu"),
		FNewMenuDelegate::CreateRaw(this, &FUE4_VPinMAMEModule::FillPulldownMenu),
		"Example",
		FName(TEXT("ExampleMenu"))
	);
}

void FUE4_VPinMAMEModule::FillPulldownMenu(FMenuBuilder &menuBuilder)
{
	menuBuilder.BeginSection("cExampleSection", FText::FromString("VPinMAME Settings"));
	menuBuilder.AddMenuSeparator(FName("Section_1"));
	menuBuilder.EndSection();
}



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUE4_VPinMAMEModule, UE4_VPinMAME)


