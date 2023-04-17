// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers/StorageHelper.h"
#include "Helpers/AndroidUtils.h"
#include "Helpers/HelperParameters.h"
#include "CoreMinimal.h"
#include "Helpers/ZipHelper.h"
#include "Misc/Paths.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "HAL/PlatformFilemanager.h"
#include "Engine/Engine.h"
#include "GenericPlatform/GenericWindow.h"
#include "Engine/GameViewportClient.h"
#include <algorithm>
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"

FString UStorageHelper::GetRootFolder()
{   
#if PLATFORM_ANDROID
    UAndroidUtils* androidUtils = NewObject<UAndroidUtils>();
    return androidUtils->GetPersistentDataPath();
#elif PLATFORM_IOS
    return[PhotoViewController getPersistentDataPath];
#else
    return FPaths::Combine(FPlatformProcess::UserSettingsDir(), FString("UE5"));
#endif
    
}

FString UStorageHelper::GetFullPathFromRelative(FString& InPath)
{
    return FString();
}

FString UStorageHelper::GetMeshZipFilename(const FString& root, const FString& fileName)
{
    if(fileName.IsEmpty())
    { 
        return FPaths::Combine(root, FString("mesh.zip")); 
    }
    else 
    {
        if(fileName.Contains(FString(".zip")))
        {
            return FPaths::Combine(root,fileName);
        }
        else 
        {   
            return FPaths::Combine(root,(fileName + FString(".zip")));
        }
    }
}

FString UStorageHelper::GetMeshFilename(const FString& root, const FString& fileName, const FString& extensionName)
{
    if (fileName.IsEmpty())
    {
        
        return FPaths::Combine(root,(FString("model")+FString(".")+extensionName));
    }
    else
    {
        if (fileName.Contains(extensionName))
        {
            return FPaths::Combine(root,fileName);
        }
        else
        {
            return FPaths::Combine(root, (fileName+FString(".")+extensionName));
        }
    }
}

FString UStorageHelper::GetTextureFilename(const FString& root)
{
    return FPaths::Combine(root, FString("model.jpg"));
}

FString UStorageHelper::GetZipFilename(const FString& root, EMeshLod lod)
{
    if (lod == EMeshLod::avatar_lod_0)
    {
        return FPaths::Combine(root, FString("blendshapes.zip"));
    }
    else
    {
        return FPaths::Combine(root, UHelperParameters::LodToString(lod), FString("blendshapes.zip"));
    }
}

FString UStorageHelper::GetModelInfoFilename(const FString& root)
{
    return FString();
}


void UStorageHelper::Unzip(const FString& archive, const FString& destination, bool createSubFolder, bool removeArchive)
{
    ZipHelper z;
    z.Unzip(archive, destination, createSubFolder, removeArchive);
}

FString UStorageHelper::GetMeshsFolder(const FString& root)
{
    return FPaths::Combine(root, FString("Meshes"));
}

FString UStorageHelper::GetUserInfoFolder()
{
    return FPaths::Combine(GetRootFolder(), FString("UserInfo"));
}

FString UStorageHelper::GetUserPicturesFolder()
{
    return FPaths::Combine(GetRootFolder(), FString("Pictures"));
}

FString UStorageHelper::GetAndroidPath(const FString& folderName)
{
    return FPaths::Combine(GetRootFolder(), folderName);
}

FString UStorageHelper::GetLodDirectory(const FString& root, EMeshLod lod)
{
    if (lod == EMeshLod::default_lod)
    {
        return root;
    }
    else
    {
        return FPaths::Combine(root, UHelperParameters::LodToString(lod));
    }
}

FString UStorageHelper::GetMeshDestination(const FString& AssetID, const FString& AssetURL)
{
    return FPaths::Combine(GetMeshsFolder(GetRootFolder()), AssetID);
}

void UStorageHelper::DeleteFile(const FString& file)
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    PlatformFile.DeleteFile(*file);
}

bool UStorageHelper::FileExists(const FString& file)
{
    if (file.IsEmpty())
    {
        return false;
    }
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    return PlatformFile.FileExists(*file);
}


void UStorageHelper::FindAllFilesInDirectory(const FString Directory, const bool fullPath, const TArray<FString> onlyFilesStartingWith, const TArray<FString> onlyFilesWithExtension, FAsyncSearchDelegate onComplete)
{
    // Get all files in directory
    TArray<FString> directoriesToSkip;
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    FLocalTimestampDirectoryVisitor Visitor(PlatformFile, directoriesToSkip, directoriesToSkip, false);
    PlatformFile.IterateDirectory(*Directory, Visitor);
    TArray<FString> files;


    int32 NamesLength = (int32)onlyFilesStartingWith.Num();
    int32 ExtensionLength = (int32)onlyFilesWithExtension.Num();
    
   /* if () 
    {
       LoopIndex = onlyFilesWithExtension.Num();
    }
    else
    { 
        LoopIndex = onlyFilesStartingWith.Num();
    }*/
    NamesLength = ((NamesLength<ExtensionLength)?ExtensionLength:NamesLength);

    for (TMap<FString, FDateTime>::TIterator TimestampIt(Visitor.FileTimes); TimestampIt; ++TimestampIt)
    {
        const FString filePath = TimestampIt.Key();
        const FString fileName = FPaths::GetCleanFilename(filePath);
        bool shouldAddFile = 0<NamesLength?false:true;
   
        // Check if file extension is required characters
        for(int32 index=0 ;index < NamesLength;index++){
                 if(onlyFilesWithExtension.IsValidIndex(index) )
                {   
                     FString currentSearchExtension = onlyFilesWithExtension[index];
                     if (!currentSearchExtension.IsEmpty())
                     {
                         if ((FPaths::GetExtension(fileName, false).Equals(currentSearchExtension, ESearchCase::IgnoreCase))){
                             shouldAddFile = true;
                             break;
                         }   
                     }
                 }

        
                 if (onlyFilesStartingWith.IsValidIndex(index))
                 {
                     FString currentSearchString = onlyFilesStartingWith[index];
                     if (!currentSearchString.IsEmpty())
                     {
                         if (!(fileName.Left(currentSearchString.Len()).Equals(currentSearchString))){
                             shouldAddFile = true;
                             break;
                         }                        
                     }
                 }
                
        }

        if (shouldAddFile)
            files.Add(fullPath ? filePath : fileName);
    }
    
    onComplete.ExecuteIfBound(files);
}

FString UStorageHelper::GetTextureFolder(FString& rootFolder)
{
    return FPaths::Combine(rootFolder,FString("textures"));
}

void UStorageHelper::DeleteDirectory(const FString& Path)
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    PlatformFile.DeleteDirectoryRecursively(*Path);
}
