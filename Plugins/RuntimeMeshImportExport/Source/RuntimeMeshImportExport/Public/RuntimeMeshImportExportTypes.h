// MIT License
//
// Copyright (c) 2019 Lucid Layers

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInterface.h"
#include "RuntimeMeshImportExportTypes.generated.h"

struct FRuntimeMeshExportResult;
struct FRuntimeMeshImportResult;
struct FRuntimeMeshImportExportProgress;
struct aiExportFormatDesc;


DECLARE_DELEGATE(FRuntimeImportExportGameThreadDone);
DECLARE_DYNAMIC_DELEGATE(FRuntimeImportExportGameThreadDoneDyn);
DECLARE_DELEGATE_OneParam(FRuntimeExportFinished, const FRuntimeMeshExportResult /*result*/);
DECLARE_DELEGATE_OneParam(FRuntimeImportFinished, const FRuntimeMeshImportResult /*result*/);
DECLARE_DELEGATE_OneParam(FRuntimeStaticImportFinished, const FRuntimeStaticMeshImportResult /*result*/);

UENUM(BlueprintType)
enum class ERuntimeMeshImportExportProgressType : uint8
{
    Nothing = 0,
    // Only when not sure whats happening. More a precaution then a used type
    Unknown,
    // Assimp is reading the file
    AssimpFileRead,
    // Assimp is writing the data to a file
    AssimpFileWrite,
    // Assimp is processing imported data
    AssimpPostProcess,
    // Iterating scene nodes for mesh data to export
    GatheringMeshs,
    // Iterating scene nodes for meshes
    ImportingMeshes,
    // Importing material data from Assimp to Unreal
    ImportingMaterials
};

USTRUCT(BlueprintType)
struct FRuntimeMeshImportExportProgress
{
    GENERATED_BODY()
    FRuntimeMeshImportExportProgress() {}
    FRuntimeMeshImportExportProgress(ERuntimeMeshImportExportProgressType inType, int32 inCurrent, int32 inMax) : type(inType), current(inCurrent), max(inMax) {}
    // The type of operation that is currently being done
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    ERuntimeMeshImportExportProgressType type = ERuntimeMeshImportExportProgressType::Nothing;
    // The current operation that is done of type
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    int32 current = 0;
    // The maximum of operations to be done for type
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    int32 max = 0;
};

DECLARE_DELEGATE_OneParam(FRuntimeMeshImportExportProgressUpdate, const FRuntimeMeshImportExportProgress& /*status*/);
DECLARE_DYNAMIC_DELEGATE_OneParam(FRuntimeMeshImportExportProgressUpdateDyn, const FRuntimeMeshImportExportProgress&, progress);

USTRUCT(BlueprintType)
struct FRuntimeMeshExportResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bSuccess = false;

    // 	The log created during export (independent of bLogToUnreal).
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    FString exportLog = FString();

    // Error that might have happened during export.
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    FString error = FString();

    // If this is > 0, you should check the exportLog for reasons.
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    int32 numObjectsSkipped = 0;
   
};

UENUM(BlueprintType)
enum class ERotationCorrection : uint8
{
    Minus_90,
    Zero,
    Plus_90,
};

USTRUCT(BlueprintType)
struct FTransformCorrection
{
    GENERATED_BODY();

    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bFlipX = false;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bFlipY = false;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bFlipZ = false;

    UPROPERTY(BlueprintReadWrite, Category = "Default")
    ERotationCorrection RollCorrection_X = ERotationCorrection::Zero;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    ERotationCorrection PitchCorrection_Y = ERotationCorrection::Zero;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    ERotationCorrection YawCorrection_Z = ERotationCorrection::Zero;

    UPROPERTY(BlueprintReadWrite, Category = "Default")
    float scaleFactor = 1;
};

USTRUCT(BlueprintType)
struct FRuntimeMeshExportParam
{
    GENERATED_BODY()

    // Set to true to combine mesh sections with the same material within the same node
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bCombineSameMaterial = false;

    // The LOD that shall be exported
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    int32 lod = 0;

    // True: Skip the mesh if 'lod' is not available.
    // False: Mesh shall return the next possible LOD
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bSkipLodNotValid = false;

    // Can be obtained with URuntimeMeshImportExportLibrary::GetSupportedExtensionsExport
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    FString formatId;

    // Export file
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    FString file;

    // Shall we overwrite an existing file
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bOverrideExisting = false;

    // A correction that is applied to the transform of the RootNode
    // to make the object display correctly in other software
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    FTransformCorrection correction;

    // Write to Unreals log during export
    // Note: Important stuff is logged nonetheless.
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bLogToUnreal = false;

};


USTRUCT(BlueprintType)
struct FRuntimeMeshExportAsyncParam
{
    GENERATED_BODY()

    // The number of mesh data to gather per tick from exportables
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    int32 numGatherPerTick = 0;

    UPROPERTY(BlueprintReadWrite, Category = "Default")
    FRuntimeMeshExportParam param;
};


USTRUCT(BlueprintType)
struct FExportableMeshSection
{
    GENERATED_BODY();

    // The transform the mesh has to the world. If the mesh is within a ProceduralMeshComponent,
    // it is the world transform of the component.
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    FTransform meshToWorld;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    UMaterialInterface* material = nullptr;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    TArray<FVector> vertices;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    TArray<FVector> normals;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    TArray<FVector> tangents;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    TArray<FVector2D> textureCoordinates;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    TArray<FColor> vertexColors;
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    TArray<int32> triangles;

    // Append other data to this if it has the same material
    void Append(FExportableMeshSection&& other);

};

UENUM(BlueprintType)
enum class EPathType : uint8
{
    Absolute,
    ProjectRelative,
    ContentRelative,
};

UENUM(BlueprintType)
enum class EImportMethodMesh : uint8
{
    // Keep the meshes separated
    Keep,
    // Merge all meshes together to a single mesh
    Merge,
};

UENUM(BlueprintType)
enum class EImportMethodSection : uint8
{
    // Keep the sections separated
    Keep,
    // Merge all sections of a mesh together
    Merge,
    // Merge only the sections of a mesh that have the same material
    MergeSameMaterial,
};

USTRUCT(BlueprintType)
struct FRuntimeMeshImportSectionInfo
{
    GENERATED_BODY()

    // Name of the material for this section.
    // Can be None when everything gets combined.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    FName materialName;

    // Index of the material in the material list.
    // Can be -1 when everything gets combined.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    int32 materialIndex = -1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    TArray<FVector> vertices;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    TArray<int32> triangles;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    TArray<FVector> normals;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    TArray<FVector2D> uv0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    TArray<FLinearColor> vertexColors;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    TArray<FVector> tangents;

    // Append other section data to this
    void Append_Move(FRuntimeMeshImportSectionInfo&& other);
};

USTRUCT(BlueprintType)
struct FRuntimeMeshImportMeshInfo
{
    GENERATED_BODY()

    // Name of the imported mesh. Name None when merged
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    FName meshName;

    // Mesh material sections
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    TArray<FRuntimeMeshImportSectionInfo> sections;
};

USTRUCT(BlueprintType)
struct FRuntimeMeshImportExportMaterialParam
{
    GENERATED_BODY()

    FRuntimeMeshImportExportMaterialParam() {}
    FRuntimeMeshImportExportMaterialParam(FName&& inName) : name(MoveTemp(inName)) {}

    // Material parameter name
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    FName name;
};

USTRUCT(BlueprintType)
struct FRuntimeMeshImportExportMaterialParamScalar : public FRuntimeMeshImportExportMaterialParam
{
    GENERATED_BODY()

    FRuntimeMeshImportExportMaterialParamScalar() {}
    FRuntimeMeshImportExportMaterialParamScalar(FName&& inName, const float& inValue)
        : FRuntimeMeshImportExportMaterialParam(MoveTemp(inName)), value(inValue)
    {}

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    float value = 0.f;
};

USTRUCT(BlueprintType)
struct FRuntimeMeshImportExportMaterialParamVector : public FRuntimeMeshImportExportMaterialParam
{
    GENERATED_BODY()

    FRuntimeMeshImportExportMaterialParamVector() {}
    FRuntimeMeshImportExportMaterialParamVector(FName&& inName, const FLinearColor& inValue)
        : FRuntimeMeshImportExportMaterialParam(MoveTemp(inName)), value(inValue)
    {}

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    FLinearColor value = FLinearColor::Black;
};

USTRUCT(BlueprintType)
struct FRuntimeMeshImportExportMaterialParamTexture : public FRuntimeMeshImportExportMaterialParam
{
    GENERATED_BODY()

    FRuntimeMeshImportExportMaterialParamTexture() {}
    FRuntimeMeshImportExportMaterialParamTexture(FName inName) : FRuntimeMeshImportExportMaterialParam(MoveTemp(inName)) {}

    // CONSULT THE ASSIMP DOCUMENTATION ABOUT THE MEANING IN aiTexture
    int32 width = 0;
    // CONSULT THE ASSIMP DOCUMENTATION ABOUT THE MEANING IN aiTexture
    int32 height = 0;
    // CONSULT THE ASSIMP DOCUMENTATION ABOUT THE MEANING IN aiTexture
    FString byteDescription;

    // Each texture is imported as byte array.
    // NOTE: For each texture type e.g. Diffuse, Assimp has a texture stack. Though the plugin for now only imports the first texture within the stack.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    TArray<uint8> byteData;
};


// Mirror of Assimp material.h::aiShadingMode
UENUM(BlueprintType)
enum class ERuntimeMeshImportExportMaterialShadingMode : uint8
{
    // Zero entry. Needed for compilation. No valid shading mode
    REQUIRED = 0x0,

    // Flat shading. Shading is done on per-face base, diffuse only. Also known as 'faceted shading'.
    Flat = 0x1,

    // Simple Gouraud shading.
    Gouraud = 0x2,

    // Phong-Shading -
    Phong = 0x3,

    // Phong-Blinn-Shading
    Blinn = 0x4,

    // Toon-Shading per pixel. Also known as 'comic' shader.
    Toon = 0x5,

    // OrenNayar-Shading per pixel. Extension to standard Lambertian shading, taking the roughness of the material into account
    OrenNayar = 0x6,

    // Minnaert-Shading per pixel. Extension to standard Lambertian shading, taking the "darkness" of the material into account
    Minnaert = 0x7,

    // CookTorrance-Shading per pixel. Special shader for metallic surfaces.
    CookTorrance = 0x8,

    // No shading at all. Constant light influence of 1.0.
    NoShading = 0x9,

    // Fresnel shading
    Fresnel = 0xa,

    // Only used to determine type
    PRIVATE_Max,

    Unknown = 0xFF,
};
#define MaterialShadingModeFromInt(mode) \
 (mode == 0 || (uint8)mode > (uint8)ERuntimeMeshImportExportMaterialShadingMode::PRIVATE_Max) \
        ? ERuntimeMeshImportExportMaterialShadingMode::Unknown \
        : ERuntimeMeshImportExportMaterialShadingMode((uint8)mode);

// Mirror of Assimp material.h::aiBlendMode
UENUM(BlueprintType)
enum class ERuntimeMeshImportExportMaterialBlendMode : uint8
{
    Default = 0x0,
    Additive = 0x1,
    // Only used to determine type
    PRIVATE_Max,
    Unknown = 0xFF,
};
#define MaterialBlendModeFromInt(mode) \
(mode == 0 || (uint8)mode > (uint8)ERuntimeMeshImportExportMaterialBlendMode::PRIVATE_Max) \
? ERuntimeMeshImportExportMaterialBlendMode::Unknown \
: ERuntimeMeshImportExportMaterialBlendMode((uint8)mode);

USTRUCT(BlueprintType)
struct FRuntimeMeshImportMaterialInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    FName name;

    // TwoSided must be specified by the material defaults
    bool bTwoSided = false;
    // Wireframe must be specified by the material defaults
    bool bWireFrame = false;

    // ShadingMode must be specified by the material defaults
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    ERuntimeMeshImportExportMaterialShadingMode shadingMode = ERuntimeMeshImportExportMaterialShadingMode::REQUIRED;
    // Value read from Assimp material data. Might come in handy if shadingMode == Unknown
    // -1 if the shading mode was not written to the material
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    int32 shadingModeInt = -1;

    // BlendMode must be specified by the material defaults
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    ERuntimeMeshImportExportMaterialBlendMode blendMode = ERuntimeMeshImportExportMaterialBlendMode::Default;
    // Value read from Assimp material data. Might come in handy if blendMode == Unknown
    // -1 if the blend mode was not written to the material
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    int32 blendModeInt = -1;

    // Scalars that can be specified as parameters in the material graph
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    TArray<FRuntimeMeshImportExportMaterialParamScalar> scalars;

    // Vectors that can be specified as parameters in the material graph
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    TArray<FRuntimeMeshImportExportMaterialParamVector> vectors;

    // Textures that can be specified as parameters in the material graph
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    TArray<FRuntimeMeshImportExportMaterialParamTexture> textures;
};

USTRUCT(BlueprintType)
struct FRuntimeMeshImportResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    bool bSuccess = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    TArray<FRuntimeMeshImportMeshInfo> meshInfos;

    // Materials will only get imported when sectionImportMethod != EImportMethodSection::Merge
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    TArray<FRuntimeMeshImportMaterialInfo> materialInfos;
};

USTRUCT(BlueprintType)
struct FRuntimeStaticMeshImportResult
{
    GENERATED_BODY()

        UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
        bool bSuccess = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
        UStaticMesh* mesh;
};

USTRUCT(BlueprintType)
struct FAssimpExportFormat
{
    GENERATED_BODY()

    FAssimpExportFormat() {}

    FAssimpExportFormat(const aiExportFormatDesc* desc);

    // a short string ID to uniquely identify the export format. Use this ID string to
    // specify which file format you want to export to when calling #aiExportScene().
    // Example: "dae" or "obj"
    UPROPERTY(BlueprintReadOnly, Category = "Default")
    FString id;

    // A short description of the file format to present to users. Useful if you want
    // to allow the user to select an export format.
    UPROPERTY(BlueprintReadOnly, Category = "Default")
    FString description;

    // Recommended file extension for the exported file in lower case.
    UPROPERTY(BlueprintReadOnly, Category = "Default")
    FString fileExtension;
};

/**
 *	PostProcess flags for the Assimp Importer.
 *	This are not all possible flags, but the ones that seem useful to be exposed.
 */
USTRUCT(BlueprintType)
struct FAssimpImportPostProcessFlags
{
    GENERATED_BODY()

    unsigned int GetAsInt() const;

    /**
     * Calculates the tangents and bitangents for the imported meshes.
     *
     * Does nothing if a mesh does not have normals. You might want this post
     * processing step to be executed if you plan to use tangent space calculations
     * such as normal mapping applied to the meshes.
     */
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bCalculateTangentSpace = true;

    /**
     * Identifies and joins identical vertex data sets within all
     * imported meshes.
     *
     * After this step is run, each mesh contains unique vertices,
     * so a vertex may be used by multiple faces. You usually want
     * to use this post processing step. If your application deals with
     * indexed geometry, this step is compulsory or you'll just waste rendering
     * time. <b>If this flag is not specified</b>, no vertices are referenced by
     * more than one face and <b>no index buffer is required</b> for rendering.
     */
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bJoinIdenticalVertices = false;

    /**
     * Generates smooth normals for all vertices in the mesh.
     *
     * This is ignored if normals are already there at the time this flag
     * is evaluated. Model importers try to load them from the source file, so
     * they're usually already there.
     */
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bGenerateSmoothNormals = true;

    /**
     * Reorders triangles for better vertex cache locality.
     *
     * The step tries to improve the ACMR (average post-transform vertex cache
     * miss ratio) for all meshes. The implementation runs in O(n) and is
     * roughly based on the 'tipsify' algorithm (see <a href="
     * http://www.cs.princeton.edu/gfx/pubs/Sander_2007_%3ETR/tipsy.pdf">this
     * paper</a>).
     */
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bImproveCacheLocality = false;

    /**
     * This step tries to determine which meshes have normal vectors
     * that are facing inwards and inverts them.
     *
     * The algorithm is simple but effective:
     * the bounding box of all vertices + their normals is compared against
     * the volume of the bounding box of all vertices without their normals.
     * This works well for most objects, problems might occur with planar
     * surfaces. However, the step tries to filter such cases.
     * The step inverts all in-facing normals. Generally it is recommended
     * to enable this step, although the result is not always correct.
     */
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bFixInfacingNormals = false;

    /**
     *  This step searches for duplicate meshes and replaces them
     *  with references to the first mesh.
     *
     *  This step takes a while, so don't use it if speed is a concern.
     *  Its main purpose is to workaround the fact that many export
     *  file formats don't support instanced meshes, so exporters need to
     *  duplicate meshes. This step removes the duplicates again. Please
     *  note that Assimp does not currently support per-node material
     *  assignment to meshes, which means that identical meshes with
     *  different materials are currently *not* joined, although this is
     *  planned for future versions.
     */
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bFindInstances = false;

    /**
     *  A post-processing step to reduce the number of meshes.
     *
     *  This will, in fact, reduce the number of draw calls.
     *
     *  This is a very effective optimization and is recommended to be used
     *  together with #aiProcess_OptimizeGraph, if possible.
     */
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bOptimizeMeshes = true;


    /**
     *  A post-processing step to optimize the scene hierarchy.
     *
     *  Nodes without animations, bones, lights or cameras assigned are
     *  collapsed and joined.
     *
     *  Use this flag with caution. Most simple files will be collapsed to a
     *  single node, so complex hierarchies are usually completely lost. This is not
     *  useful for editor environments, but probably a very effective
     *  optimization if you just want to get the model data, convert it to your
     *  own format, and render it as fast as possible.
     *
     *  This flag is designed to be used with #aiProcess_OptimizeMeshes for best
     *  results.
     *
     *  @note 'Crappy' scenes with thousands of extremely small meshes packed
     *  in deeply nested nodes exist for almost all file formats.
     *  #aiProcess_OptimizeMeshes in combination with #aiProcess_OptimizeGraph
     *  usually fixes them all and makes them renderable.
     */
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bOptimizeGraph = false;

    /**
     * Drops normals for all faces of all meshes.
     *
     * This is ignored if no normals are present.
     */
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bDropNormals = false;
};

USTRUCT(BlueprintType)
struct FRuntimeMeshImportParameter
{
    GENERATED_BODY()

    // The file to import. Depending on 'pathType'
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    FString file;

    // Transform that is applied to the imported scene
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    FTransform transform;

    // Choose whether the 'file' provided is absolute or relative
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    EPathType pathType = EPathType::Absolute;

    // Choose how meshes shall be treated on import (applied before 'importMethodSection')
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    EImportMethodMesh importMethodMesh = EImportMethodMesh::Keep;

    // Choose how mesh sections are treated on import (applied after 'importMethodMesh')
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    EImportMethodSection importMethodSection = EImportMethodSection::MergeSameMaterial;

    // When checked, the scene is transformed to fit into a 100cm cube, objects placed around the center.
    UPROPERTY(BlueprintReadWrite, Category = "Default")
    bool bNormalizeScene = false;

    UPROPERTY(BlueprintReadWrite, Category = "Default")
    FAssimpImportPostProcessFlags assimpPostProcessFlags;
};