// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "UnrealEd.h"
#include "SlateExtras.h"
#include "Editor/LevelEditor/Public/LevelEditor.h"
#include "VPmame.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVPinball, Log, All);

class IVPinMAMEModuleListenerInterface
{
public:
	virtual ~IVPinMAMEModuleListenerInterface() {};
	virtual void OnStartupModule() {};
	virtual void OnShutdownModule() {};
};

class FUE4_VPinMAMEModule : public IModuleInterface
{
	UVPmame *pPinMame;
public:
	FUE4_VPinMAMEModule() { pPinMame = nullptr; }

	virtual void StartupModule();
	virtual void ShutdownModule();

	virtual void AddModuleListeners();

	static inline FUE4_VPinMAMEModule& Get()
    {
        return FModuleManager::LoadModuleChecked< FUE4_VPinMAMEModule >("UE4_VPinMAME");
    }

	void SetPinMAME(UVPmame *pMame) { pPinMame = pMame; };
	UVPmame *GetPinMAME() { return pPinMame; };

	void AddMenuExtension(const FMenuExtensionDelegate &extensionDelegate, FName extensionHook, const TSharedPtr<FUICommandList> &CommandList = nullptr, EExtensionHook::Position position = EExtensionHook::Before);

protected:
	TArray<TSharedRef<IVPinMAMEModuleListenerInterface>> ModuleListeners;
	TSharedPtr<FExtender> MenuExtender;

	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;

	static TSharedRef<FWorkspaceItem> MenuRoot;

	void MakePulldownMenu(FMenuBarBuilder &menuBuilder);
	void FillPulldownMenu(FMenuBuilder &menuBuilder);
 };
