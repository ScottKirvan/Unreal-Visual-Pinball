// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "UnrealEd.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Editor/LevelEditor/Public/LevelEditor.h"
#include "Editor/PropertyEditor/Public/PropertyEditing.h"
#include "IAssetTypeActions.h"

class IVPinMAMEModuleListenerInterface
{
public:
	virtual void OnStartupModule() {};
	virtual void OnShutdownModule() {};
};

class FUE4_VPinMAMEModule : public IModuleInterface
{
public:

	virtual void StartupModule();
	virtual void ShutdownModule();

	virtual void AddModuleListeners() {};



	void AddMenuExtension(const FMenuExtensionDelegate &extensionDelegate, FName extensionHook, const TSharedPtr<FUICommandList> &CommandList = NULL, EExtensionHook::Position position = EExtensionHook::Before);

protected:
	TArray<TSharedRef<IVPinMAMEModuleListenerInterface>> ModuleListeners;
	TSharedPtr<FExtender> MenuExtender;

	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;

	static TSharedRef<FWorkspaceItem> MenuRoot;

	void MakePulldownMenu(FMenuBarBuilder &menuBuilder);
	void FillPulldownMenu(FMenuBuilder &menuBuilder);
 };
