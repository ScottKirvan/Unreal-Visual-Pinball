// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "UnrealEd.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Editor/LevelEditor/Public/LevelEditor.h"
#include "Editor/PropertyEditor/Public/PropertyEditing.h"
#include "IAssetTypeActions.h"
#include "VPmame.h"

class IVPinMAMEModuleListenerInterface
{
public:
	virtual void OnStartupModule() {};
	virtual void OnShutdownModule() {};
};

class FUE4_VPinMAMEModule : public IModuleInterface
{
	UVPmame *pPinMame;
public:

	virtual void StartupModule();
	virtual void ShutdownModule();

	virtual void AddModuleListeners();

	static inline FUE4_VPinMAMEModule& Get()
    {
        return FModuleManager::LoadModuleChecked< FUE4_VPinMAMEModule >("UE4_VPinMAME");
    }

	void SetPinMAME(UVPmame *pMame) { pPinMame = pMame; };
	UVPmame *GetPinMAME() { return pPinMame; };

	void AddMenuExtension(const FMenuExtensionDelegate &extensionDelegate, FName extensionHook, const TSharedPtr<FUICommandList> &CommandList = NULL, EExtensionHook::Position position = EExtensionHook::Before);

protected:
	TArray<TSharedRef<IVPinMAMEModuleListenerInterface>> ModuleListeners;
	TSharedPtr<FExtender> MenuExtender;

	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;

	static TSharedRef<FWorkspaceItem> MenuRoot;

	void MakePulldownMenu(FMenuBarBuilder &menuBuilder);
	void FillPulldownMenu(FMenuBuilder &menuBuilder);
 };
