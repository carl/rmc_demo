// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HelperParameters.generated.h"

/**
 * 
 */


//UENUM(BlueprintType)
//enum class EPipelineType : uint8
//{
//    head_2_0_bust_mobile                UMETA(DisplayName = "head_2.0|bust/mobile"),
//    head_2_0_head_mobile                UMETA(DisplayName = "head_2.0|head/mobile"),
//};

UENUM(BlueprintType)
enum class EMeshLod : uint8
{
    default_lod                 UMETA(DisplayName = "LOD Default"),
    avatar_lod_0                UMETA(DisplayName = "LOD 0"),
    avatar_lod_1                UMETA(DisplayName = "LOD 1"),
    avatar_lod_2                UMETA(DisplayName = "LOD 2"),
    avatar_lod_3                UMETA(DisplayName = "LOD 3"),
    avatar_lod_4                UMETA(DisplayName = "LOD 4"),
    avatar_lod_5                UMETA(DisplayName = "LOD 5"),
    avatar_lod_6                UMETA(DisplayName = "LOD 6"),
    avatar_lod_7                UMETA(DisplayName = "LOD 7"),
};
USTRUCT(BlueprintType)
struct RUNTIMEMESHIMPORTEXPORT_API FMeshTexture
{
    GENERATED_BODY()
        UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString File;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString Identity;
};

USTRUCT(BlueprintType)
struct RUNTIMEMESHIMPORTEXPORT_API FConnection
{
public:
    GENERATED_BODY()
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeImport|Params")
        FString ClientId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeImport|Params")
        FString ClientSecret;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeImport|Params")
        FString PlayerUID;
    UPROPERTY()
        FString Token;
};


USTRUCT(BlueprintType)
struct RUNTIMEMESHIMPORTEXPORT_API FMeshData
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString Code;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString Url;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString Status;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        int Progress;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString MeshUrl;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString TextureUrl;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString PreviewUrl;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString ThumbnailUrl;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        TArray<FMeshTexture> Textures;
    
    FMeshData()
    {
        Code = FString();
        Url = FString();
        Status = FString();
        Progress = 0;
        MeshUrl = FString();
        TextureUrl = FString();
        PreviewUrl = FString();
        ThumbnailUrl = FString();
        Textures = TArray<FMeshTexture>();
    }

};

USTRUCT(BlueprintType)
struct RUNTIMEMESHIMPORTEXPORT_API FMeshParams
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString ImagePath;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString Name;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString Description;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString JsonParams;
};


USTRUCT(BlueprintType)
struct RUNTIMEMESHIMPORTEXPORT_API FParameter
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString Set;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString Name;
};

USTRUCT(BlueprintType)
struct RUNTIMEMESHIMPORTEXPORT_API FAvatarGenerationParams
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        TArray<FParameter> ModelInfo;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        TArray<FParameter> AdditionalTextures;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        TArray<FParameter> AvatarModifications;
};

UENUM()
enum class EParametersType : uint8
{
    available_params           UMETA(DisplayName = "available"),
    default_params             UMETA(DisplayName = "default"),
};

USTRUCT(BlueprintType)
struct RUNTIMEMESHIMPORTEXPORT_API FMeshRgb
{
    GENERATED_BODY()
        UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (ClampMin = "0", ClampMax = "255", MakeStructureDefaultValue = "-1"))
        int R = -1;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (ClampMin = "0", ClampMax = "255", MakeStructureDefaultValue = "-1"))
        int G = -1;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (ClampMin = "0", ClampMax = "255", MakeStructureDefaultValue = "-1"))
        int B = -1;
};

UENUM()
enum class EBoolProperty : uint8
{
    NotSet         UMETA(DisplayName = "Not Set"),
    On             UMETA(DisplayName = "True"),
    Off            UMETA(DisplayName = "False"),
};

USTRUCT(BlueprintType)
struct RUNTIMEMESHIMPORTEXPORT_API FMeshModifications
{
    GENERATED_BODY()
        UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (SKey = "add_eyelid_shadow", SSet = "plus"))
        EBoolProperty AddEyelidShadow = EBoolProperty::NotSet;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (SKey = "add_glare", SSet = "plus"))
        EBoolProperty AddGlare = EBoolProperty::NotSet;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (SKey = "allow_modify_neck", SSet = "plus"))
        EBoolProperty AllowModifyNeck = EBoolProperty::NotSet;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (SKey = "curved_bottom", SSet = "plus"))
        EBoolProperty CurvedBottom = EBoolProperty::NotSet;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (SKey = "enhance_lighting", SSet = "plus"))
        EBoolProperty EnhanceLighting = EBoolProperty::NotSet;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (SKey = "eye_iris_color", SSet = "plus"))
        FMeshRgb EyeIrisColor;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (SKey = "eye_sclera_color", SSet = "plus"))
        FMeshRgb EyeScleraColor;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (MakeStructureDefaultValue = "-1", SKey = "generated_haircut_faces_count", SSet = "plus"))
        int GeneratedHaircutFacesCount = -1;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (MakeStructureDefaultValue = "-1", SKey = "generated_haircut_texture_size", SSet = "plus"))
        int GeneratedHaircutTextureSize = -1;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (SKey = "hair_color", SSet = "plus"))
        FMeshRgb HairColor;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (SKey = "parametric_eyes_texture", SSet = "plus"))
        EBoolProperty ParametricEyesTexture = EBoolProperty::NotSet;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (SKey = "remove_smile", SSet = "plus"))
        EBoolProperty RemoveSmile = EBoolProperty::NotSet;;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (SKey = "remove_glasses", SSet = "plus"))
        EBoolProperty RemoveGlasses = EBoolProperty::NotSet;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (SKey = "slightly_cartoonish_texture", SSet = "plus"))
        EBoolProperty SlightlyCartoonishTexture = EBoolProperty::NotSet;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (SKey = "teeth_color", SSet = "plus"))
        FMeshRgb TeethColor;
    UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (MakeStructureDefaultValue = "-1", SKey = "texture_size", SSet = "plus"))
        int TextureSize = -1;
};


UCLASS()
class RUNTIMEMESHIMPORTEXPORT_API UHelperParameters : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utils")
        static bool LodToInt(EMeshLod lod, int& lodInt);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RuntimeImport|Utils")
        static FString LodToString(EMeshLod lod);
	
};


USTRUCT(BlueprintType)
struct RUNTIMEMESHIMPORTEXPORT_API FModelInfo
{
    GENERATED_BODY()
        UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString Age;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FMeshRgb EyeIrisColor;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FMeshRgb EyeScleraColor;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        TArray<float> FacialLandmarks;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString Gender;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FMeshRgb HairColor;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FString Race;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
        FMeshRgb SkinColor;
};

