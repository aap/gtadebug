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

void patchDebugCam(void);

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

enum RsKeyCodes
{
	rsESC = 0x3E8,
	rsF1 = 0x3E9,
	rsF2 = 0x3EA,
	rsF3 = 0x3EB,
	rsF4 = 0x3EC,
	rsF5 = 0x3ED,
	rsF6 = 0x3EE,
	rsF7 = 0x3EF,
	rsF8 = 0x3F0,
	rsF9 = 0x3F1,
	rsF10 = 0x3F2,
	rsF11 = 0x3F3,
	rsF12 = 0x3F4,
	rsINSERT = 0x3F5,
	rsDEL = 0x3F6,
	rsHOME = 0x3F7,
	rsEND = 0x3F8,
	rsPGUP = 0x3F9,
	rsPGDN = 0x3FA,
	rsUP = 0x3FB,
	rsDOWN = 0x3FC,
	rsLEFT = 0x3FD,
	rsRIGHT = 0x3FE,
	rsDIV = 0x3FF,
	rsMUL = 0x400,
	rsADD = 0x401,
	rsSUB = 0x402,
	rsEXTDEL = 0x403,
	rsEXT_END = 0x404,
	rsEXTDOWN = 0x405,
	rsEXTPGDN = 0x406,
	rsEXTLEFT = 0x407,
	rsNUM5 = 0x408,
	rsNUMLOCK = 0x409,
	rsEXTRIGHT = 0x40A,
	rsEXTHOME = 0x40B,
	rsEXTUP = 0x40C,
	rsEXTPGUP = 0x40D,
	rsEXTINSERT = 0x40E,
	rsENTER = 0x40F,
	rsSCROLL = 0x410,
	rsPAUSE = 0x411,
	rsBACK = 0x412,
	rsTAB = 0x413,
	rsCAPSLOCK = 0x414,
	rsEXTENTER = 0x415,
	rsLSHIFT = 0x416,
	rsRSHIFT = 0x417,
	rsSHIFT = 0x418,
	rsLCTRL = 0x419,
	rsRCTRL = 0x41A,
	rsLMENU = 0x41B,
	rsRMENU = 0x41C,
	rsLWIN = 0x41D,
	rsRWIN = 0x41E,
	rsAPPS = 0x41F,
	rsNULL = 0x420,
};

#include "common.h"
#include "game.h"

struct CPostEffects
{
	static int &m_InfraredVisionGrainStrength;
	static int &m_NightVisionGrainStrength;
	static float &m_fNightVisionSwitchOnFXCount;
	static bool &m_bInfraredVision;

	static bool &m_bDisableAllPostEffect;

	static bool &m_bColorEnable;
	static int &m_colourLeftUOffset;
	static int &m_colourRightUOffset;
	static int &m_colourTopVOffset;
	static int &m_colourBottomVOffset;
	static float &m_colour1Multiplier;
	static float &m_colour2Multiplier;
	static float &SCREEN_EXTRA_MULT_CHANGE_RATE;
	static float &SCREEN_EXTRA_MULT_BASE_CAP;
	static float &SCREEN_EXTRA_MULT_BASE_MULT;

	static bool &m_bRadiosity;
	static bool &m_bRadiosityDebug;
	static int &m_RadiosityFilterPasses;
	static int &m_RadiosityRenderPasses;
	static int &m_RadiosityIntensityLimit;
	static int &m_RadiosityIntensity;
	static bool &m_bRadiosityBypassTimeCycleIntensityLimit;
	static int &m_RadiosityFilterUCorrection;
	static int &m_RadiosityFilterVCorrection;

	static bool &m_bDarknessFilter;
	static int &m_DarknessFilterAlpha;
	static int &m_DarknessFilterAlphaDefault;
	static int &m_DarknessFilterRadiosityIntensityLimit;

	static bool &m_bCCTV;
	static bool &m_bFog;
	static bool &m_bNightVision;
	static bool &m_bHeatHazeFX;
	static bool &m_bHeatHazeMaskModeTest;
	static bool &m_bGrainEnable;
	static bool &m_waterEnable;

	static bool &m_bSpeedFX;
	static bool &m_bSpeedFXTestMode;
	static uint8 &m_SpeedFXAlpha;
};
