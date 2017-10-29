#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include "windows.h"
#include <stdint.h>
#include <assert.h>
#include <rwcore.h>
#include <rpworld.h>
typedef uintptr_t uintptr;
#include "MemoryMgr.h"
#include "debugmenu_public.h"

typedef uint8_t uint8, uchar;
typedef uint16_t uint16, ushort;
typedef uint32_t uint32, uint;
typedef uint64_t uint64;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

#define RAD2DEG(x) (float)(180.0f*(x)/M_PI)
#define DEG2RAD(x) (float)((x)/180.0f*M_PI)

#define nil NULL

#define nelem(a) (sizeof(a)/sizeof(a[0]))

struct GlobalScene
{
	RpWorld *world;
	RwCamera *camera;
};
extern GlobalScene &Scene;

struct RsInputDevice
{
	int inputDeviceType;
	int used;
	void *inputEventHandler;
};

struct RsGlobalType
{
	const char *appName;
	int width;
	int height;
	int maximumWidth;
	int maximumHeight;
	int maxFPS;
	int quit;
	void *ps;
	RsInputDevice keyboard;
	RsInputDevice mouse;
	RsInputDevice pad;
};
extern RsGlobalType &RsGlobal;

#include "common.h"
#include "game.h"
