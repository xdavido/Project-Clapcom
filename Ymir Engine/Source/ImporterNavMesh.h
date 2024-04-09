#pragma once

class dtNavMesh;
typedef unsigned int dtTileRef;

#include "RecastNavigation/Detour/DetourNavMesh.h"
#include "RecastNavigation/InputGeom.h"
#include "Globals.h"

static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;

struct NavMeshSetHeader
{
	int magic;
	int version;
	int numTiles;
	dtNavMeshParams params;
};

struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};

struct GeometryData
{
	GeometryData();
	GeometryData(InputGeom* geometry);

	float bMin[3];
	float bMax[3];
};

namespace ImporterNavMesh
{
	uint Save(const char* assets_path, dtNavMesh* navMesh, BuildSettings& buildSettings, uint UID);
	dtNavMesh* Load(uint navMeshResourceUID, BuildSettings& buildSettings);
}