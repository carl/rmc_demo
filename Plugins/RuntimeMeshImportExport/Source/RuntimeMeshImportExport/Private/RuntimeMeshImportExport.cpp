// MIT License
//
// Copyright (c) 2019 Lucid Layers

#include "RuntimeMeshImportExport.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/PlatformFile.h"
#include "HAL/PlatformProcess.h"

#define LOCTEXT_NAMESPACE "FRuntimeMeshImportExportModule"

void FRuntimeMeshImportExportModule::StartupModule()
{	
	UE_LOG(LogTemp, Warning, TEXT("--------------------------MESH-IMPORT-EXPORT_STARTUPMODULE-------------------------"));
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FString PluginBaseDir = IPluginManager::Get().FindPlugin("RuntimeMeshImportExport")->GetBaseDir();
	UE_LOG(LogTemp, Warning, TEXT("PluginBaseDirectory : %s"),*PluginBaseDir);
	FString configString;
#if UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Warning, TEXT("RELEASE"));
	configString = "Release";
#else 
	UE_LOG(LogTemp, Warning, TEXT("DEBUG"));
	configString = "debug";
#endif
	UE_LOG(LogTemp, Warning, TEXT("1"));
#if PLATFORM_WINDOWS
	UE_LOG(LogTemp, Warning, TEXT("WINDOWS"));
	FString platformString = "x64";
	FString dllFileName = FString(TEXT("assimp-vc142-mt")) + (UE_BUILD_SHIPPING ? TEXT("") : TEXT("d")) + TEXT(".dll");
	//FString dllFileName = FString(TEXT("assimp-vc142-mt.dll"));
	FString dllFile = FPaths::Combine(PluginBaseDir, FString("Source/ThirdParty/assimp/bin"), platformString, configString, dllFileName);
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*dllFile))
	{
		RMIE_LOG(Fatal, "Missing file: %s", *dllFile);
	}
	dllHandle_assimp = FPlatformProcess::GetDllHandle(*dllFile);


#elif PLATFORM_ANDROID
	UE_LOG(LogTemp, Warning, TEXT("ANDROID"));
	FString  platformString = "Android";
	//FString dllFileName = FString(TEXT("assimp-vc141-mt")) + (UE_BUILD_SHIPPING ? TEXT("") : TEXT("d")) + TEXT(".dll");
	FString dllFileName = FString(TEXT("libassimp.so"));
	FString dllFile = dllFileName;//FPaths::Combine(PluginBaseDir, FString("Source/ThirdParty/assimp/bin/Android/arm64-v8a"), dllFileName);
	UE_LOG(LogTemp, Warning, TEXT("dllFile %s"),*dllFile);
	/*if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*dllFile))
	{
		RMIE_LOG(Fatal, "Missing file: %s", *dllFile);
	}*/
	UE_LOG(LogTemp, Warning, TEXT("AfterCheck %s"), *dllFile);
	dllHandle_assimp = FPlatformProcess::GetDllHandle(*dllFile);
	if(dllHandle_assimp){
		UE_LOG(LogTemp, Warning, TEXT("DllHandleWasLoadedSuccessfully"));
		}
		{
		UE_LOG(LogTemp, Warning, TEXT("---------------------------------DLL FILE NOT FOUND------------------------"));
		}
#endif
	UE_LOG(LogTemp, Warning, TEXT("---------------------------------END------------------------"));
}

void FRuntimeMeshImportExportModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FPlatformProcess::FreeDllHandle(dllHandle_assimp);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRuntimeMeshImportExportModule, RuntimeMeshImportExport)