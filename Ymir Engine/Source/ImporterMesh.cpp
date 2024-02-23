#include "ImporterMesh.h"
#include "PhysfsEncapsule.h"

void ImporterMesh::Import(const aiMesh* mesh, Mesh* ourMesh)
{

}

const char* ImporterMesh::Save(const Mesh* ourMesh, uint& retSize)
{
    //// Define the size of the header (ranges) and data (vertices)
    //uint headerSize = sizeof(uint) * 2; // num_indices and num_vertices
    //uint vertexSize = sizeof(float) * 3 + sizeof(float) * 3 + sizeof(float) * 2; // size of Vertex structure

    //// Calculate the total size of the file buffer
    //uint ranges[2] = { ourMesh->indices.size(), ourMesh->vertices.size() };
    ////uint size = headerSize + sizeof(uint) * ourMesh->indices.size() + vertexSize * ourMesh->vertices.size();
    //uint64 size = sizeof(ranges) + sizeof(uint) * ourMesh->indices.size() + (sizeof(uint) * 3) * ourMesh->vertices.size();

    //// Allocate memory for the file buffer
    //*fileBuffer = new char[size];

    //// Create a cursor to keep track of the current position in the buffer
    //char* cursor = *fileBuffer;

    //// Store the header (ranges)
    //uint bytes = sizeof(ranges);
    //memcpy(cursor, ranges, bytes);
    //cursor += bytes;

    //// Store the indices
    //bytes = sizeof(uint) * ourMesh->indices.size();
    //memcpy(cursor, ourMesh->indices.data(), bytes);
    //cursor += bytes;

    //// Store the vertices
    //bytes = sizeof(float) * 3 * ourMesh->vertices.size();
    //memcpy(cursor, ourMesh->vertices.data(), bytes);
    //cursor += bytes;

    //// Return the size of the file buffer
    //cursor = nullptr;
    //return size;

    uint aCounts[2] = { ourMesh->indices.size(), ourMesh->vertices.size() };

    retSize = sizeof(aCounts) + (sizeof(uint) * aCounts[0]) + (sizeof(Vertex) * aCounts[1]);

    char* fileBuffer = new char[retSize];
    char* cursor = fileBuffer;

    uint bytes = sizeof(aCounts);
    memcpy(cursor, aCounts, bytes);
    cursor += bytes;

    bytes = sizeof(uint) * aCounts[0];
    memcpy(cursor, ourMesh->indices.data(), bytes);
    cursor += bytes;

    bytes = sizeof(Vertex) * aCounts[1];
    memcpy(cursor, ourMesh->vertices.data(), bytes);
    cursor += bytes;

    return fileBuffer;
}

void ImporterMesh::Load(const char* path, ResourceMesh* ourMesh)
{
    //char* cursor = const_cast<char*>(fileBuffer);

    //// Load the header (ranges)
    //uint ranges[2];

    //uint bytes = sizeof(ranges);
    //memcpy(ranges, cursor, bytes);
    //cursor += bytes;

    //// Resize indices and vertices
    //ourMesh->indices.resize(ranges[0]);
    //ourMesh->vertices.resize(ranges[1]);

    //// Load indices
    //bytes = sizeof(uint) * ourMesh->indices.size();
    ////ourMesh->indices.push_back(new uint[ourMesh->indices.size()]);
    //memcpy(ourMesh->indices.data(), cursor, bytes);
    //cursor += bytes;

    //// Load vertices
    //bytes = sizeof(float) * 3 * ourMesh->vertices.size() + sizeof(float) * 3 * ourMesh->vertices.size() + sizeof(float) * 2 * ourMesh->vertices.size();
    //memcpy(ourMesh->vertices.data(), cursor, bytes);
    //cursor += bytes;

    char* fileBuffer = nullptr;

    uint size = PhysfsEncapsule::LoadFileToBuffer(path, &fileBuffer);

    if (size == 0)
        return;

    char* cursor = fileBuffer;
    uint variables[2];

    uint bytes = sizeof(variables);
    memcpy(variables, cursor, bytes);

    ourMesh->indices.resize(variables[0]);
    ourMesh->vertices.resize(variables[1]);

    cursor += bytes;

    bytes = sizeof(uint) * ourMesh->indices.size();
    memcpy(ourMesh->indices.data(), cursor, bytes);
    cursor += bytes;

    bytes = sizeof(Vertex) * ourMesh->vertices.size();
    memcpy(ourMesh->vertices.data(), cursor, bytes);
    cursor += bytes;

    delete[] fileBuffer;
    fileBuffer = nullptr;

}

void ImporterMesh::Load(const char* fileBuffer, Mesh* ourMesh)
{
    char* cursor = const_cast<char*>(fileBuffer);

    // Load the header (ranges)
    uint ranges[2];
    uint bytes = sizeof(ranges);
    memcpy(ranges, cursor, bytes);
    cursor += bytes;

    // Resize indices and vertices
    ourMesh->indices.resize(ranges[0]);
    ourMesh->vertices.resize(ranges[1]);

    // Load indices
    bytes = sizeof(uint) * ourMesh->indices.size();
    memcpy(ourMesh->indices.data(), cursor, bytes);
    cursor += bytes;

    // Load vertices
    bytes = sizeof(float) * 3 * ourMesh->vertices.size() + sizeof(float) * 3 * ourMesh->vertices.size() + sizeof(float) * 2 * ourMesh->vertices.size();
    memcpy(ourMesh->vertices.data(), cursor, bytes);
    cursor += bytes;

    ourMesh->LoadMesh();

}
