// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Async/Future.h"


#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#endif
#include "AndroidUtils.generated.h"

/**
 * 
 */
UCLASS()
class RUNTIMEMESHIMPORTEXPORT_API UAndroidUtils : public UObject
{
	GENERATED_BODY()

public:

	UAndroidUtils();
	UFUNCTION()
	FString GetPersistentDataPath();

	UFUNCTION()
	FString GetExternalDataPath();

private:
	FDelegateHandle DelegateHandle;
#if PLATFORM_ANDROID
	jmethodID GetPersistentDataPathJMethod;
	bool CheckPermissions();
#endif
};
