// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimSequenceEditorTools.h"

#define LOCTEXT_NAMESPACE "FAnimSequenceEditorToolsModule"

void FAnimSequenceEditorToolsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FAnimSequenceEditorToolsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAnimSequenceEditorToolsModule, AnimSequenceEditorTools)