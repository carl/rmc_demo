// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers/HelperParameters.h"
#include "Dom/JsonObject.h"
#include <JsonObjectConverter.h>

void SerializeParamsSet(TSharedPtr<FJsonObject> root, const FString& name, const TArray<FParameter>& set)
{
    if (!set.Num()) { return; }
    TSharedPtr<FJsonObject> setJson = MakeShareable(new FJsonObject);

    TArray<TSharedPtr<FJsonValue>> objArray;
    FString subset = set[0].Set;
    for (int i = 0; i < set.Num(); i++)
    {
        TSharedPtr< FJsonObject > jsonObj = MakeShareable(new FJsonObject);
        TSharedRef<FJsonValueObject> jsonValue = MakeShareable(new FJsonValueObject(jsonObj));
        if (set[i].Set != subset)
        {
            setJson->SetArrayField(subset, objArray);
            subset = set[i].Set;
            objArray.Empty();
        }
        objArray.Add(MakeShareable(new FJsonValueString(set[i].Name)));
    }
    setJson->SetArrayField(subset, objArray);
    root->SetObjectField(name, setJson);
}

bool HasValue(EBoolProperty var)
{
    return var != EBoolProperty::NotSet;
}

bool HasValue(FMeshRgb var)
{
    return (0 <= var.B && var.B <= 255 &&
        0 <= var.G && var.G <= 255 &&
        0 <= var.R && var.R <= 255);
}


bool HasValue(int var)
{
    return 0 <= var && var <= INT_MAX;
}

void SerializeValue(TSharedPtr<FJsonObject> root, EBoolProperty var, const FString& key)
{
    root->SetBoolField(key, var == EBoolProperty::On);
}

void SerializeValue(TSharedPtr<FJsonObject> root, int var, const FString& key)
{
    root->SetNumberField(key, var);
}

void SerializeValue(TSharedPtr<FJsonObject> root, FMeshRgb var, const FString& key)
{
    TSharedPtr< FJsonObject > jsonObj = MakeShareable(new FJsonObject);
    TSharedRef<FJsonValueObject> jsonValue = MakeShareable(new FJsonValueObject(jsonObj));
    jsonObj->SetNumberField("red", var.R);
    jsonObj->SetNumberField("green", var.G);
    jsonObj->SetNumberField("blue", var.B);

    root->SetObjectField(key, jsonObj);
}

template <class T>
void SerializeProperty(TSharedPtr<FJsonObject> root, const T& property, const FString& key)
{
    if (HasValue(property)) {
        SerializeValue(root, property, key);
    }
}

void SerializeAvatarModifications(TSharedPtr<FJsonObject> root, const FMeshModifications& modifications)
{
    TSharedPtr<FJsonObject> avatarModifications = MakeShareable(new FJsonObject);
    TSharedPtr<FJsonObject> setJson = MakeShareable(new FJsonObject);
    SerializeProperty(setJson, modifications.AddEyelidShadow, "add_eyelid_shadow");
    SerializeProperty(setJson, modifications.AddGlare, "add_glare");
    SerializeProperty(setJson, modifications.AllowModifyNeck, "allow_modify_neck");
    SerializeProperty(setJson, modifications.CurvedBottom, "curved_bottom");
    SerializeProperty(setJson, modifications.EnhanceLighting, "enhance_lighting");
    SerializeProperty(setJson, modifications.EyeIrisColor, "eye_iris_color");
    SerializeProperty(setJson, modifications.EyeScleraColor, "eye_sclera_color");
    SerializeProperty(setJson, modifications.GeneratedHaircutFacesCount, "generated_haircut_faces_count");
    SerializeProperty(setJson, modifications.GeneratedHaircutTextureSize, "generated_haircut_texture_size");
    SerializeProperty(setJson, modifications.HairColor, "hair_color");
    SerializeProperty(setJson, modifications.ParametricEyesTexture, "parametric_eyes_texture");
    SerializeProperty(setJson, modifications.RemoveGlasses, "remove_glasses");
    SerializeProperty(setJson, modifications.RemoveSmile, "remove_smile");
    SerializeProperty(setJson, modifications.SlightlyCartoonishTexture, "slightly_cartoonish_texture");
    SerializeProperty(setJson, modifications.TeethColor, "teeth_color");
    SerializeProperty(setJson, modifications.TextureSize, "texture_size");

    avatarModifications->SetObjectField("plus", setJson);
    root->SetObjectField("avatar_modifications", avatarModifications);

}
bool UHelperParameters::LodToInt(EMeshLod lod, int& lodInt)
{
    return false;
}

FString UHelperParameters::LodToString(EMeshLod lod)
{
    return FString();
}