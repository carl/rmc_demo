/* Copyright (C) Itseez3D, Inc. - All Rights Reserved
 * You may not use this file except in compliance with an authorized license
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * UNLESS REQUIRED BY APPLICABLE LAW OR AGREED BY ITSEEZ3D, INC. IN WRITING, SOFTWARE DISTRIBUTED UNDER THE LICENSE IS DISTRIBUTED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED
 * See the License for the specific language governing permissions and limitations under the License.
 * Written by Itseez3D, Inc. <support@itseez3D.com>, May 2020
 */

#include "Helpers/ZipHelper.h"
#include "Misc/Paths.h"
#include <string>
#include <vector>
#include "HAL/PlatformFilemanager.h"

std::string outDir;

void ExtractFiles(mz_zip_archive& m_ziparchive, std::vector<std::string>& m_files);

bool ZipHelper::VerifyOrCreateDirectory(const FString& TestDir)
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    // Directory Exists?
    if (!PlatformFile.DirectoryExists(*TestDir))
    {
        PlatformFile.CreateDirectory(*TestDir);

        if (!PlatformFile.DirectoryExists(*TestDir))
        {
            return false;
        }
    }
    return true;
}

void ZipHelper::Unzip(const FString& uinFile, const FString& uoutDir, bool toSubfolder, bool removeArchive)
{
    std::string zipFile;
    zipFile = std::string(TCHAR_TO_UTF8(*uinFile));
    if (toSubfolder)
    {
        FString subdirName = FPaths::GetBaseFilename(uinFile);
        FString subdir = FPaths::Combine(uoutDir, subdirName);
        VerifyOrCreateDirectory(subdir);
        outDir = std::string(TCHAR_TO_UTF8(*subdir));
    }
    else 
    {
        outDir = std::string(TCHAR_TO_UTF8(*uoutDir));
    }

    std::vector<std::string> files = {};
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));

    auto status = mz_zip_reader_init_file(&zip_archive, zipFile.c_str(), 0);
    if (!status) return;// files;
    ExtractFiles(zip_archive, files);
    //int fileCount = (int)mz_zip_reader_get_num_files(&zip_archive);
    //if (fileCount == 0)
    //{
    //    mz_zip_reader_end(&zip_archive);
    //    return;// files;
    //}
    //mz_zip_archive_file_stat file_stat;
    //if (!mz_zip_reader_file_stat(&zip_archive, 0, &file_stat))
    //{
    //    mz_zip_reader_end(&zip_archive);
    //    return;// files;
    //}

    //// Get and print information about each file in the archive.
    //for (int i = 0; i < fileCount; i++)
    //{
    //    if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) continue;
    //    if (mz_zip_reader_is_file_a_directory(&zip_archive, i))
    //    { // skip directories for now
    //        std::string folderpath = outDir + '/' + file_stat.m_filename + '/';
    //        mz_zip_archive zip_archivefolder;
    //        memset(&zip_archivefolder, 0, sizeof(zip_archivefolder));
    //        auto status = mz_zip_reader_init_file(&zip_archivefolder, folderpath.c_str(), 0);
    //        
    //    }
    //    std::string destFile = outDir + '/' + file_stat.m_filename;
    //    if (mz_zip_reader_extract_to_file(&zip_archive, i, destFile.c_str(), 0))
    //    {
    //        files.emplace_back(destFile);
    //    }
    //}

    //// Close the archive, freeing any resources it was using
    //mz_zip_reader_end(&zip_archive);
    //return files;
    if (removeArchive) {
        IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
        PlatformFile.DeleteFile(*uinFile);
    }
}

void ExtractFiles(mz_zip_archive& m_ziparchive, std::vector<std::string>& m_files)
{
    int fileCount = (int)mz_zip_reader_get_num_files(&m_ziparchive);
    
    // Get and print information about each file in the archive.
    if (fileCount == 0)
    {
        mz_zip_reader_end(&m_ziparchive);
        return;// files;
    }
    mz_zip_archive_file_stat file_stat;
    if (!mz_zip_reader_file_stat(&m_ziparchive, 0, &file_stat))
    {
        mz_zip_reader_end(&m_ziparchive);
        return;// files;
    }
    
    for (int i = 0; i < fileCount; i++)
    {
        if (!mz_zip_reader_file_stat(&m_ziparchive, i, &file_stat)) continue;
        if (mz_zip_reader_is_file_a_directory(&m_ziparchive, i))
        { // skip directories for now
            std::string folderpath = outDir + file_stat.m_filename;
            IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

            // Directory Exists?
        /*    if (!PlatformFile.DirectoryExists(&UTF8_TO_TCHAR(folderpath)))
            {
                PlatformFile.CreateDirectory(&UTF8_TO_TCHAR(folderpath));*/
                void * folder = nullptr;
                size_t folderSize = (size_t)file_stat.m_uncomp_size;
                folder = mz_zip_reader_extract_to_heap(&m_ziparchive,i, &folderSize,0);
                mz_bool isValid = mz_zip_validate_file(&m_ziparchive,i,0);
                if(!folder)
                {
                    mz_free(folder);
                    continue;
                }
                else
                {
                    std::string str_unzip;
                    str_unzip.assign((const char *)folder,folderSize);
                    mz_zip_archive folderarchive;
                    memset(&folderarchive, 0, sizeof(folderarchive));
                    
                    auto status = mz_zip_reader_init_mem(&folderarchive,folder,sizeof(folder),0);
                    if (!status)
                    {
                        mz_zip_reader_end(&folderarchive);
                        mz_free(folder);
                        continue;
                    }
                    else {
                       
                        ExtractFiles(folderarchive, m_files);
                    }
                }
             /*   
            }
            else {
            }*/
            //mz_zip_archive zip_archivefolder;
           // memset(&zip_archivefolder, 0, sizeof(zip_archivefolder));
            
        /*    auto status = mz_zip_writer_add_from_zip_reader(&zip_archivefolder, &m_ziparchive,i);
            if (!status)
            {
                mz_zip_reader_end(&m_ziparchive);
                continue;
            }
            ExtractFiles(zip_archivefolder,m_files);*/
            
        }
        std::string destFile = outDir + file_stat.m_filename;
        if (mz_zip_reader_extract_to_file(&m_ziparchive, i, destFile.c_str(), 0))
        {
            m_files.emplace_back(destFile);
        }
        else
        {  
            void* ImageFile = mz_zip_reader_extract_to_heap(&m_ziparchive,i,&file_stat.m_uncomp_size,0);
           // TArray<uint16> ImageBuffer = reinterpret_cast<TArray<uint16>>(ImageFile);
            
        }
    }

    // Close the archive, freeing any resources it was using
    mz_zip_reader_end(&m_ziparchive);
}