// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimSequenceTools_BPLibrary.h"

//Entry point blueprints functions
void UAnimSequenceTools_BPLibrary::CloneAnimSequences(const TArray<UObject*> SelectedAssets) {

	StartBatchAnimFactory(SelectedAssets);
}

void UAnimSequenceTools_BPLibrary::CloneAnimSequencesTargeted(const TArray<UObject*> SelectedAssets) {

	StartTargetedAnimFactory(SelectedAssets);
}

void UAnimSequenceTools_BPLibrary::CloneAnimSequencesToDir(const TArray<UObject*> SelectedAssets, const FString outputPath) {

	StartBatchAnimFactoryToDir(SelectedAssets, &outputPath);
}



//Handles opening UI editors to process
IAssetEditorInstance* UAnimSequenceTools_BPLibrary::OpenServiceEditor(const TArray<UObject*> SelectedAssets) {

	TArray<UObject*> FirstAssetArray;

	FirstAssetArray.Add(SelectedAssets[0]);
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
	TWeakPtr<IAssetTypeActions> AssetTypeActions = AssetToolsModule.Get().GetAssetTypeActionsForClass(FirstAssetArray[0]->GetClass());

	if (AssetTypeActions.IsValid())
	{
		AssetTypeActions.Pin()->OpenAssetEditor(FirstAssetArray);
		return GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->FindEditorForAsset(SelectedAssets[0], true);
	}

	return nullptr;
}


//Opens Editor Windows for all selected assets
bool UAnimSequenceTools_BPLibrary::OpenEditorWindowsForAssets(const TArray<UObject*> SelectedAssets)
{
	if (SelectedAssets.Num() > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Created new asset!"));
		// forward to asset manager to open the asset for us
		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
		TWeakPtr<IAssetTypeActions> AssetTypeActions = AssetToolsModule.Get().GetAssetTypeActionsForClass(SelectedAssets[0]->GetClass());
		if (AssetTypeActions.IsValid())
		{

			AssetTypeActions.Pin()->OpenAssetEditor(SelectedAssets);
		}
	}
	return true;
}





//Activates the BatchAnimSeqCloner and creates the new UObject assets
void UAnimSequenceTools_BPLibrary::StartBatchAnimFactoryToDir(const TArray<UObject*> SelectedAssets, const FString* outputPath) {

	//Grab the editor window insntace
	const bool bDoNotShowNameDialog = false;
	const bool bAllowReplaceExisting = true;
	bool bBringToFrontIfOpen = true;

	UE_LOG(LogTemp, Display, TEXT("Batch Cloning Animation Sequences : %"), SelectedAssets.Num());
	TArray<UAnimationAsset*> AnimAssets;
	TArray<UObject*> DuplicatedAssets;

	TSharedPtr<AnimSequenceTools_AnimTracker> AnimTrackerPtr = MakeShareable(new AnimSequenceTools_AnimTracker());
	TWeakPtr<AnimSequenceTools_AnimTracker> WeakPtr = AnimTrackerPtr.ToWeakPtr();


	for (auto ObjIt = SelectedAssets.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		//If the asset is a valid UAnimationAsset
		if (UAnimationAsset* AnimAsset = Cast<UAnimationAsset>(*ObjIt))
		{

			AnimAssets.Add(AnimAsset);
			AnimTrackerPtr->AddSourceAnimAsset(AnimAsset);

			FString fileName = AnimAsset->GetName();
			FString Name, PackageName;

			// Find the position of the last occurrence of "Content" in the path
			int32 Index = outputPath->Find(TEXT("Content"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);



			// If "Content" is found, trim the path
			if (Index != INDEX_NONE)
			{
				PackageName = outputPath->RightChop(Index + 7); // 7 is the length of "Content"
				PackageName = "/Game" + PackageName + "/AssetName/";
			}
			UE_LOG(LogTemp, Display, TEXT("Trimmed packadge directory %s"), *PackageName);
			
			UE_LOG(LogTemp, Display, TEXT("Asset Name %s"), *fileName);
			const FString PackagePath = FPackageName::GetLongPackagePath(PackageName);


			UPackage* Package = CreatePackage(*PackagePath);
			UAnimSequenceFactory* MyFactory = NewObject<UAnimSequenceFactory>(); // Can omit, and a default factory will be used
			MyFactory->TargetSkeleton = AnimAsset->GetSkeleton();


			//Ensure the packadge and file name are NOT identical to the source file we are copying from
			UObject* NewObject = MyFactory->FactoryCreateNew(UAnimSequence::StaticClass(), Package, *fileName, RF_Standalone | RF_Public, NULL, GWarn);
			DuplicatedAssets.Add(NewObject);
			FAssetRegistryModule::AssetCreated(NewObject);


		}

	}

	IAssetEditorInstance* AnimationEditorInstance = OpenServiceEditor(SelectedAssets);
	AnimTrackerPtr->SetEditor(AnimationEditorInstance);

	//Assign Source Anim Asset		
	if (AnimationEditorInstance != nullptr) {

		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

		BatchAnimSeqCloner(DuplicatedAssets, WeakPtr);
		AnimationEditorInstance->CloseWindow();
	}

	else {
		UE_LOG(LogTemp, Error, TEXT("Pointer to original editor invalid!"));
	}



}


//Activates the BatchAnimSeqCloner and creates the new UObject assets
void UAnimSequenceTools_BPLibrary::StartBatchAnimFactory(const TArray<UObject*> SelectedAssets) {

	//Grab the editor window insntace
	const bool bDoNotShowNameDialog = false;
	const bool bAllowReplaceExisting = true;
	bool bBringToFrontIfOpen = true;

	UE_LOG(LogTemp, Display, TEXT("Batch Cloning Animation Sequences : %"),  SelectedAssets.Num());
	TArray<UAnimationAsset*> AnimAssets;
	TArray<UObject*> DuplicatedAssets;

	TSharedPtr<AnimSequenceTools_AnimTracker> AnimTrackerPtr = MakeShareable(new AnimSequenceTools_AnimTracker());
	TWeakPtr<AnimSequenceTools_AnimTracker> WeakPtr = AnimTrackerPtr.ToWeakPtr();
	




	FString Name, PackageName;
	//Open Dialoge modal
	/*
	

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
	FString AssetPath = SelectedAssets[0]->GetOutermost()->GetName();

	// Determine an appropriate name
	AssetToolsModule.Get().CreateUniqueAssetName(AssetPath, TEXT(""), PackageName, Name);

	// set the unique asset as a default name
	TSharedRef<SCreateAnimationAssetDlg> NewAnimDlg =
		SNew(SCreateAnimationAssetDlg)
		.DefaultAssetPath(FText::FromString(PackageName));

	// show a dialog to determine a new asset name
	if (NewAnimDlg->ShowModal() == EAppReturnType::Cancel)
	{
		PackageName = TEXT("/Game/DuplicatedAnimations/AssetName/");
	}
	else {

		PackageName = NewAnimDlg->GetFullAssetPath();
	}
	
	UPackage* Package = CreatePackage(*PackageName);
	*/
	//End Dialogue Modal Open






	

	for (auto ObjIt = SelectedAssets.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		//If the asset is a valid UAnimationAsset
		if (UAnimationAsset* AnimAsset = Cast<UAnimationAsset>(*ObjIt))
		{

			AnimAssets.Add(AnimAsset);
			AnimTrackerPtr->AddSourceAnimAsset(AnimAsset);
			Name = AnimAsset->GetName();	
			//Hardcoded Packadge setting
			PackageName = TEXT("/Game/DuplicatedAnimations/");
			UPackage* Package = CreatePackage(*PackageName);

			UE_LOG(LogTemp, Display, TEXT("Change Package name to %s"), *PackageName);
			UE_LOG(LogTemp, Display, TEXT("Asset Name %s"), *Name);

			

			
			UAnimSequenceFactory* MyFactory = NewObject<UAnimSequenceFactory>(); // Can omit, and a default factory will be used
			MyFactory->TargetSkeleton = AnimAsset->GetSkeleton();

			
			//Ensure the packadge and file name are NOT identical to the source file we are copying from
			UObject* NewObject = MyFactory->FactoryCreateNew(UAnimSequence::StaticClass(), Package, *Name, RF_Standalone | RF_Public, NULL, GWarn);
			DuplicatedAssets.Add(NewObject);
			FAssetRegistryModule::AssetCreated(NewObject);


		}

	}

	IAssetEditorInstance* AnimationEditorInstance = OpenServiceEditor(SelectedAssets);
	AnimTrackerPtr->SetEditor(AnimationEditorInstance);
	
	//Assign Source Anim Asset		
	if (AnimationEditorInstance != nullptr) {
		
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

		BatchAnimSeqCloner(DuplicatedAssets, WeakPtr);
		AnimationEditorInstance->CloseWindow();
	}

	else {
		UE_LOG(LogTemp, Error, TEXT("Pointer to original editor invalid!"));
	}


	
}


bool UAnimSequenceTools_BPLibrary::BatchAnimSeqCloner(const TArray<UObject*> NewAssets, TWeakPtr<AnimSequenceTools_AnimTracker> AnimTrackerPtr) {

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

	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Animation Window Found Anim Asset!"));
	}

	return bResult;
#endif

}





//Activates the TargetedAnimSeqCloner by using the AnimationEditorUtils::ExecuteNewAnimAsset UI based workflow
void UAnimSequenceTools_BPLibrary::StartTargetedAnimFactory(const TArray<UObject*> SelectedAssets) {

	//Grab the editor window insntace
	const bool bDoNotShowNameDialog = false;
	const bool bAllowReplaceExisting = true;
	bool bBringToFrontIfOpen = true;

	UE_LOG(LogTemp, Display, TEXT("Duplicating Anim Sequences through targeting"));
	TArray<UAnimationAsset*> AnimAssets;

	TArray<TSoftObjectPtr<UObject>> Objects;	

	TSharedPtr<AnimSequenceTools_AnimTracker> AnimTrackerPtr = MakeShareable(new AnimSequenceTools_AnimTracker());
	TWeakPtr<AnimSequenceTools_AnimTracker> WeakPtr = AnimTrackerPtr.ToWeakPtr();


	for (auto ObjIt = SelectedAssets.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		//If the asset is a valid UAnimationAsset
		if (UAnimationAsset* AnimAsset = Cast<UAnimationAsset>(*ObjIt))
		{

			Objects.Add(AnimAsset->GetSkeleton());	

			AnimAssets.Add(AnimAsset);
			AnimTrackerPtr->AddSourceAnimAsset(AnimAsset);

		}

	}


	IAssetEditorInstance* AnimationEditorInstance = OpenServiceEditor(SelectedAssets);
	AnimTrackerPtr->SetEditor(AnimationEditorInstance);

	//Assign Source Anim Asset		
	if (AnimationEditorInstance != nullptr) {

		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

		
		AnimationEditorInstance->FocusWindow();
		AnimationEditorUtils::ExecuteNewAnimAsset<UAnimSequenceFactory, UAnimSequence>(Objects, FString("_Sequence"),
			FAnimAssetCreated::CreateStatic(&UAnimSequenceTools_BPLibrary::TargetedAnimSeqCloner, WeakPtr),
			bDoNotShowNameDialog, bAllowReplaceExisting);

		AnimTrackerPtr->GetEditor()->CloseWindow();
		
	}

	else {
		UE_LOG(LogTemp, Error, TEXT("Pointer to original editor invalid!"));
	}



}


bool UAnimSequenceTools_BPLibrary::TargetedAnimSeqCloner(const TArray<UObject*> NewAssets, TWeakPtr<AnimSequenceTools_AnimTracker> AnimTrackerPtr) {

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


			ISequenceRecorder& RecorderModule = FModuleManager::Get().LoadModuleChecked<ISequenceRecorder>("SequenceRecorder");
			bResult &= RecorderModule.RecordSingleNodeInstanceToAnimation(SourceMeshComponent, TargetSequence, true);

			FAssetRegistryModule::AssetCreated(NewAssets[index]);
		}



	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Animation Window Found Anim Asset!"));
	}
	
	return bResult;

#endif


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

