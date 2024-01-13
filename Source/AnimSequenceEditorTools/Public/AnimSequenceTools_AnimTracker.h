// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ANIMSEQUENCEEDITORTOOLS_API AnimSequenceTools_AnimTracker
{
public:
	AnimSequenceTools_AnimTracker();

	UAnimationAsset* GetSourceAnim();
	void SetSourceAnim(UAnimationAsset* source);

	TArray<UAnimationAsset*> GetSourceAnimAssets();
	void AddSourceAnimAsset(UAnimationAsset* source);

	IAssetEditorInstance* GetEditor();
	void SetEditor(IAssetEditorInstance* source);

private:

	UAnimationAsset* AnimationAsset;
	IAssetEditorInstance* AssetEditor;

	TArray<UAnimationAsset*> AnimationAssets;

};
