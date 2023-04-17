// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Helpers/StorageHelper.h"
#include "Helpers/HelperParameters.h"
#include "HTTPHelper.generated.h"



DECLARE_DELEGATE_TwoParams(FAsyncRequestCompleteDelegate, FString, int32);
DECLARE_DELEGATE_TwoParams(FAsyncFileResultDelegate, FString, int32);
DECLARE_DELEGATE_TwoParams(FAsyncDataDelegate, FMeshData, int32);
/**
 * 
 */
UCLASS()
class RUNTIMEMESHIMPORTEXPORT_API UHTTPHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
		static void DownloadFileByUrl(const FString& Url, const FString& PathToSave,EMeshFile fileType, FAsyncFileResultDelegate OnComplete);
		
		static void GetMeshTextures(const FConnection& connection, const FMeshData& MeshData, FAsyncDataDelegate OnComplete);
    
        static void GetMeshFile(const FString& URL, const FString& dstDirPath,const FString& fileName, EMeshFile fileType, FAsyncFileResultDelegate OnComplete);
       

        static const int DEFAULT_ERR_CODE = -1;

        UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Http|Tools")
            static TArray<uint8> StringToByteArray(const FString& String);

        UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Http|Tools")
            static FString ParseStringParamFromJsonString(FString JsonString, FString Key);

        UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Http|Tools")
            static int32 ParseIntParamFromJsonString(FString JsonString, FString Key);


        static int32 HandleResponseCode(FHttpResponsePtr Response);
        static const FString RootApiUrl;
private:

};
