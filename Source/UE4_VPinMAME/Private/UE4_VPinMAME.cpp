// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE4_VPinMAME.h"

#define LOCTEXT_NAMESPACE "FUE4_VPinMAMEModule"

void FUE4_VPinMAMEModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FUE4_VPinMAMEModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUE4_VPinMAMEModule, UE4_VPinMAME)