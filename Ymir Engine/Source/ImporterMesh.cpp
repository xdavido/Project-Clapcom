#include "ImporterMesh.h"
#include "PhysfsEncapsule.h"

#include "External/mmgr/mmgr.h"

void ImporterMesh::Import(const aiMesh* mesh, Mesh* ourMesh)
{

}

// Function to save mesh data into a file buffer
const char* ImporterMesh::Save(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, uint& retSize)
{
    // Array to store counts of indices and vertices
    uint aCounts[2] = { indices.size(), vertices.size() };

    // Calculate total size required for file buffer
    retSize = sizeof(aCounts) + (sizeof(uint) * aCounts[0]) + (sizeof(Vertex) * aCounts[1]);

    // Allocate memory for file buffer
    char* fileBuffer = new char[retSize];
    // Cursor to track writing position in the file buffer
    char* cursor = fileBuffer;

    // Copy counts of indices and vertices to the file buffer
    uint bytes = sizeof(aCounts);
    memcpy(cursor, aCounts, bytes);
    cursor += bytes;

    // Copy indices data to the file buffer
    bytes = sizeof(uint) * aCounts[0];
    memcpy(cursor, indices.data(), bytes);
    cursor += bytes;

    // Copy vertices data to the file buffer
    bytes = sizeof(Vertex) * aCounts[1];
    memcpy(cursor, vertices.data(), bytes);
    cursor += bytes;

    // Return the file buffer
    return fileBuffer;
}

// Function to load mesh data from a file buffer
void ImporterMesh::Load(const char* path, ResourceMesh* ourMesh)
{
    // Pointer to hold file buffer data
    char* fileBuffer = nullptr;

    // Load file contents into file buffer and get its size
    uint size = PhysfsEncapsule::LoadFileToBuffer(path, &fileBuffer);

    // If file is empty, return
    if (size == 0)
        return;

    // Cursor to track reading position in the file buffer
    char* cursor = fileBuffer;
    // Array to store counts of indices and vertices
    uint variables[2];

    // Read counts of indices and vertices from the file buffer
    uint bytes = sizeof(variables);
    memcpy(variables, cursor, bytes);

    // Resize indices and vertices vectors in ourMesh
    ourMesh->indices.resize(variables[0]);
    ourMesh->vertices.resize(variables[1]);

    // Move cursor to next position after counts data
    cursor += bytes;

    // Read indices data from the file buffer and copy it to ourMesh
    bytes = sizeof(uint) * ourMesh->indices.size();
    memcpy(ourMesh->indices.data(), cursor, bytes);
    cursor += bytes;

    // Read vertices data from the file buffer and copy it to ourMesh
    bytes = sizeof(Vertex) * ourMesh->vertices.size();
    memcpy(ourMesh->vertices.data(), cursor, bytes);
    cursor += bytes;

    // Deallocate memory for file buffer
    delete[] fileBuffer;
    fileBuffer = nullptr;
}