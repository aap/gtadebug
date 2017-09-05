#include "debug.h"

DebugMenuAPI gDebugMenuAPI;

HMODULE dllModule;
int gtaversion = -1;

#define FIELD(type, var, offset) *(type*)((uint8*)var + offset)

WRAPPER void VehicleCheat(int id) { EAXJMP(0x43A0B0); }

static int (*IsAlreadyRunning)();
int
delayedPatches(void)
{
	if(IsAlreadyRunning())
		return 1;
	if(DebugMenuLoad()){
		static const char *weathers[] = {
			"Extrasunny LA", "Sunny LA", "Extrasunny Smog LA", "Sunny Smog LA", "Cloudy LA",
			"Sunny SF", "Extrasunny SF", "Cloudy SF", "Rainy SF", "Foggy SF",
			"Sunny Vegas", "Extrasunny Vegas", "Cloudy Vegas",
			"Extrasunny Countryside", "Sunny Countryside", "Cloudy Countryside", "Rainy Countryside",
			"Extrasunny Desert", "Sunny Desert", "Sandstorm Desert",
			"Underwater"
		};
		static const char *vehnames[] = {
			"landstal", "bravura", "buffalo", "linerun", "peren", "sentinel", "dumper", "firetruk", "trash",
			"stretch", "manana", "infernus", "voodoo", "pony", "mule", "cheetah", "ambulan", "leviathn", "moonbeam",
			"esperant", "taxi", "washing", "bobcat", "mrwhoop", "bfinject", "hunter", "premier", "enforcer", "securica", "banshee",
			"predator", "bus", "rhino", "barracks", "hotknife", "artict1", "previon", "coach", "cabbie", "stallion", "rumpo", "rcbandit",
			"romero", "packer", "monster", "admiral", "squalo", "seaspar", "pizzaboy", "tram", "artict2", "turismo", "speeder",
			"reefer", "tropic", "flatbed", "yankee", "caddy", "solair", "topfun", "skimmer", "pcj600", "faggio", "freeway",
			"rcbaron", "rcraider", "glendale", "oceanic", "sanchez", "sparrow", "patriot", "quad", "coastg", "dinghy", "hermes",
			"sabre", "rustler", "zr350", "walton", "regina", "comet", "bmx", "burrito", "camper", "marquis", "baggage",
			"dozer", "maverick", "vcnmav", "rancher", "fbiranch", "virgo", "greenwoo", "jetmax", "hotring", "sandking", "blistac",
			"polmav", "boxville", "benson", "mesa", "rcgoblin", "hotrina", "hotrinb", "bloodra", "rnchlure", "supergt", "elegant",
			"journey", "bike", "mtbike", "beagle", "cropdust", "stunt", "petro", "rdtrain", "nebula", "majestic", "buccanee", "shamal",
			"hydra", "fcr900", "nrg500", "copbike", "cement", "towtruck", "fortune", "cadrona", "fbitruck", "willard", "forklift",
			"tractor", "combine", "feltzer", "remingtn", "slamvan", "blade", "freight", "streak", "vortex", "vincent", "bullet", "clover",
			"sadler", "firela", "hustler", "intruder", "primo", "cargobob", "tampa", "sunrise", "merit", "utility", "nevada", "yosemite",
			"windsor", "monstera", "monsterb", "uranus", "jester", "sultan", "stratum", "elegy", "raindanc", "rctiger", "flash", "tahoma",
			"savanna", "bandito", "freiflat", "streakc", "kart", "mower", "duneride", "sweeper", "broadway", "tornado", "at400", "dft30",
			"huntley", "stafford", "bf400", "newsvan", "tug", "petrotr", "emperor", "wayfarer", "euros", "hotdog", "club", "freibox", "artict3",
			"androm", "dodo", "rccam", "launch", "copcarla", "copcarsf", "copcarvg", "copcarru", "picador", "swatvan", "alpha", "phoenix",
			"glenshit", "sadlshit", "bagboxa", "bagboxb", "tugstair", "boxburg", "farmtr1", "utiltr1"
		};
		MenuEntry *e;
		e = DebugMenuAddVar("Time & Weather", "Current Hour", &CClock::ms_nGameClockHours, nil, 1, 0, 23, nil);
		DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddVar("Time & Weather", "Current Minute", &CClock::ms_nGameClockMinutes,
			[](){ CWeather::InterpolationValue = CClock::ms_nGameClockMinutes/60.0f + CClock::ms_nGameClockSeconds / 3600.0f; }, 1, 0, 59, nil);
			DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddVar("Time & Weather", "Old Weather", &CWeather::OldWeatherType, nil, 1, 0, 20, weathers);
		DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddVar("Time & Weather", "New Weather", &CWeather::NewWeatherType, nil, 1, 0, 20, weathers);
		DebugMenuEntrySetWrap(e, true);
		DebugMenuAddVar("Time & Weather", "Time scale", &CTimer::ms_fTimeScale, nil, 0.1f, 0.0f, 10.0f);

		static int vehId = 400;
		DebugMenuAddVar("Spawn", "Vehicle", &vehId, nil, 1, 400, 611, vehnames);
		DebugMenuAddCmd("Spawn", "Spawn", [](){ VehicleCheat(vehId); });
		DebugMenuAddCmd("Spawn", "Spawn Bullet", [](){ VehicleCheat(541); });
		DebugMenuAddCmd("Spawn", "Spawn Hunter", [](){ VehicleCheat(425); });

		DebugMenuAddVarBool8("PostFX", "Disable all PostFX", (int8_t*)&CPostEffects::m_bDisableAllPostEffect, nil);
		DebugMenuAddVarBool8("PostFX", "Colour filter", (int8_t*)&CPostEffects::m_bColorEnable, nil);
		DebugMenuAddVar("PostFX", "Left offset", &CPostEffects::m_colourLeftUOffset, nil, 1, -1600, 1600, nil);
		DebugMenuAddVar("PostFX", "Right offset", &CPostEffects::m_colourRightUOffset, nil, 1, -1600, 1600, nil);
		DebugMenuAddVar("PostFX", "Top offset", &CPostEffects::m_colourTopVOffset, nil, 1, -1600, 1600, nil);
		DebugMenuAddVar("PostFX", "Bottom offset", &CPostEffects::m_colourBottomVOffset, nil, 1, -1600, 1600, nil);
		DebugMenuAddVar("PostFX", "Color1 multiplier", &CPostEffects::m_colour1Multiplier, nil, 0.1f, 0.0f, 20.0f);
		DebugMenuAddVar("PostFX", "Color2 multiplier", &CPostEffects::m_colour2Multiplier, nil, 0.1f, 0.0f, 20.0f);

		DebugMenuAddVar("PostFX", "Screen extra mult change rate", &CPostEffects::SCREEN_EXTRA_MULT_CHANGE_RATE, nil, 0.001f, 0.0f, 1.0f);
		DebugMenuAddVar("PostFX", "Screen extra mult base cap", &CPostEffects::SCREEN_EXTRA_MULT_BASE_CAP, nil, 0.1f, 0.0f, 1.0f);
		DebugMenuAddVar("PostFX", "Screen extra mult base mult", &CPostEffects::SCREEN_EXTRA_MULT_BASE_MULT, nil, 0.1f, 0.0f, 1.0f);

		DebugMenuAddVarBool8("PostFX", "Radiosity", (int8_t*)&CPostEffects::m_bRadiosity, nil);
		DebugMenuAddVar("PostFX", "Radiosity Filter Passes", &CPostEffects::m_RadiosityFilterPasses, nil, 1, 0, 10, nil);
		DebugMenuAddVar("PostFX", "Radiosity Render Passes", &CPostEffects::m_RadiosityRenderPasses, nil, 1, 0, 10, nil);
		DebugMenuAddVar("PostFX", "Radiosity Intensity", &CPostEffects::m_RadiosityIntensity, nil, 1, 0, 255, nil);
		DebugMenuAddVar("PostFX", "Radiosity Intensity Limit", &CPostEffects::m_RadiosityIntensityLimit, nil, 1, 0, 255, nil);
		DebugMenuAddVarBool8("PostFX", "Radiosity Bypass Timecycle Intensity Limit", (int8_t*)&CPostEffects::m_bRadiosityBypassTimeCycleIntensityLimit, nil);
		DebugMenuAddVar("PostFX", "Radiosity Filter U Correction", &CPostEffects::m_RadiosityFilterUCorrection, nil, 1, 0, 255, nil);
		DebugMenuAddVar("PostFX", "Radiosity Filter V Correction", &CPostEffects::m_RadiosityFilterVCorrection, nil, 1, 0, 255, nil);
		DebugMenuAddVarBool8("PostFX", "Radiosity Debug", (int8_t*)&CPostEffects::m_bRadiosityDebug, nil);

		DebugMenuAddVarBool8("PostFX", "Darkness filter", (int8_t*)&CPostEffects::m_bDarknessFilter, nil);
		DebugMenuAddVar("PostFX", "Darkness Filter Alpha", &CPostEffects::m_DarknessFilterAlpha, nil, 1, 0, 255, nil);
		DebugMenuAddVar("PostFX", "Darkness Filter Alpha Default", &CPostEffects::m_DarknessFilterAlphaDefault, nil, 1, 0, 255, nil);
		DebugMenuAddVar("PostFX", "Darkness Filter Radiosity Intensity Limit", &CPostEffects::m_DarknessFilterRadiosityIntensityLimit, nil, 1, 0, 255, nil);
		DebugMenuAddVarBool8("PostFX", "CCTV", (int8_t*)&CPostEffects::m_bCCTV, nil);
		DebugMenuAddVarBool8("PostFX", "Fog", (int8_t*)&CPostEffects::m_bFog, nil);
		DebugMenuAddVarBool8("PostFX", "Night Vision", (int8_t*)&CPostEffects::m_bNightVision, nil);
		DebugMenuAddVarBool8("PostFX", "Infrared Vision", (int8_t*)&CPostEffects::m_bInfraredVision, nil);
		DebugMenuAddVarBool8("PostFX", "Heat Haze FX", (int8_t*)&CPostEffects::m_bHeatHazeFX, nil);
		DebugMenuAddVarBool8("PostFX", "Heat Haze mask mode test", (int8_t*)&CPostEffects::m_bHeatHazeMaskModeTest, nil);
		DebugMenuAddVarBool8("PostFX", "Grain Enable", (int8_t*)&CPostEffects::m_bGrainEnable, nil);
		DebugMenuAddVarBool8("PostFX", "Water Enable", (int8_t*)&CPostEffects::m_waterEnable, nil);

		DebugMenuAddVarBool8("PostFX", "SpeedFX", (int8_t*)&CPostEffects::m_bSpeedFX, nil);
		DebugMenuAddVarBool8("PostFX", "SpeedFX Test", (int8_t*)&CPostEffects::m_bSpeedFXTestMode, nil);
		DebugMenuAddVar("PostFX", "SpeedFX Alpha", &CPostEffects::m_SpeedFXAlpha, nil, 1, 0, 255, nil);

		DebugMenuAddVarBool8("PostFX", "TakePhoto", (int8_t*)0xC8A7C1, nil);
	}
	return 0;
}

void
patch(void)
{
	InterceptCall(&IsAlreadyRunning, delayedPatches, 0x74872D);
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH){
		dllModule = hInst;

		if(*(DWORD*)0x82457C != 0x94BF &&
		   *(DWORD*)0x8245BC == 0x94BF)
			return FALSE;
		patch();
	}

	return TRUE;
}
