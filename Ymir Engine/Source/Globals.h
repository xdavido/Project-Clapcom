#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>
#include <vector>
#include <map>

#include "External/Glew/include/glew.h"
#include "External/SDL/include/SDL_opengl.h"
#include <gl/GL.h>

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define MIN(a,b) ((a)<(b)) ? (a) : (b)
#define MAX(a,b) ((a)>(b)) ? (a) : (b)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define PI 3.14159265358979323846264338327950288

#define MIN(a,b) ((a)<(b)) ? (a) : (b)
#define MAX(a,b) ((a)>(b)) ? (a) : (b)

typedef unsigned int uint;
typedef unsigned __int64 uint64;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------

#ifdef _STANDALONE
#define SCREEN_WIDTH 2560
#define SCREEN_HEIGHT 1440
#else
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024
#endif // _STANDALONE

#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_MAXIMIZED false
#define WIN_BORDERLESS false

#ifdef _STANDALONE
#define WIN_FULLSCREEN_DESKTOP true
#else
#define WIN_FULLSCREEN_DESKTOP false
#endif // _STANDALONE

#define VSYNC true
#define TITLE "Ymir Engine"

// Deletes a buffer
#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

// Clear a vector
template <class T>
void ClearVec(std::vector<T>& x)
{
	x.clear();
	x.shrink_to_fit();
}

// Clear a vector of pointers
template <class T>
void ClearVecPtr(std::vector<T*>& x)
{
	for (auto it = x.rbegin(); it != x.rend(); ++it)
	{
		// code
		delete(*it);
		(*it) = nullptr;
	}

	x.clear();
	x.shrink_to_fit();
}