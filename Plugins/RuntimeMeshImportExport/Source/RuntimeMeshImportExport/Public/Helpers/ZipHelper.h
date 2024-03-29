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
#include "Helpers/miniz.h"
#include "CoreMinimal.h"

class RUNTIMEMESHIMPORTEXPORT_API ZipHelper
{
public:
    void Unzip(const FString& uinFile, const FString& uoutDir, bool toSubfolder, bool removeArchive);
    bool VerifyOrCreateDirectory(const FString& TestDir);
};