// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimSequenceTools_BPLibrary.h"

//Entry point blueprints function
void UAnimSequenceTools_BPLibrary::CloneAnimSequences(const TArray<UObject*> NewAssets) {

	AnimFactoryEditorSwap(NewAssets);
}


/*
//Activates the AnimSequence Factory and iterates through each selected asset passed in
void UAnimSequenceTools_BPLibrary::AnimFactory(const TArray<UObject*> NewAssets) {

	//Grab the editor window insntace
	const bool bDoNotShowNameDialog = false;
	const bool bAllowReplaceExisting = true;
	bool bBringToFrontIfOpen = true;

	UE_LOG(LogTemp, Display, TEXT("Using The editor"));
	TArray<UAnimationAsset*> AnimAssets;
	TArray<TSoftObjectPtr<UObject>> Objects;

	for (auto ObjIt = NewAssets.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UAnimationAsset* AnimAsset = Cast<UAnimationAsset>(*ObjIt))
		{
			AnimAssets.Add(AnimAsset);
		}

	}

	for (UAnimationAsset* AnimAsset : AnimAssets)
	{
		Objects.Add(AnimAsset->GetSkeleton());
		IAssetEditorInstance* AnimationEditorInstance = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->FindEditorForAsset(AnimAsset, bBringToFrontIfOpen);

		TSharedPtr<AnimSequenceTools_AnimTracker> AnimSeqPtr = MakeShareable(new AnimSequenceTools_AnimTracker());
		AnimSeqPtr->SetSourceAnim(AnimAsset);
		TWeakPtr<AnimSequenceTools_AnimTracker> WeakPtr = AnimSeqPtr.ToWeakPtr();
		//seqTracker.SetAnimationRef(AnimAsset);

		//Assign Source Anim Asset		
		if (AnimationEditorInstance != nullptr) {

			AnimationEditorInstance->FocusWindow();
			AnimationEditorUtils::ExecuteNewAnimAsset<UAnimSequenceFactory, UAnimSequence>(Objects, FString("_Sequence"),
				FAnimAssetCreated::CreateStatic(&UAnimSequenceTools_BPLibrary::AnimFactoryCloning, WeakPtr),
				bDoNotShowNameDialog, bAllowReplaceExisting);
		}

		else {
			UE_LOG(LogTemp, Error, TEXT("Pointer to original editor invalid!"));
		}

		Objects.Empty();
	}

}

//Handles the animation cloning process
void UAnimSequenceTools_BPLibrary::AnimFactoryCloning(const TArray<UObject*> NewAssets, TWeakPtr<AnimSequenceTools_AnimTracker> AnimTrackerPtr) {

	//Grab the editor window insntace
	bool bResult = true;
	bool bBringToFrontIfOpen = true;

#if WITH_EDITOR
	//UE_LOG(LogTemp, Display, TEXT("%s"), localInt);
	UE_LOG(LogTemp, Display, TEXT("Running CloneAnimSequenceInstance %"), NewAssets.Num());

	UAnimationAsset* TargetAnimAsset = Cast<UAnimationAsset>(NewAssets[0]);
	UAnimationAsset* SourceAnimAsset = AnimTrackerPtr.Pin()->GetSourceAnim();


	IAssetEditorInstance* EditorInstance = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->FindEditorForAsset(SourceAnimAsset, bBringToFrontIfOpen);
	if (EditorInstance != nullptr)
	{
		//Focuses to window
		UE_LOG(LogTemp, Display, TEXT("Found editor window for asset %s"), *TargetAnimAsset->GetName());

		if (EditorInstance->GetEditorName() == FName("AnimationEditor"))
		{

			//Change the current anim to this one
			IAnimationEditor* AnimEditor = static_cast<IAnimationEditor*>(EditorInstance);
			TSharedRef<IPersonaToolkit> PersonaToolkit = AnimEditor->GetPersonaToolkit();
			USkeletalMeshComponent* MeshComponent = PersonaToolkit->GetPreviewMeshComponent();
			UAnimSequence* Sequence = Cast<UAnimSequence>(TargetAnimAsset);
			UE_LOG(LogTemp, Display, TEXT("Ready to process asset with component %s"), *MeshComponent->GetName());
			//Sequence is new Fbx passed in via object
			//MeshComponent must come from old Ui

			ISequenceRecorder& RecorderModule = FModuleManager::Get().LoadModuleChecked<ISequenceRecorder>("SequenceRecorder");
			bResult &= RecorderModule.RecordSingleNodeInstanceToAnimation(MeshComponent, Sequence, true);

		}


	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Animation Window Found Anim Asset!"));
	}

	if (bResult) {

		FAssetRegistryModule::AssetCreated(TargetAnimAsset);
	}

#endif

}

//Activates the AnimSequence Factory and iterates through each selected asset passed in
void UAnimSequenceTools_BPLibrary::AnimFactoryNoEditor(const TArray<UObject*> NewAssets) {

	//Grab the editor window insntace
	const bool bDoNotShowNameDialog = false;
	const bool bAllowReplaceExisting = true;
	bool bBringToFrontIfOpen = true;

	UE_LOG(LogTemp, Display, TEXT("Using no editor"));
	TArray<UAnimationAsset*> AnimAssets;
	TArray<TSoftObjectPtr<UObject>> Objects;

	for (auto ObjIt = NewAssets.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UAnimationAsset* AnimAsset = Cast<UAnimationAsset>(*ObjIt))
		{
			AnimAssets.Add(AnimAsset);
		}

	}

	for (UAnimationAsset* AnimAsset : AnimAssets)
	{
		Objects.Add(AnimAsset->GetSkeleton());


		UDebugSkelMeshComponent* SourceAnimSkeleton = NewObject<UDebugSkelMeshComponent>(AnimAsset->GetSkeleton());


		TSharedPtr<AnimSequenceTools_AnimTracker> AnimSeqPtr = MakeShareable(new AnimSequenceTools_AnimTracker());
		AnimSeqPtr->SetSourceAnim(AnimAsset);
		TWeakPtr<AnimSequenceTools_AnimTracker> WeakPtr = AnimSeqPtr.ToWeakPtr();
		//seqTracker.SetAnimationRef(AnimAsset);

		//Assign Source Anim Asset
		if (SourceAnimSkeleton != nullptr) {

			AnimationEditorUtils::ExecuteNewAnimAsset<UAnimSequenceFactory, UAnimSequence>(Objects, FString("_Sequence"),
				FAnimAssetCreated::CreateStatic(&UAnimSequenceTools_BPLibrary::AnimFactoryCloningNoEditor, WeakPtr),
				bDoNotShowNameDialog, bAllowReplaceExisting);
		}

		else {
			UE_LOG(LogTemp, Error, TEXT("Pointer to original asset skeleton is not found!"));
		}

		Objects.Empty();
	}

}

//Handles the animation cloning process
bool UAnimSequenceTools_BPLibrary::AnimFactoryCloningNoEditor(const TArray<UObject*> NewAssets, TWeakPtr<AnimSequenceTools_AnimTracker> AnimTrackerPtr) {

	//Grab the editor window insntace
	bool bResult = true;
	bool bBringToFrontIfOpen = true;

#if WITH_EDITOR
	//UE_LOG(LogTemp, Display, TEXT("%s"), localInt);
	UE_LOG(LogTemp, Display, TEXT("Running CloneAnimSequenceInstance %"), NewAssets.Num());

	UAnimationAsset* TargetAnimAsset = Cast<UAnimationAsset>(NewAssets[0]);
	UAnimationAsset* SourceAnimAsset = AnimTrackerPtr.Pin()->GetSourceAnim();
	USkeletalMeshComponent* SourceAnimSkeleton = NewObject<USkeletalMeshComponent>(SourceAnimAsset->GetSkeleton());
	//AnimScriptInstance = NewObject<UAnimInstance>(this, AnimClass);
	SourceAnimSkeleton->InitializeAnimScriptInstance();
	if (SourceAnimSkeleton != nullptr)
	{
		//Focuses to window
		UE_LOG(LogTemp, Display, TEXT("Found Skeleton for asset %s"), *TargetAnimAsset->GetName());


			//Change the current anim to this one
			UAnimSequence* Sequence = Cast<UAnimSequence>(TargetAnimAsset);
			UE_LOG(LogTemp, Display, TEXT("Ready to process asset with component %s"), *SourceAnimSkeleton->GetName());
			//Sequence is new Fbx passed in via object
			//MeshComponent must come from old Ui

			ISequenceRecorder& RecorderModule = FModuleManager::Get().LoadModuleChecked<ISequenceRecorder>("SequenceRecorder");
			UE_LOG(LogTemp, Display, TEXT("About to record"));
			bResult &= RecorderModule.RecordSingleNodeInstanceToAnimation(SourceAnimSkeleton, Sequence, true);
			UE_LOG(LogTemp, Display, TEXT("Processing status: %s"), bResult);



	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Animation Window Found Anim Asset!"));
	}

	if (bResult) {

		FAssetRegistryModule::AssetCreated(TargetAnimAsset);
	}

#endif

	return bResult;
}

*/


IAssetEditorInstance* UAnimSequenceTools_BPLibrary::OpenServiceEditor(const TArray<UObject*> NewAssets) {

	TArray<UObject*> FirstAssetArray;

	FirstAssetArray.Add(NewAssets[0]);
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
	TWeakPtr<IAssetTypeActions> AssetTypeActions = AssetToolsModule.Get().GetAssetTypeActionsForClass(FirstAssetArray[0]->GetClass());

	if (AssetTypeActions.IsValid())
	{
		AssetTypeActions.Pin()->OpenAssetEditor(FirstAssetArray);
		return GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->FindEditorForAsset(NewAssets[0], true);
	}

	return nullptr;
}






//Activates the AnimSequence Factory and iterates through each selected asset passed in
void UAnimSequenceTools_BPLibrary::AnimFactoryEditorSwap(const TArray<UObject*> NewAssets) {

	//Grab the editor window insntace
	const bool bDoNotShowNameDialog = false;
	const bool bAllowReplaceExisting = true;
	bool bBringToFrontIfOpen = true;

	UE_LOG(LogTemp, Display, TEXT("Using The editor"));
	TArray<UAnimationAsset*> AnimAssets;
	TArray<TSoftObjectPtr<UObject>> Objects;	

	TSharedPtr<AnimSequenceTools_AnimTracker> AnimTrackerPtr = MakeShareable(new AnimSequenceTools_AnimTracker());
	TWeakPtr<AnimSequenceTools_AnimTracker> WeakPtr = AnimTrackerPtr.ToWeakPtr();

	for (auto ObjIt = NewAssets.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UAnimationAsset* AnimAsset = Cast<UAnimationAsset>(*ObjIt))
		{
			Objects.Add(AnimAsset->GetSkeleton());			
			AnimAssets.Add(AnimAsset);
			AnimTrackerPtr->AddSourceAnimAsset(AnimAsset);
		}

	}


	IAssetEditorInstance* AnimationEditorInstance = OpenServiceEditor(NewAssets);	
	AnimTrackerPtr->SetEditor(AnimationEditorInstance);
	UAnimSequenceFactory factoryInst = UAnimSequenceFactory();
	factoryInst.ConfigureProperties();
	//Assign Source Anim Asset		
	if (AnimationEditorInstance != nullptr) {
		/*
		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
		
		FString Name, PackageName;
		AssetToolsModule.Get().CreateUniqueAssetName(TEXT("/Game/AssetFolder/AssetName"), TEXT(""), PackageName, Name);
		const FString PackagePath = FPackageName::GetLongPackagePath(PackageName);
		UObject* NewObject = AssetToolsModule.Get().CreateAsset(Name, PackagePath, UBlueprint::StaticClass());
		UPackage::Save(package, NewObject, RF_Public | RF_Standalone, *FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension()));

		// Inform asset registry
		AssetRegistry.AssetCreated(NewObject);

		// Tell content browser to show the newly created asset
		TArray<UObject*> Objects;
		Objects.Add(NewObject);
		ContentBrowserModule.Get().SyncBrowserToAssets(Objects);

		// Inform asset registry
		AssetRegistry.AssetCreated(NewObject);
		*/
		
		
		AnimationEditorInstance->FocusWindow();
		AnimationEditorUtils::ExecuteNewAnimAsset<UAnimSequenceFactory, UAnimSequence>(Objects, FString("_Sequence"),
			FAnimAssetCreated::CreateStatic(&UAnimSequenceTools_BPLibrary::AnimFactoryCloningEditorSwap, WeakPtr),
			bDoNotShowNameDialog, bAllowReplaceExisting);
		
		AnimTrackerPtr->GetEditor()->CloseWindow();
	}

	else {
		UE_LOG(LogTemp, Error, TEXT("Pointer to original editor invalid!"));
	}


	
}


bool UAnimSequenceTools_BPLibrary::AnimFactoryCloningEditorSwap(const TArray<UObject*> NewAssets, TWeakPtr<AnimSequenceTools_AnimTracker> AnimTrackerPtr) {

	//Grab the editor window insntace
	bool bResult = true;
	bool bBringToFrontIfOpen = true;

#if WITH_EDITOR
	//UE_LOG(LogTemp, Display, TEXT("%s"), localInt);
	UE_LOG(LogTemp, Display, TEXT("Running CloneAnimSequenceInstance %"), NewAssets.Num());

	
	//UAnimationAsset* SourceAnimAsset = AnimTrackerPtr.Pin()->GetSourceAnim();
	TArray<UAnimationAsset*> AnimAssets = AnimTrackerPtr.Pin()->GetSourceAnimAssets();

	IAssetEditorInstance* EditorInstance = AnimTrackerPtr.Pin()->GetEditor();
	
	if (EditorInstance != nullptr)
	{
		//Focuses to window
		UE_LOG(LogTemp, Display, TEXT("Found service editor window!"));

		if (EditorInstance->GetEditorName() == FName("AnimationEditor"))
		{
			IAnimationEditor* AnimServiceEditor = static_cast<IAnimationEditor*>(EditorInstance);

			for (int index = 0; index < AnimAssets.Num(); index++) {

				//Change the current anim to this one
				UAnimationAsset* TargetAnimAsset = Cast<UAnimationAsset>(NewAssets[index]);
				UAnimationAsset* SourceAnimAsset = AnimAssets[index];


				AnimServiceEditor->SetAnimationAsset(SourceAnimAsset);
				TSharedRef<IPersonaToolkit> PersonaToolkit = AnimServiceEditor->GetPersonaToolkit();
				USkeletalMeshComponent* SourceMeshComponent = PersonaToolkit->GetPreviewMeshComponent();
				UAnimSequence* TargetSequence = Cast<UAnimSequence>(TargetAnimAsset);
				UE_LOG(LogTemp, Display, TEXT("Ready to process asset with component %s"), *SourceMeshComponent->GetName());
				//Sequence is new Fbx passed in via object
				//MeshComponent must come from old Ui

				ISequenceRecorder& RecorderModule = FModuleManager::Get().LoadModuleChecked<ISequenceRecorder>("SequenceRecorder");
				bResult &= RecorderModule.RecordSingleNodeInstanceToAnimation(SourceMeshComponent, TargetSequence, true);
			
				FAssetRegistryModule::AssetCreated(NewAssets[index]);
			}
		}


	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Animation Window Found Anim Asset!"));
	}
	/*
	if (bResult) {

		FAssetRegistryModule::AssetCreated(TargetAnimAsset);
	}*/

#endif

	return bResult;
}






//Opens Editor Windows for all selected assets
bool UAnimSequenceTools_BPLibrary::OpenEditorWindowsForAssets(const TArray<UObject*> NewAssets)
{
	if (NewAssets.Num() > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Created new asset!"));
		// forward to asset manager to open the asset for us
		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
		TWeakPtr<IAssetTypeActions> AssetTypeActions = AssetToolsModule.Get().GetAssetTypeActionsForClass(NewAssets[0]->GetClass());
		if (AssetTypeActions.IsValid())
		{

			AssetTypeActions.Pin()->OpenAssetEditor(NewAssets);
		}
	}
	return true;
}


//Will register and open the editor window for a single asset
bool UAnimSequenceTools_BPLibrary::HandleAssetCreated(const TArray<UObject*> NewAssets)
{
	if (NewAssets.Num() > 0)
	{
		FAssetRegistryModule::AssetCreated(NewAssets[0]);

		// forward to asset manager to open the asset for us
		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
		TWeakPtr<IAssetTypeActions> AssetTypeActions = AssetToolsModule.Get().GetAssetTypeActionsForClass(NewAssets[0]->GetClass());
		if (AssetTypeActions.IsValid())
		{
			AssetTypeActions.Pin()->OpenAssetEditor(NewAssets);
		}
	}
	return true;
}

//Will register and open the editor window for all assets passed in the array
bool UAnimSequenceTools_BPLibrary::HandleAssetsArrayCreated(const TArray<UObject*> NewAssets)
{
	for (UObject* Asset : NewAssets)
	{
		if (Asset == nullptr) {

			continue;
		}

		FAssetRegistryModule::AssetCreated(Asset);
		// forward to asset manager to open the asset for us
		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
		TWeakPtr<IAssetTypeActions> AssetTypeActions = AssetToolsModule.Get().GetAssetTypeActionsForClass(Asset->GetClass());
		if (AssetTypeActions.IsValid())
		{
			AssetTypeActions.Pin()->OpenAssetEditor(NewAssets);
		}
	}
	return true;
}

