// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Animation/AnimSequence.h"
#include "AnimSequenceTools_AnimTracker.h"
//#include "GenericPlatformMisc.h"




#if WITH_EDITOR


#include "Editor.h"
#include "UnrealEd.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AnimationEditorUtils.h"

#include "IAnimationEditor.h"
#include "IPersonaToolkit.h"
#include "ISequenceRecorder.h"
//#include "SAnimationDlgs.h"

//#include "DataAssetFactory.h"
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
		static bool OpenEditorWindowsForAssets(const TArray<UObject*> SelectedAssets);

	UFUNCTION(BlueprintCallable, Category = "AnimSequenceTools")
		static void CloneAnimSequences(const TArray<UObject*> SelectedAssets);

	UFUNCTION(BlueprintCallable, Category = "AnimSequenceTools")
		static void CloneAnimSequencesTargeted(const TArray<UObject*> SelectedAssets);
	
	UFUNCTION(BlueprintCallable, Category = "AnimSequenceTools")
		static void CloneAnimSequencesToDir(const TArray<UObject*> SelectedAssets, const FString outputPath = "Game/DuplicatedAnimations/AssetName/");


		


	static IAssetEditorInstance* OpenServiceEditor(const TArray<UObject*> NewAssets);

	static void StartBatchAnimFactoryToDir(const TArray<UObject*> SelectedAssets, const FString* outputPath);

	static void StartBatchAnimFactory(const TArray<UObject*> SelectedAssets);

	static bool BatchAnimSeqCloner(const TArray<UObject*> NewAssets, TWeakPtr<AnimSequenceTools_AnimTracker> AnimTrackerPtr);

	//UFUNCTION(BlueprintCallable, Category = "AnimSequenceTools")
		//static FString OpenFilePathSelectionModal();


	static void StartTargetedAnimFactory(const TArray<UObject*> SelectedAssets);

	static bool TargetedAnimSeqCloner(const TArray<UObject*> NewAssets, TWeakPtr<AnimSequenceTools_AnimTracker> AnimTrackerPtr);




private:
	static bool HandleAssetCreated(const TArray<UObject*> NewAssets);
	static bool HandleAssetsArrayCreated(const TArray<UObject*> NewAssets);


};
