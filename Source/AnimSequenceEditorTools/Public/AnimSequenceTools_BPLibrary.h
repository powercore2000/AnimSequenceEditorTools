// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Animation/AnimSequence.h"
#include "AnimSequenceTools_AnimTracker.h"





#if WITH_EDITOR


#include "Editor.h"
#include "UnrealEd.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AnimationEditorUtils.h"

#include "IAnimationEditor.h"
#include "IPersonaToolkit.h"
#include "ISequenceRecorder.h"
#endif

#include "AnimSequenceTools_BPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ANIMSEQUENCEEDITORTOOLS_API UAnimSequenceTools_BPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "AnimSequenceTools")
		static bool OpenEditorWindowsForAssets(const TArray<UObject*> NewAssets);

	UFUNCTION(BlueprintCallable, Category = "AnimSequenceTools")
		static void CloneAnimSequences(const TArray<UObject*> NewAssets);


	static IAssetEditorInstance* OpenServiceEditor(const TArray<UObject*> NewAssets);

	static bool AnimFactoryCloningEditorSwap(const TArray<UObject*> NewAssets, TWeakPtr<AnimSequenceTools_AnimTracker> AnimTrackerPtr);

	static void AnimFactoryEditorSwap(const TArray<UObject*> NewAssets);






	static void AnimFactoryCloning(const TArray<UObject*> NewAssets, TWeakPtr<AnimSequenceTools_AnimTracker> AnimTrackerPtr);

	static void AnimFactory(const TArray<UObject*> NewAssets);


	static bool AnimFactoryCloningNoEditor(const TArray<UObject*> NewAssets, TWeakPtr<AnimSequenceTools_AnimTracker> AnimTrackerPtr);

	static void AnimFactoryNoEditor(const TArray<UObject*> NewAssets);

private:
	static bool HandleAssetCreated(const TArray<UObject*> NewAssets);
	static bool HandleAssetsArrayCreated(const TArray<UObject*> NewAssets);


};
