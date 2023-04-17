/* Copyright (C) Itseez3D, Inc. - All Rights Reserved
 * You may not use this file except in compliance with an authorized license
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * UNLESS REQUIRED BY APPLICABLE LAW OR AGREED BY ITSEEZ3D, INC. IN WRITING, SOFTWARE DISTRIBUTED UNDER THE LICENSE IS DISTRIBUTED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED
 * See the License for the specific language governing permissions and limitations under the License.
 * Written by Itseez3D, Inc. <support@itseez3D.com>, May 2020
 */


#include <Helpers/PlyUtils.hpp>
#include <fstream>
#include <iostream>
#include <numeric>

using namespace tinyply;

inline std::vector<uint8_t> readFileBinary(const std::string& pathToFile)
{
    std::ifstream file(pathToFile, std::ios::binary);
    std::vector<uint8_t> fileBufferBytes;

    if (file.is_open())
    {
        file.seekg(0, std::ios::end);
        size_t sizeBytes = file.tellg();
        file.seekg(0, std::ios::beg);
        fileBufferBytes.resize(sizeBytes);
        if (file.read((char*)fileBufferBytes.data(), sizeBytes)) return fileBufferBytes;
    }
    else throw std::runtime_error("could not open binary ifstream to path " + pathToFile);
    return fileBufferBytes;
}

struct MemoryBuffer : public std::streambuf
{
    char* p_start{ nullptr };
    char* p_end{ nullptr };
    size_t size;

    MemoryBuffer(char const* first_elem, size_t size)
        : p_start(const_cast<char*>(first_elem)), p_end(p_start + size), size(size)
    {
        setg(p_start, p_start, p_end);
    }

    pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which) override
    {
        if (dir == std::ios_base::cur) gbump(static_cast<int>(off));
        else setg(p_start, (dir == std::ios_base::beg ? p_start : p_end) + off, p_end);
        return gptr() - p_start;
    }

    pos_type seekpos(pos_type pos, std::ios_base::openmode which) override
    {
        return seekoff(pos, std::ios_base::beg, which);
    }
};

struct MemoryStream : virtual MemoryBuffer, public std::istream
{
    MemoryStream(char const* first_elem, size_t size)
        : MemoryBuffer(first_elem, size), std::istream(static_cast<std::streambuf*>(this)) {}
};




void RuntimeImportPlyUtils::readPly(const FString& plyFilepath, bool preload,
                                std::vector<vert3>& vertsResult,
                                std::vector<faceUv>& uvResult,
                                std::vector<faceVerIndex3>& facesResult,
                                std::vector<int>& duplicatesMapResult)
{
    std::unique_ptr<std::istream> fileStream;
    std::vector<uint8_t> byteBuffer;
    std::string filepath = std::string(TCHAR_TO_UTF8(*plyFilepath));
    try
    {
        // For most files < 1gb, pre-loading the entire file upfront and wrapping it into a 
        // stream is a net win for parsing speed, about 40% faster. 
        if (preload)
        {
            byteBuffer = readFileBinary(filepath);
            fileStream.reset(new MemoryStream((char*)byteBuffer.data(), byteBuffer.size()));
        }
        else
        {
            fileStream.reset(new std::ifstream(filepath, std::ios::binary));
        }

        if (!fileStream || fileStream->fail()) throw std::runtime_error("file_stream failed to open " + filepath);

        fileStream->seekg(0, std::ios::beg);

        PlyFile file;
        file.parse_header(*fileStream);
        std::shared_ptr<PlyData> plyTexcoords, plyFaces, plyVertices;
        try { plyVertices = file.request_properties_from_element("vertex", { "x", "y", "z" }, 0); }
        catch (const std::exception& e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }
        try { plyTexcoords = file.request_properties_from_element("face", { "texcoord" }, 0); }
        catch (const std::exception& e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }
        try { plyFaces = file.request_properties_from_element("face", { "vertex_indices" }, 0); }
        catch (const std::exception& e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

        file.read(*fileStream);

        std::vector<vert3> vertices;
        //read vertices
        if(plyVertices)
        { 
            const size_t verticesNum = plyVertices->buffer.size_bytes();
            vertices.resize (plyVertices->count);
            std::memcpy(vertices.data(), plyVertices->buffer.get(), verticesNum);
        }

        std::vector<faceUv3> faceUvCoords;
        //read face uv coordinates
        if (plyTexcoords)
        {
            const size_t faceTextCoordsNum = plyTexcoords->buffer.size_bytes();
            faceUvCoords.resize(plyTexcoords->count);
            std::memcpy(faceUvCoords.data(), plyTexcoords->buffer.get(), faceTextCoordsNum);
        }
        

        std::vector<faceVerIndex3> faces;
        //read face indices
        if (plyFaces)
        {
            const size_t faceIndicesNum = plyFaces->buffer.size_bytes();
            faces.resize(plyFaces->count);
            std::memcpy(faces.data(), plyFaces->buffer.get(), faceIndicesNum);
        }

        std::vector<vert3> finalVerts;
        finalVerts.resize(faces.size() + vertices.size());
        
        std::vector<faceUv> finalUv;
        finalUv.resize(faces.size() + vertices.size());

        std::vector<faceVerIndex3> finalFaces;
        finalFaces.resize(faces.size());
        std::copy(faces.begin(), faces.end(), finalFaces.begin());

        std::vector<int> duplicates;
        duplicates.resize(faces.size() + vertices.size());
        std::vector<int> duplicatesMap;


        const vert3 NOT_INIT_VERT3 = { -1.0f, -1.0f, -1.0f };
        const faceUv NOT_INIT_UV = { -1.0, -1.0 };
        std::fill(finalVerts.begin(), finalVerts.end(), NOT_INIT_VERT3);
        std::fill(duplicates.begin(), duplicates.end(), -1);
        std::fill(finalUv.begin(), finalUv.end(), NOT_INIT_UV);

        if (faces.size())
        {
            int newVertexIdx = vertices.size();
            for (int i = 0; i < faces.size(); i++)
            {
                const faceVerIndex3& vert = faces[i];
                for (int j = 0; j < 3; j++)
                {
                    int vindex = vert.v[j];
                    faceUv uv = faceUvCoords[i].uv[j];
                    while (finalUv[vindex] != NOT_INIT_UV && finalUv[vindex] != uv)
                    {
                        if (duplicates[vindex] == -1)
                        {
                            duplicates[vindex] = newVertexIdx++;
                        }
                        vindex = duplicates[vindex];
                    }
                    finalUv[vindex] = uv;
                    finalVerts[vindex] = vertices[vert.v[j]];
                    finalFaces[i].v[j] = vindex;
                }

            }
            finalVerts.resize(newVertexIdx);
            finalUv.resize(newVertexIdx);
            duplicatesMap.resize(newVertexIdx);
            std::iota(duplicatesMap.begin(), duplicatesMap.begin() + vertices.size(), 0);
            std::fill(duplicatesMap.begin()+vertices.size(), duplicatesMap.end(), -1);
            for (int i = 0; i < vertices.size(); i++)
            {
                int duplicateIdx = duplicates[i];
                while (duplicateIdx != -1)
                {
                    duplicatesMap[duplicateIdx] = i;
                    duplicateIdx = duplicates[duplicateIdx];
                }
            }

        } //if have faces
        else 
        {
            std::copy(vertices.begin(), vertices.end(), finalVerts.begin());
        }
        std::swap(finalVerts, vertsResult);
        std::swap(finalUv, uvResult);
        std::swap(finalFaces, facesResult);
        std::swap(duplicatesMapResult, duplicatesMap);

    }
    catch (const std::exception& e)
    {
        std::cerr << "Caught tinyply exception: " << e.what() << std::endl;
    }
}

bool RuntimeImportPlyUtils::operator==(const vert3& v1, const vert3& v2)
{
    return  v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

bool RuntimeImportPlyUtils::operator!=(const vert3& v1, const vert3& v2)
{
    return !(v1 == v2);
}

bool RuntimeImportPlyUtils::operator==(const faceUv& uv1, const faceUv& uv2)
{
    return uv1.u == uv2.u && uv1.v == uv2.v;
}

bool RuntimeImportPlyUtils::operator!=(const faceUv& uv1, const faceUv& uv2)
{
    return !(uv1 == uv2);
}
