/* Copyright (C) Itseez3D, Inc. - All Rights Reserved
 * You may not use this file except in compliance with an authorized license
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * UNLESS REQUIRED BY APPLICABLE LAW OR AGREED BY ITSEEZ3D, INC. IN WRITING, SOFTWARE DISTRIBUTED UNDER THE LICENSE IS DISTRIBUTED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED
 * See the License for the specific language governing permissions and limitations under the License.
 * Written by Itseez3D, Inc. <support@itseez3D.com>, May 2020
 */

#include "Helpers/AsyncRequestsNodes.h"
#include "Helpers/HTTPHelper.h"
#include "TimerManager.h"
#include "Engine/World.h"



//================================================================
void UAsyncInitRuntimeImportConnection::OnConnectionInitialized(FConnection connection, int32 code)
{
    Completed.Broadcast(connection, code);
    SetReadyToDestroy();
}

void UAsyncInitRuntimeImportConnection::Activate()
{
//    UHTTPHelper::InitConnection(Connection, FAsyncConnectionDelegate::CreateUObject(this, &UAsyncInitRuntimeImportConnection::OnConnectionInitialized));
}

UAsyncInitRuntimeImportConnection* UAsyncInitRuntimeImportConnection::AsyncInitRuntimeImportConnection(UObject* WorldContextObject, FConnection connection)
{
    UAsyncInitRuntimeImportConnection* Action = NewObject<UAsyncInitRuntimeImportConnection>();
    Action->Connection = connection;
    return Action;
}

void UAsyncCreateMesh::Activate()
{
//    UHTTPHelper::CreateAvatar(Connection, Params, FAsyncMeshDataDelegate::CreateUObject(this, &UAsyncCreateAvatar::OnContentReceived));
}

UAsyncCreateMesh* UAsyncCreateMesh::AsyncCreateMesh(UObject* WorldContextObject, FConnection connection, FMeshParams params)
{
    UAsyncCreateMesh* Action = NewObject<UAsyncCreateMesh>();
    Action->Connection = connection;
    Action->Params = params;
    return Action;
}

void UAsyncCreateMesh::OnContentReceived(FMeshData Data, int32 Code)
{
    AvatarCreated.Broadcast(Data, this->Connection, Code);
    SetReadyToDestroy();
}

void UAwaitMeshCalculation::Activate()
{
    //UHTTPHelper::GetMeshData(Connection, MeshData, FAsyncMeshDataDelegate::CreateUObject(this, &UAwaitMeshCalculation::OnStatusUpdated));
}

void UAwaitMeshCalculation::OnTimer()
{
    Activate();
}

void UAwaitMeshCalculation::OnStatusUpdated(FMeshData Data, int32 Code)
{
    if (Data.Status == "Failed" || Code)
    {
        AvatarCalculated.Broadcast(Data, Connection, Code);
        UWorld* world = World->GetWorld();
        world->GetTimerManager().ClearTimer(RequestTimer);
        SetReadyToDestroy();
    }
    else if (Data.Status != "Completed")
    {
        ProgressChanged.Broadcast(Data, Connection, Code);
        World->GetWorld()->GetTimerManager().SetTimer(RequestTimer, this, &UAwaitMeshCalculation::OnTimer, 1.0f, false);
    }
    else
    {
        AvatarCalculated.Broadcast(Data, Connection, Code);
        World->GetWorld()->GetTimerManager().ClearTimer(RequestTimer);
        SetReadyToDestroy();
    }

}

UAwaitMeshCalculation* UAwaitMeshCalculation::AwaitAvatarCalculation(UObject* WorldContextObject, FConnection connection, FMeshData MeshData)
{
    UAwaitMeshCalculation* Awaiter = NewObject<UAwaitMeshCalculation>();
    Awaiter->RegisterWithGameInstance(WorldContextObject);
    Awaiter->World = WorldContextObject;
    Awaiter->Connection = connection;
    Awaiter->MeshData = MeshData;

    return Awaiter;
}

void UAsyncGetMeshFile::Activate()
{
    UHTTPHelper::GetMeshFile(AssetURL,AvatarPath,AssetName,FileType,FAsyncFileResultDelegate::CreateUObject(this, &UAsyncGetMeshFile::OnAvatarDownloaded));
}

void UAsyncGetMeshFile::OnAvatarDownloaded(FString File, int32 Code)
{
    AvatarFileDownloaded.Broadcast(File, Code);
    SetReadyToDestroy();
}

UAsyncGetMeshFile* UAsyncGetMeshFile::AsyncGetMeshFile(UObject* WorldContextObject, const FString& AssetID, const FString& AssetName, const FString& AssetURL, const FString& avatarPath,EMeshFile fileType)
{
    UAsyncGetMeshFile* MeshDownloader = NewObject<UAsyncGetMeshFile>();
    MeshDownloader->AssetID = AssetID;
    MeshDownloader->AssetName = AssetName;
    MeshDownloader->AssetURL = AssetURL;
    MeshDownloader->AvatarPath = avatarPath;
    MeshDownloader->FileType = fileType;
    return MeshDownloader;
}

void UAsyncGetAvailableParameters::Activate()
{
    //UHTTPHelper::GetAvailableParameters(Connection, PipelineType, ParametersType, FAvatarParametersDelegate::CreateUObject(this, &UAsyncGetAvailableParameters::OnParamsObtained));
}

UAsyncGetAvailableParameters* UAsyncGetAvailableParameters::AsyncGetAvailableParameters(UObject* WorldContextObject, FConnection connection, EParametersType parametersType)
{
    UAsyncGetAvailableParameters* object = NewObject<UAsyncGetAvailableParameters>();
    object->Connection = connection;
    object->ParametersType = parametersType;
    return object;
}

void UAsyncGetAvailableParameters::OnParamsObtained(FAvatarGenerationParams params, int32 Code)
{
    ParamsObtained.Broadcast(Connection, params, Code);
    SetReadyToDestroy();
}

void UAsyncGetModelInfo::Activate()
{
  //  UHTTPHelper::GetModelInfo(Connection, MeshData, FAsyncModelInfoDelegate::CreateUObject(this, &UAsyncGetModelInfo::OnModelInfoObtained));
}

void UAsyncGetModelInfo::OnModelInfoObtained(FModelInfo ModelInfo, int32 Code)
{
    ReceivedModelInfo.Broadcast(Connection, ModelInfo, Code);
    SetReadyToDestroy();
}

UAsyncGetModelInfo* UAsyncGetModelInfo::AsyncGetModelInfo(UObject* WorldContextObject, FConnection connection, FMeshData MeshData)
{
    UAsyncGetModelInfo* object = NewObject<UAsyncGetModelInfo>();
    object->Connection = connection;
    object->MeshData = MeshData;
    return object;
}

void UAsyncCreatePlayerID::Activate()
{
  //  UHTTPHelper::CreatePlayerID(Connection, Comment, FAsyncConnectionDelegate::CreateUObject(this, &UAsyncCreatePlayerID::OnRequestComplete));
}

UAsyncCreatePlayerID* UAsyncCreatePlayerID::AsyncCreatePlayerID(UObject* WorldContextObject, FConnection connection, FString comment)
{
    UAsyncCreatePlayerID* Action = NewObject<UAsyncCreatePlayerID>();
    Action->Connection = connection;
    return Action;
}

void UAsyncCreatePlayerID::OnRequestComplete(FConnection Content, int32 Code)
{
    Completed.Broadcast(Content, Code);
    SetReadyToDestroy();
}

UAsyncGetAvatarAdditionalTextures* UAsyncGetAvatarAdditionalTextures::AsyncGetAvatarAdditionalTextures(UObject* WorldContextObject, FConnection connection, FMeshData MeshData)
{
    UAsyncGetAvatarAdditionalTextures* object = NewObject<UAsyncGetAvatarAdditionalTextures>();
    object->Connection = connection;
    object->MeshData = MeshData;
    return object;
}

void UAsyncGetAvatarAdditionalTextures::Activate()
{
    UHTTPHelper::GetMeshTextures(Connection, MeshData, FAsyncDataDelegate::CreateUObject(this, &UAsyncGetAvatarAdditionalTextures::OnRequestSuccess));
}

void UAsyncGetAvatarAdditionalTextures::OnRequestSuccess(FMeshData L_MeshData, int32 Code)
{
    AdditionalTexturesRequestComplete.Broadcast(L_MeshData, Connection, Code);
    SetReadyToDestroy();
}

UAsyncSaveModelInfo* UAsyncSaveModelInfo::AsyncSaveModelInfo(UObject* WorldContextObject, FConnection connection, FMeshData MeshData, const FString& avatarPath)
{
    UAsyncSaveModelInfo* object = NewObject<UAsyncSaveModelInfo>();
    object->Connection = connection;
    object->MeshData = MeshData;
    object->AvatarPath = avatarPath;
    return object;
}

void UAsyncSaveModelInfo::Activate()
{
  //  UHTTPHelper::SaveModelInfo(Connection, MeshData, AvatarPath, FAsyncFileResultDelegate::CreateUObject(this, &UAsyncSaveModelInfo::OnFileDownloaded));
}

void UAsyncSaveModelInfo::OnFileDownloaded(FString File, int32 Code)
{
    AvatarFileDownloaded.Broadcast(File, Code);
    SetReadyToDestroy();
}


UAsyncGetMeshData* UAsyncGetMeshData::AsyncGetMeshData(UObject* WorldContextObject, FConnection connection, FMeshData MeshData)
{
    UAsyncGetMeshData* object = NewObject<UAsyncGetMeshData>();
    object->Connection = connection;
    object->MeshData = MeshData;
    return object;
}

void UAsyncGetMeshData::Activate()
{
 //   UHTTPHelper::GetMeshData(Connection, MeshData, FAsyncMeshDataDelegate::CreateUObject(this, &UAsyncGetMeshData::OnRequestSuccess));
}

void UAsyncGetMeshData::OnRequestSuccess(FMeshData L_MeshData, int32 Code)
{
    MeshDataRequestComplete.Broadcast(MeshData, Connection, Code);
    SetReadyToDestroy();
}



void UASyncSearchFileInDirectory::Activate()
{
    UStorageHelper::FindAllFilesInDirectory(InPath,fullPath,onlyFilesWithStartingNames,onlyFilesWithExtensions,FAsyncSearchDelegate::CreateUObject(this,&UASyncSearchFileInDirectory::OnRequestComplete));
}

void UASyncSearchFileInDirectory::OnRequestComplete(const TArray<FString>& filesFound)
{
    Completed.Broadcast(filesFound);
    SetReadyToDestroy();
}

UASyncSearchFileInDirectory* UASyncSearchFileInDirectory::SearchFileInDirectory(UObject* WorldContextObject, FString L_InPath, bool L_bfullPath, TArray<FString> L_onlyFilesWithStartingNames, TArray<FString> L_onlyFilesWithExtensions)
{
    UASyncSearchFileInDirectory* object = NewObject<UASyncSearchFileInDirectory>();
    object->InPath = L_InPath;
    object->fullPath = L_bfullPath;
    object->onlyFilesWithStartingNames = L_onlyFilesWithStartingNames;
    object->onlyFilesWithExtensions = L_onlyFilesWithExtensions;
    return object;
}