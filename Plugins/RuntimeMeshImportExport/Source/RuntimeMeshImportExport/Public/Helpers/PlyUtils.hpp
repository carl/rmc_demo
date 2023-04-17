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
#include <Helpers/tinyply.h>
#include "CoreMinimal.h"

namespace RuntimeImportPlyUtils
{
    struct RUNTIMEMESHIMPORTEXPORT_API vert3
    {
        float x;
        float y;
        float z;
    };

    struct RUNTIMEMESHIMPORTEXPORT_API faceUv
    {
        float u;
        float v;
    };

    struct RUNTIMEMESHIMPORTEXPORT_API faceUv3
    {
        faceUv uv[3];
    };

    struct RUNTIMEMESHIMPORTEXPORT_API faceVerIndex3
    {
        int32_t v[3];
    };

    RUNTIMEMESHIMPORTEXPORT_API bool operator==(const vert3& v1, const vert3& v2);
    RUNTIMEMESHIMPORTEXPORT_API bool operator!=(const vert3& v1, const vert3& v2);
    RUNTIMEMESHIMPORTEXPORT_API bool operator==(const faceUv& uv1, const faceUv& uv2);
    RUNTIMEMESHIMPORTEXPORT_API bool operator!=(const faceUv& uv1, const faceUv& uv2);

    RUNTIMEMESHIMPORTEXPORT_API void readPly(const FString& plyFilepath, bool preload,
        std::vector<vert3>& vertsResult,
        std::vector<faceUv>& uvResult,
        std::vector<faceVerIndex3>& facesResult,
        std::vector<int>& duplicatesMap);
}