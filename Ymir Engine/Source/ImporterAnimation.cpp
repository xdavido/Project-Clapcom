#include "ImporterAnimation.h"
#include "Bone.h"
#include "PhysfsEncapsule.h"

void ImporterAnimation::Import(const std::string& animationPath, ResourceAnimation* ourAnimation)
{

}

const char* ImporterAnimation::Save(const Animation* ourAnimation, uint& retSize)
{
	std::string boneNames = ""; //Bone names stored together

	retSize = sizeof(float) + sizeof(float) + sizeof(uint) * 4; // Duration + ticksPerSecond + bonesSize + boneNamesSize + AnimationNameSize + BoneInfoMapSize (headers size)

	for (int i = 0; i < ourAnimation->bones.size(); i++) {
		boneNames += ourAnimation->bones[i].name;
		boneNames += "\n";

		retSize += ourAnimation->bones[i].name.size() * sizeof(uint) * 4;
		retSize += ourAnimation->bones[i].positions.size() * (sizeof(float) + sizeof(float3)); // Save the float TimeStamp and the position vector
		retSize += ourAnimation->bones[i].rotations.size() * (sizeof(float) + sizeof(Quat)); // Save the float TimeStamp and the position quaternion
		retSize += ourAnimation->bones[i].scales.size() * (sizeof(float) + sizeof(float3));
	}
	retSize += ourAnimation->boneInfoMap.size() * sizeof(BoneInfo) + ourAnimation->boneInfoMap.size() * sizeof(std::string);
	
	uint header[6] = { ourAnimation->duration, ourAnimation->ticksPerSecond, ourAnimation->bones.size(), boneNames.size(), ourAnimation->name.size(), ourAnimation->boneInfoMap.size()};

	retSize += sizeof(header);
	CalculateNodeSize(ourAnimation->rootNode, retSize);

	// BoneInfoMap	
	

	char* fileBuffer = new char[retSize];
	char* cursor = fileBuffer;

	// Save header 
	uint headerSize = sizeof(header);
	memcpy(cursor, header, headerSize);
	cursor += headerSize;

	// Save boneNamesString
	memcpy(cursor, &boneNames[0], boneNames.size());
	cursor += boneNames.size();

	// Bones
	for (int i = 0; i < ourAnimation->bones.size(); i++) {
		uint boneHeader[3] = { ourAnimation->bones[i].positions.size(), ourAnimation->bones[i].rotations.size(), ourAnimation->bones[i].scales.size() };
		uint boneHeaderSize = sizeof(uint) * 3;
		memcpy(cursor, boneHeader, boneHeaderSize);
		cursor += boneHeaderSize;

		// Vector values
		// Position
		std::vector<float> posKey;
		std::vector<float3> posVal;
		posKey.reserve(ourAnimation->bones[i].positions.size());
		posVal.reserve(ourAnimation->bones[i].positions.size());
		for (std::vector<KeyPosition>::const_iterator it = ourAnimation->bones[i].positions.begin(); it != ourAnimation->bones[i].positions.end(); it++) {
			posKey.push_back(it->timeStamp);
			posVal.push_back(it->position);
		}
		memcpy(cursor, &posKey[0], ourAnimation->bones[i].positions.size() * sizeof(float));
		cursor += ourAnimation->bones[i].positions.size() * sizeof(float);
		memcpy(cursor, &posVal[0], ourAnimation->bones[i].positions.size() * sizeof(float3));
		cursor += ourAnimation->bones[i].positions.size() * sizeof(float3);

		// Rotation
		std::vector<float> rotKey;
		std::vector<Quat> rotVal;
		posKey.reserve(ourAnimation->bones[i].rotations.size());
		posVal.reserve(ourAnimation->bones[i].rotations.size());
		for (std::vector<KeyRotation>::const_iterator it = ourAnimation->bones[i].rotations.begin(); it != ourAnimation->bones[i].rotations.end(); it++) {
			rotKey.push_back(it->timeStamp);
			rotVal.push_back(it->rotation);
		}
		memcpy(cursor, &rotKey[0], ourAnimation->bones[i].rotations.size() * sizeof(float));
		cursor += ourAnimation->bones[i].rotations.size() * sizeof(float);
		memcpy(cursor, &rotVal[0], ourAnimation->bones[i].rotations.size() * sizeof(Quat));
		cursor += ourAnimation->bones[i].rotations.size() * sizeof(Quat);

		// Scales
		std::vector<float> scaKey;
		std::vector<float3> scaVal;
		scaKey.reserve(ourAnimation->bones[i].scales.size());
		scaVal.reserve(ourAnimation->bones[i].scales.size());
		for (std::vector<KeyScale>::const_iterator it = ourAnimation->bones[i].scales.begin(); it != ourAnimation->bones[i].scales.end(); it++) {
			scaKey.push_back(it->timeStamp);
			scaVal.push_back(it->scale);
		}
		memcpy(cursor, &scaKey[0], ourAnimation->bones[i].scales.size() * sizeof(float));
		cursor += ourAnimation->bones[i].scales.size() * sizeof(float);
		memcpy(cursor, &scaVal[0], ourAnimation->bones[i].scales.size() * sizeof(float3));
		cursor += ourAnimation->bones[i].scales.size() * sizeof(float3);
	}

	// Save animation name
	memcpy(cursor, ourAnimation->name.c_str(), ourAnimation->name.size() + 1);
	cursor += ourAnimation->name.size() + 1;

	// Save BoneInfoMap

	for (const auto& pair : ourAnimation->boneInfoMap) {
		const std::string& name = pair.first;
		const BoneInfo& info = pair.second;

		uint nameSize = name.size();
		memcpy(cursor, &nameSize, sizeof(uint));
		cursor += sizeof(uint);
		memcpy(cursor, name.c_str(), nameSize);
		cursor += nameSize;

		memcpy(cursor, &info, sizeof(BoneInfo));
		cursor += sizeof(BoneInfo);
	}

	// Save assimp imported nodes
	SaveAssimpNode(ourAnimation->rootNode, cursor);

	return fileBuffer;
}

void ImporterAnimation::Load(const char* path, ResourceAnimation* ourAnimation)
{
	std::string boneNames; 

	char* fileBuffer = nullptr;

	// Load file contents into file buffer and get its size
	uint size = PhysfsEncapsule::LoadFileToBuffer(path, &fileBuffer);

	// If file is empty, return
	if (size == 0)
		return;

	// Cursor to track reading position in the file buffer
	char* cursor = fileBuffer;

	// Load headers
	uint header[6];
	uint headerSize = sizeof(uint) * 6;
	memcpy(header, cursor, headerSize);
	cursor += headerSize;

	ourAnimation->duration = header[0];
	ourAnimation->ticksPerSecond = header[1];

	// Load bone name string
	uint boneNamesSize = header[3] * sizeof(char);
	boneNames.resize(header[3]);
	memcpy(&boneNames[0], cursor, boneNamesSize);
	cursor += boneNamesSize;

	// Load numBones
	ourAnimation->bones.reserve(header[2]);

	// Create bones with name
	std::stringstream ss(boneNames);
	std::string tmp; 

	while (std::getline(ss, tmp, '\n')) {
		Bone aux; 
		aux.name = tmp; 
		ourAnimation->bones.push_back(aux);
	}

	// Read all bones

	for (uint i = 0; i < header[2]; i++) {
		
		//Map sizes
		uint boneHeader[3];
		memcpy(boneHeader, cursor, sizeof(uint) * 3);
		cursor += sizeof(uint) * 3;

		// Position
		std::vector<float> posKey;
		std::vector<float3> posVal;
		posKey.resize(boneHeader[0]);
		posVal.resize(boneHeader[0]);
		memcpy(&posKey[0], cursor, boneHeader[0] * sizeof(float));
		cursor += boneHeader[0] * sizeof(float);
		memcpy(&posVal[0], cursor, boneHeader[0] * sizeof(float3));
		cursor += boneHeader[0] * sizeof(float3);

		// Rotation
		std::vector<float> rotKey;
		std::vector<Quat> rotVal;
		rotKey.resize(boneHeader[1]);
		rotVal.resize(boneHeader[1]);
		memcpy(&rotKey[0], cursor, boneHeader[1] * sizeof(float));
		cursor += boneHeader[1] * sizeof(float);
		memcpy(&rotVal[0], cursor, boneHeader[1] * sizeof(Quat));
		cursor += boneHeader[1] * sizeof(Quat);

		// Scale
		std::vector<float> scaKey;
		std::vector<float3> scaVal;
		scaKey.resize(boneHeader[2]);
		scaVal.resize(boneHeader[2]);
		memcpy(&scaKey[0], cursor, boneHeader[2] * sizeof(float));
		cursor += boneHeader[2] * sizeof(float);
		memcpy(&scaVal[0], cursor, boneHeader[2] * sizeof(float3));
		cursor += boneHeader[2] * sizeof(float3);

		// Build vectors
		ourAnimation->bones[i].positions.resize(boneHeader[0]);
		ourAnimation->bones[i].numPositions = boneHeader[0];
		// Position
		for (int j = 0; j < posKey.size(); j++) {
			
			ourAnimation->bones[i].positions[j].timeStamp = posKey[j];
			ourAnimation->bones[i].positions[j].position = posVal[j];
		}

		// Rotation
		ourAnimation->bones[i].rotations.resize(boneHeader[1]);
		ourAnimation->bones[i].numRotations = boneHeader[1];
		for (int j = 0; j < rotKey.size(); j++) {
			ourAnimation->bones[i].rotations[j].timeStamp = rotKey[j];
			ourAnimation->bones[i].rotations[j].rotation = rotVal[j];
		}

		// Scale
		ourAnimation->bones[i].scales.resize(boneHeader[2]);
		ourAnimation->bones[i].numScales = boneHeader[2];
		for (int j = 0; j < scaKey.size(); j++) {
			ourAnimation->bones[i].scales[j].timeStamp = scaKey[j];
			ourAnimation->bones[i].scales[j].scale = scaVal[j];
		}
	}

	// Load animation name. The + 1 in animation name size is due to the '\0'
	uint animationNameSize = header[4] * sizeof(char) + 1;
	char* nameBuffer = new char[animationNameSize];
	memcpy(nameBuffer, cursor, animationNameSize);
	ourAnimation->name = std::string(nameBuffer);
	delete[] nameBuffer;
	cursor += animationNameSize;

	// Load boneInfoMap

	for (int i = 0; i < header[5]; ++i) {
		uint nameSize;
		memcpy(&nameSize, cursor, sizeof(uint));
		cursor += sizeof(uint);

		std::string name(cursor, nameSize);
		cursor += nameSize;

		BoneInfo info;
		memcpy(&info, cursor, sizeof(BoneInfo));
		cursor += sizeof(BoneInfo);

		ourAnimation->boneInfoMap[name] = info;
	}

	// Assign id
	for (uint i = 0; i < header[2]; i++) {
		ourAnimation->bones[i].id = ourAnimation->boneInfoMap[ourAnimation->bones[i].name].id;
	}

	// Load AssimpNodeData
	ourAnimation->rootNode = LoadNode(cursor);
	// Deallocate memory for file buffer
	delete[] fileBuffer;
	fileBuffer = nullptr;
}

void ImporterAnimation::SaveAssimpNode(const AssimpNodeData& node, char*& cursor) {
	// Save matrix transform
	memcpy(cursor, &node.transformation, sizeof(float4x4));
	cursor += sizeof(float4x4);

	// Save name
	uint nameSize = node.name.size();
	memcpy(cursor, &nameSize, sizeof(uint));
	cursor += sizeof(uint);
	memcpy(cursor, node.name.c_str(), nameSize);
	cursor += nameSize; 

	// Save child number
	memcpy(cursor, &node.childrenCount, sizeof(int));
	cursor += sizeof(int);

	for (int i = 0; i < node.childrenCount; i++) {
		SaveAssimpNode(node.children[i], cursor);
	}
}

AssimpNodeData ImporterAnimation::LoadNode(char*& cursor)
{
	AssimpNodeData node; 

	// Load transform
	memcpy(&node.transformation, cursor, sizeof(float4x4));
	cursor += sizeof(float4x4);

	// Load name
	uint nameSize;
	memcpy(&nameSize, cursor, sizeof(uint));
	cursor += sizeof(uint);
	node.name.resize(nameSize);
	memcpy(&node.name[0], cursor, nameSize);
	cursor += nameSize;

	// Load children
	memcpy(&node.childrenCount, cursor, sizeof(int));
	cursor += sizeof(int);

	// Load children vector
	for (int i = 0; i < node.childrenCount; i++) {
		AssimpNodeData child = LoadNode(cursor);
		node.children.push_back(child);
	}
	return node;
}

void ImporterAnimation::CalculateNodeSize(const AssimpNodeData& node, uint& size)
{
	size += sizeof(float4x4); 

	size += sizeof(uint) + node.name.size() + 1; 

	size += sizeof(int);

	for (int i = 0; i < node.childrenCount; i++) {
		CalculateNodeSize(node.children[i], size);
	}
}
