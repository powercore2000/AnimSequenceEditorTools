// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimSequenceTools_AnimTracker.h"

AnimSequenceTools_AnimTracker::AnimSequenceTools_AnimTracker()
{
}



void AnimSequenceTools_AnimTracker::SetSourceAnim(UAnimationAsset* source) {

	AnimationAsset = source;

}

UAnimationAsset* AnimSequenceTools_AnimTracker::GetSourceAnim() {

	return AnimationAsset;
}



void AnimSequenceTools_AnimTracker::AddSourceAnimAsset(UAnimationAsset* source) {

	AnimationAssets.Add(source);

}

TArray<UAnimationAsset*> AnimSequenceTools_AnimTracker::GetSourceAnimAssets() {

	return AnimationAssets;
}



void AnimSequenceTools_AnimTracker::SetEditor(IAssetEditorInstance* source) {

	AssetEditor = source;

}

IAssetEditorInstance* AnimSequenceTools_AnimTracker::GetEditor() {

	return AssetEditor;
}


