#ifndef __ANIMATIONCONTROLLER__
#define __ANIMATIONCONTROLLER__

#include <vector>
#include "Globals.h"

class AnimationController {
public:
	AnimationController();

	~AnimationController();

	//Animation configuration management
	void AddAnimation();

	void AddAnimation(uint64 animationID);

public:
	std::vector<uint64> animations;
};

#endif // __ANIMATIONCONTROLLER__
