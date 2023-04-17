/* Copyright (C) Itseez3D, Inc. - All Rights Reserved
 * You may not use this file except in compliance with an authorized license
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * UNLESS REQUIRED BY APPLICABLE LAW OR AGREED BY ITSEEZ3D, INC. IN WRITING, SOFTWARE DISTRIBUTED UNDER THE LICENSE IS DISTRIBUTED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED
 * See the License for the specific language governing permissions and limitations under the License.
 * Written by Itseez3D, Inc. <support@itseez3D.com>, May 2020
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/StreamableManager.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Templates/SubclassOf.h"
#include "Helpers/HTTPHelper.h"
#include "Helpers/HelperParameters.h"
#include "Helpers/StorageHelper.h"
#include "Containers/Array.h"
#include "Containers/Queue.h"
#include "Containers/Map.h"
#include "Dom/JsonObject.h"
#include <JsonObjectConverter.h>
#include "Engine/EngineTypes.h"
#include "AsyncRequestsNodes.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncRequestCompletedDelegate, const FString&, Content, int32, Code);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAsyncRequestFailedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncConnectionMulticastDelegate, const FConnection&, Connection, int32, Code);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAsyncMeshDataMulticastDelegate, const FMeshData&, MeshData, const FConnection&, Connection, int32, Code);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDownloadFileMulticastDelegate,const FString&, DownloadedFilePath, int32, Code);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAsyncDownloadArrayOfFilesMulticastDelegate, const FMeshData&, MeshData, const FConnection&, Connection, const TArray<FString>&, DownloadedFilePath, int32, Code);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAsyncGetParametersMulticastDelegate, const FConnection&, Connection, const FAvatarGenerationParams&, Parameters, int32, Code);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAsyncModelInfoMulticastDelegate, const FConnection&, Connection, const FModelInfo&, ModelInfo, int32, Code);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncFoundFilesMulticastDelegate,const TArray<FString>&,foundFiles);
UCLASS()
class RUNTIMEMESHIMPORTEXPORT_API UAsyncInitRuntimeImportConnection : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "RuntimeImport|Http|Requests")
    static UAsyncInitRuntimeImportConnection* AsyncInitRuntimeImportConnection(UObject* WorldContextObject, FConnection connection);

    virtual void Activate() override;

    UPROPERTY(BlueprintAssignable)
    FAsyncConnectionMulticastDelegate Completed;

protected:
    UPROPERTY()
    FConnection Connection;

    UPROPERTY()
    bool Success;

    void OnConnectionInitialized(FConnection Content, int32 Code);
};

UCLASS()
class RUNTIMEMESHIMPORTEXPORT_API UAsyncSaveModelInfo : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "RuntimeImport|Http|Requests")
    static UAsyncSaveModelInfo* AsyncSaveModelInfo(UObject* WorldContextObject, FConnection connection, FMeshData MeshData, const FString& avatarPath);

    UPROPERTY(BlueprintAssignable)
    FAsyncDownloadFileMulticastDelegate AvatarFileDownloaded;
    virtual void Activate() override;
protected:
    UPROPERTY()
    FMeshData MeshData;
    UPROPERTY()
    FConnection Connection;
    UPROPERTY()
    FString AvatarPath;
    UPROPERTY()
    FString DownloadResult;

    void OnFileDownloaded(FString File, int32 Code);
};

UCLASS()
class RUNTIMEMESHIMPORTEXPORT_API UAsyncGetMeshFile : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "RuntimeImport|Http|Requests")
    static UAsyncGetMeshFile* AsyncGetMeshFile(UObject* WorldContextObject,const FString& AssetID,const FString& AssetName, const FString& AssetURL, const FString &avatarPath,EMeshFile fileType);

    UPROPERTY(BlueprintAssignable)
    FAsyncDownloadFileMulticastDelegate AvatarFileDownloaded;
    virtual void Activate() override;
protected:
    UPROPERTY()
    EMeshFile FileType;
    UPROPERTY()
    FString AssetID;
    UPROPERTY()
    FString AssetName;
    UPROPERTY()
    FString AssetURL;
    UPROPERTY()
    FString AvatarPath;

    UPROPERTY()
    FString DownloadResult;

    void OnAvatarDownloaded(FString File, int32 Code);
};

UCLASS()
class RUNTIMEMESHIMPORTEXPORT_API UAwaitMeshCalculation : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "RuntimeImport|Http|Requests")
    static UAwaitMeshCalculation* AwaitAvatarCalculation(UObject* WorldContextObject, FConnection connection, FMeshData MeshData);

    UPROPERTY(BlueprintAssignable)
    FAsyncMeshDataMulticastDelegate AvatarCalculated;

    UPROPERTY(BlueprintAssignable)
    FAsyncMeshDataMulticastDelegate ProgressChanged;

    UFUNCTION()
    void OnTimer();

    virtual void Activate() override;
protected:
    /* Handle to manage the timer */
    FTimerHandle RequestTimer;

    UObject* World;

    UPROPERTY()
    FMeshData MeshData;
    UPROPERTY()
    FConnection Connection;

    void OnStatusUpdated(FMeshData Data, int32 Code);
};

UCLASS()
class RUNTIMEMESHIMPORTEXPORT_API UAsyncCreateMesh : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "RuntimeImport|Http|Requests")
    static UAsyncCreateMesh* AsyncCreateMesh(UObject* WorldContextObject, FConnection connection, FMeshParams params);

    UPROPERTY(BlueprintAssignable)
    FAsyncMeshDataMulticastDelegate AvatarCreated;

    virtual void Activate() override;

protected:
    UPROPERTY()
    FConnection Connection;

    UPROPERTY()
    FMeshParams Params;

    void OnContentReceived(FMeshData Data, int32 Code);
};

UCLASS()
class RUNTIMEMESHIMPORTEXPORT_API UAsyncGetModelInfo : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "RuntimeImport|Http|Requests")
    static UAsyncGetModelInfo* AsyncGetModelInfo(UObject* WorldContextObject, FConnection connection, FMeshData MeshData);

    UPROPERTY(BlueprintAssignable)
    FAsyncModelInfoMulticastDelegate ReceivedModelInfo;

    virtual void Activate() override;

protected:
    UPROPERTY()
    FConnection Connection;

    UPROPERTY()
    FMeshData MeshData;

    void OnModelInfoObtained(FModelInfo ModelInfo, int32 Code);
};

struct DownloadTarget
{
    FString Url;
    FString Destination;
    FString ArchiveContent = "";
};

UCLASS()
class UAsyncGetAvatarAdditionalTextures : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
        UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "RuntimeImport|Http|Requests")
        static UAsyncGetAvatarAdditionalTextures* AsyncGetAvatarAdditionalTextures(UObject* WorldContextObject, FConnection connection, FMeshData MeshData);

    UPROPERTY(BlueprintAssignable)
        FAsyncMeshDataMulticastDelegate AdditionalTexturesRequestComplete;
    virtual void Activate() override;

protected:
    UPROPERTY()
        FMeshData MeshData;
    UPROPERTY()
        FConnection Connection;
    void OnRequestSuccess(FMeshData MeshData, int32 Code);
};

UCLASS()
class RUNTIMEMESHIMPORTEXPORT_API UAsyncGetMeshData : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "RuntimeImport|Http|Requests")
    static UAsyncGetMeshData* AsyncGetMeshData(UObject* WorldContextObject, FConnection connection, FMeshData MeshData);

    UPROPERTY(BlueprintAssignable)
    FAsyncMeshDataMulticastDelegate MeshDataRequestComplete;
    virtual void Activate() override;

protected:
    UPROPERTY()
    FMeshData MeshData;
    UPROPERTY()
    FConnection Connection;
    void OnRequestSuccess(FMeshData MeshData, int32 Code);
};

UCLASS()
class RUNTIMEMESHIMPORTEXPORT_API UAsyncGetAvailableParameters : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "RuntimeImport|Http|Requests")
    static UAsyncGetAvailableParameters* AsyncGetAvailableParameters(UObject* WorldContextObject, FConnection connection, EParametersType parametersType);

    UPROPERTY(BlueprintAssignable)
    FAsyncGetParametersMulticastDelegate ParamsObtained;
    virtual void Activate() override;
protected:
    UPROPERTY()
    FConnection Connection;
    UPROPERTY()
    EParametersType ParametersType; 

    void OnParamsObtained(FAvatarGenerationParams File, int32 Code);
};

UCLASS()
class RUNTIMEMESHIMPORTEXPORT_API UAsyncCreatePlayerID : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "RuntimeImport|Http|Requests")
    static UAsyncCreatePlayerID* AsyncCreatePlayerID(UObject* WorldContextObject, FConnection connection, FString comment);

    virtual void Activate() override;

    UPROPERTY(BlueprintAssignable)
    FAsyncConnectionMulticastDelegate Completed;

protected:
    UPROPERTY()
    FConnection Connection;

    UPROPERTY()
    FString Comment;

    UPROPERTY()
    bool Success;

    void OnRequestComplete(FConnection Content, int32 Code);
};

UCLASS()
class RUNTIMEMESHIMPORTEXPORT_API UASyncSearchFileInDirectory : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "RuntimeImport|Http|Requests")
    static UASyncSearchFileInDirectory* SearchFileInDirectory(UObject* WorldContextObject,FString InPath,bool fullPath, TArray<FString> startingNames, TArray<FString> extensions);

    virtual void Activate() override;

    UPROPERTY(BlueprintAssignable)
    FAsyncFoundFilesMulticastDelegate Completed;

protected:
    UPROPERTY()
    bool fullPath;
    
    UPROPERTY()
    FString InPath;

    UPROPERTY()
    TArray<FString> onlyFilesWithStartingNames;

    UPROPERTY()
    TArray<FString> onlyFilesWithExtensions;

    void OnRequestComplete(const TArray<FString>& filesFound);
};