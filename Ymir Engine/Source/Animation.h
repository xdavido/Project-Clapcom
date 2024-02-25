#ifndef __ANIMATION__
#define __ANIMATION__

#include <vector>
#include <string>
#include <map>

#include "External/MathGeoLib/include/Math/float4.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float2.h"
#include "External/MathGeoLib/include/Math/float4x4.h"

#include "External/MathGeoLib/include/Math/Quat.h"


#include "Globals.h"

struct Channel {

	std::string name;

	std::map<double, float3> positionKeys;
	std::map<double, Quat> rotationKeys;
	std::map<double, float3> scaleKeys;



};

class Animation {
public:

	Animation();
	~Animation();

private:


public:


private:

	float duration;
	float ticksPerSecond;

	bool isLoopable;
	bool canPingPong;

	std::map<std::string, Channel> channels;

};

#endif // __ANIMATION__
