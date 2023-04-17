// Fill out your copyright notice in the Description page of Project Settings.LogTemp


#include "Helpers/AndroidUtils.h"
#if PLATFORM_ANDROID
#include "AndroidPermissionFunctionLibrary.h"
#include "AndroidPermissionCallbackProxy.h"
#include "..\..\Public\Helpers\AndroidUtils.h"
#endif


UAndroidUtils::UAndroidUtils(){
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		GetPersistentDataPathJMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetPersistentDataPath", "()Ljava/lang/String;", false);
	}
#endif
}
FString UAndroidUtils::GetPersistentDataPath()
{
#if PLATFORM_ANDROID
	FString filePath;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, UAndroidUtils::GetPersistentDataPathJMethod);
		const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
		filePath = FString(UTF8_TO_TCHAR(JavaChars));

		UE_LOG(LogTemp, Verbose, TEXT("UAndroidUtils: GetPersistentDataPath result : %s"), *filePath);
	}
	return filePath;
#else 
	TArray<bool> arr;

	return FString("");
#endif
}

FString UAndroidUtils::GetExternalDataPath()
{
	return FString();
}

#if PLATFORM_ANDROID
bool UAndroidUtils::CheckPermissions()
{
	TArray <FString> permissionsRequired = { "android.permission.WRITE_EXTERNAL_STORAGE" };
	TArray <FString> permissionsToRequest;
	for (int i = 0; i < permissionsRequired.Num(); i++)
	{
		if (!UAndroidPermissionFunctionLibrary::CheckPermission(permissionsRequired[i]))
		{
			permissionsToRequest.Add(permissionsRequired[i]);
		}
	}

	if (permissionsToRequest.Num())
	{
		if (UAndroidPermissionCallbackProxy* callback = UAndroidPermissionFunctionLibrary::AcquirePermissions(permissionsToRequest))
		{
			TPromise<bool> permissionsPromise;
			auto permissionsFuture = permissionsPromise.GetFuture();
			callback->OnPermissionsGrantedDelegate.AddLambda([this, &permissionsPromise, &permissionsToRequest](const TArray<FString>& Permissions, const TArray<bool>& GrantResults)
				{
					bool status = GrantResults.Contains(false) ? false : true;
					permissionsPromise.EmplaceValue(status);
				});
			permissionsFuture.Wait();
			bool permissionsSuccess = permissionsFuture.Get();
			return permissionsSuccess;
		}
	}
	else
	{
		return true;
	}
	return false;
}
#endif
