// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers/HTTPHelper.h"
#include "Helpers/StorageHelper.h"
#include "Async/Async.h"
#include "UObject/Class.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "JsonObjectConverter.h"
#include <algorithm>
//#include "ZipHelper.h"

void UHTTPHelper::DownloadFileByUrl(const FString& Url, const FString& PathToSave,EMeshFile fileType, FAsyncFileResultDelegate OnComplete)
{
    UE_LOG(LogTemp, Log, TEXT("RuntimeImportHTTPLibrary: DownloadFileByUrl: url: %s, path to save: %s"), *Url, *PathToSave);

    FHttpModule* Http = &FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

    Request->SetVerb("GET");
    Request->SetURL(Url);

//    Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *connection.Token));
  /*  if (!connection.PlayerUID.IsEmpty())
    {
        Request->SetHeader("X-PlayerUID", connection.PlayerUID);
    }*/

    Request->OnProcessRequestComplete().BindLambda([OnComplete, PathToSave,fileType](FHttpRequestPtr HttpRequest, FHttpResponsePtr Response, bool bWasSuccessful)
        {
            int32 ReturnCode = HandleResponseCode(Response);
            if (ReturnCode)
            {
                UE_LOG(LogTemp, Error, TEXT("RuntimeImportHTTPLibrary: DownloadFileByUrl: Error code %d"), ReturnCode);
                OnComplete.ExecuteIfBound(PathToSave, ReturnCode);
                return;
            }
            FString Content = Response->GetContentAsString();
            TArray<uint8> ByteData = Response->GetContent();
            if (FPaths::FileExists(PathToSave))
            {
                UE_LOG(LogTemp, Warning, TEXT("RuntimeImportHTTPLibrary: DownloadFileByUrl: file already exists %s"), *PathToSave);
                UStorageHelper::DeleteFile(PathToSave);
            }

            if (!FFileHelper::SaveArrayToFile(ByteData, *PathToSave))
            {
                UE_LOG(LogTemp, Error, TEXT("RuntimeImportHTTPLibrary: DownloadFileByUrl: cannot write file %s"), *PathToSave);
                OnComplete.ExecuteIfBound("Can't write a file", DEFAULT_ERR_CODE);
                return;
    
            }
            else
            {
                OnComplete.ExecuteIfBound(PathToSave, 0);
            }
        });
    Request->ProcessRequest();

}

void UHTTPHelper::GetMeshTextures(const FConnection& connection, const FMeshData& MeshData, FAsyncDataDelegate OnComplete)
{
}

void UHTTPHelper::GetMeshFile(const FString& URL, const FString& dstDirPath, const FString& fileName, EMeshFile fileType, FAsyncFileResultDelegate OnComplete)
{
    FString url, dst;
    switch (fileType)
    {
    case EMeshFile::ZIP:
        url = URL;
        dst = UStorageHelper::GetMeshFilename(dstDirPath,fileName,FString("zip"));
        break;
    
    case EMeshFile::FBX:
        url = URL;
        dst = UStorageHelper::GetMeshFilename(dstDirPath, fileName, FString("fbx"));
        break;
    
    case EMeshFile::OBJ:
        url = URL;
        dst = UStorageHelper::GetMeshFilename(dstDirPath, fileName, FString("obj"));
        break;

    case EMeshFile::GLTF:
        url = URL;
        dst = UStorageHelper::GetMeshFilename(dstDirPath, fileName, FString("gltf"));
        break;

    case EMeshFile::GLB:
        url = URL;
        dst = UStorageHelper::GetMeshFilename(dstDirPath, fileName, FString("glb"));
        break;

    case EMeshFile::PLY:
        url = URL;
        dst = UStorageHelper::GetMeshFilename(dstDirPath, fileName, FString("ply"));
        break;

    case EMeshFile::STL:
        url = URL;
        dst = UStorageHelper::GetMeshFilename(dstDirPath, fileName, FString("stl"));
        break;
    default:
        url = URL;
        dst = UStorageHelper::GetMeshFilename(dstDirPath, fileName, FString("zip"));
        break;
    }
    UE_LOG(LogTemp, Warning, TEXT("AvatarSDKHTTPLibrary: Download file: %s %s"), *url, *dst);
    UHTTPHelper::DownloadFileByUrl(url, dst, fileType, OnComplete);
}

TArray<uint8> UHTTPHelper::StringToByteArray(const FString& String) 
{
    return TArray<uint8>();
}

FString UHTTPHelper::ParseStringParamFromJsonString(FString JsonString, FString Key)
{
    return FString();
}

int32 UHTTPHelper::ParseIntParamFromJsonString(FString JsonString, FString Key)
{
    return int32();
}

int32 UHTTPHelper::HandleResponseCode(FHttpResponsePtr Response)
{
    int32 responseCode = Response->GetResponseCode();
    if (!Response.IsValid() || !responseCode)
    {
        return DEFAULT_ERR_CODE;
    }
    else if (responseCode != 200 && responseCode != 201)
    {
        return Response->GetResponseCode();
    }
    return 0;
}
