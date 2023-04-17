// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Helpers/HelperParameters.h"
#include "StorageHelper.generated.h"

/**
 * 
 */


UENUM()
enum class EMeshFileType : uint8
{   
    FBX     UMETA(DisplayName = "fbx"),
    STL     UMETA(DisplayName = "stl"),
    PLY     UMETA(DisplayName = "ply"),
    GLTF    UMETA(DisplayName = "gltf"),
    GLTF2   UMETA(DisplayName = "gltf2"),
    OBJ     UMETA(DisplayName = "obj"),
};
UENUM()
enum class EMeshFile : uint8
{
	ZIP                           UMETA(DisplayName = "ZIP"),
	FBX                           UMETA(DisplayName = "FBX"),
    OBJ                           UMETA(DisplayName = "OBJ"),
    GLTF                          UMETA(DisplayName = "GLTF"),
    GLB                           UMETA(DisplayName = "GLB"),
    STL                           UMETA(DisplayName = "STL"),
    PLY                           UMETA(DisplayName = "PLY"),
};

DECLARE_DELEGATE_OneParam(FAsyncSearchDelegate, const TArray<FString>&);

UCLASS()
class RUNTIMEMESHIMPORTEXPORT_API UStorageHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|PathHelper")
	static FString GetRootFolder();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|PathHelper")
	static FString GetFullPathFromRelative(FString & InPath);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utils")
    static FString GetMeshZipFilename(const FString& root,const FString& fileName);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utils")
    static FString GetMeshFilename(const FString& root,const FString& fileName,const FString& extensionName);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utils")
    static FString GetTextureFilename(const FString& root);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utils")
    static FString GetZipFilename(const FString& root, EMeshLod lod);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utils")
    static FString GetModelInfoFilename(const FString& root);

    UFUNCTION(BlueprintCallable, Category = "RuntimeImport|Utils")
    static void Unzip(const FString& archive, const FString& destination, bool createSubFolder, bool removeArchive);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utils")
    static FString GetMeshsFolder(const FString& root);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utils")
    static FString GetUserInfoFolder();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utils")
    static FString GetUserPicturesFolder();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utils")
    static FString GetAndroidPath(const FString& folderName);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utils")
    static FString GetLodDirectory(const FString& root, EMeshLod lod);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utils")
    static FString GetMeshDestination(const FString& AssetID, const FString& AssetURL);

    UFUNCTION(BlueprintCallable, Category = "RuntimeImport|Utils")
    static void DeleteFile(const FString& file);

    UFUNCTION(BlueprintCallable, Category = "RuntimeImport|Utils")
    static bool FileExists(const FString& file);

    static void FindAllFilesInDirectory(const FString Directory,const bool fullPath, const TArray<FString> onlyFilesStartingWith,const TArray<FString> onlyFilesWithExtension, FAsyncSearchDelegate onComplete);
    
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utlis")
    static FString GetTextureFolder(FString& rootFolder);

    UFUNCTION(BlueprintCallable, Category = "RuntimeImport}Utils")
    static void DeleteDirectory(const FString &Path);

    /*UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utils")
    static TArray<FString> SearchMeshes(FString& Path, EMeshFileType fileType)

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utils")
    static TArray<FString> SearchTextures(FString& Path)*/
};
