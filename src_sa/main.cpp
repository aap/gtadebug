#include "debug.h"

// SA

DebugMenuAPI gDebugMenuAPI;

HMODULE dllModule;
int gtaversion = -1;
CControllerConfigManager *ctrldummy;

#define FIELD(type, var, offset) *(type*)((uint8*)var + offset)

static int isPlayerInvincible;
void (__thiscall *CPlayerPed::ProcessControl)(CPlayerPed*);
void
CPlayerPed::ProcessControl_hooked(void)
{
	// Not really invincible :/
	// maybe check CPedDamageResponseCalculator::ComputeWillKillPed
	if(isPlayerInvincible)
		FIELD(float, this, 0x540) = 100.0f;
	ProcessControl(this);
}


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
		DebugMenuEntry *e;
		e = DebugMenuAddVar("Time & Weather", "Current Hour", &CClock::ms_nGameClockHours, nil, 1, 0, 23, nil);
		DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddVar("Time & Weather", "Current Minute", &CClock::ms_nGameClockMinutes,
			[](){ CWeather::InterpolationValue = CClock::ms_nGameClockMinutes/60.0f + CClock::ms_nGameClockSeconds/3600.0f; }, 1, 0, 59, nil);
		DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddVar("Time & Weather", "Old Weather", &CWeather::OldWeatherType, nil, 1, 0, 20, weathers);
		DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddVar("Time & Weather", "New Weather", &CWeather::NewWeatherType, nil, 1, 0, 20, weathers);
		DebugMenuEntrySetWrap(e, true);
		DebugMenuAddVar("Time & Weather", "Time scale", &CTimer::ms_fTimeScale, nil, 0.1f, 0.0f, 10.0f);

		static int vehId = 400;
		DebugMenuAddVar("Spawn", "Spawn Vehicle ID", &vehId, nil, 1, 400, 611, vehnames);
		DebugMenuAddCmd("Spawn", "Spawn Vehicle", [](){ VehicleCheat(vehId); });
		DebugMenuAddCmd("Spawn", "Spawn Bullet", [](){ VehicleCheat(541); });
		DebugMenuAddCmd("Spawn", "Spawn Cheetah", [](){ VehicleCheat(415); });
		DebugMenuAddCmd("Spawn", "Spawn Hotring", [](){ VehicleCheat(494); });
		DebugMenuAddCmd("Spawn", "Spawn Voodoo", [](){ VehicleCheat(412); });
		DebugMenuAddCmd("Spawn", "Spawn Tampa", [](){ VehicleCheat(549); });
		DebugMenuAddCmd("Spawn", "Spawn Rancher", [](){ VehicleCheat(489); });
		DebugMenuAddCmd("Spawn", "Spawn Admiral", [](){ VehicleCheat(445); });
		DebugMenuAddCmd("Spawn", "Spawn Stretch", [](){ VehicleCheat(409); });
		DebugMenuAddCmd("Spawn", "Spawn BJ Injection", [](){ VehicleCheat(424); });
		DebugMenuAddCmd("Spawn", "Spawn Taxi", [](){ VehicleCheat(420); });
		DebugMenuAddCmd("Spawn", "Spawn Police (LA)", [](){ VehicleCheat(596); });
		DebugMenuAddCmd("Spawn", "Spawn Police Ranger", [](){ VehicleCheat(599); });
		DebugMenuAddCmd("Spawn", "Spawn FBI Rancher", [](){ VehicleCheat(490); });
		DebugMenuAddCmd("Spawn", "Spawn Rhino", [](){ VehicleCheat(432); });
		DebugMenuAddCmd("Spawn", "Spawn SWAT van", [](){ VehicleCheat(601); });
		DebugMenuAddCmd("Spawn", "Spawn FBI truck", [](){ VehicleCheat(528); });
		DebugMenuAddCmd("Spawn", "Spawn Sea Sparrow", [](){ VehicleCheat(447); });
		DebugMenuAddCmd("Spawn", "Spawn Hunter", [](){ VehicleCheat(425); });
		DebugMenuAddCmd("Spawn", "Spawn Hydra", [](){ VehicleCheat(520); });

		static bool *cheatsActive = (bool*)0x969130;	// 92

		// Weapons and player stats
		DebugMenuAddCmd("Cheats|Player", "Weapons 1", [](){ ((void (*)(void))0x4385B0)(); });
		DebugMenuAddCmd("Cheats|Player", "Weapons 2", [](){ ((void (*)(void))0x438890)(); });
		DebugMenuAddCmd("Cheats|Player", "Weapons 3", [](){ ((void (*)(void))0x438B30)(); });
		DebugMenuAddCmd("Cheats|Player", "Money Armour Health", [](){ ((void (*)(void))0x438E40)(); });
		DebugMenuAddCmd("Cheats|Player", "Wanted Level Up", [](){ ((void (*)(void))0x438E90)(); });
		DebugMenuAddCmd("Cheats|Player", "Wanted Level Down", [](){ ((void (*)(void))0x438F20)(); });
		DebugMenuAddCmd("Cheats|Player", "Not Wanted", [](){ ((void (*)(void))0x4396C0)(); });
		DebugMenuAddCmd("Cheats|Player", "Wanted", [](){ ((void (*)(void))0x4396F0)(); });
		DebugMenuAddCmd("Cheats|Player", "Parachute", [](){ ((void (*)(void))0x4395B0)(); });
		DebugMenuAddCmd("Cheats|Player", "Jetpack", [](){ ((void (*)(void))0x439600)(); });
		DebugMenuAddVarBool8("Cheats|Player", "No Food", (int8_t*)&cheatsActive[68], nil);
		DebugMenuAddVarBool8("Cheats|Player", "Ammo", (int8_t*)&cheatsActive[72], nil);
		DebugMenuAddVarBool8("Cheats|Player", "Recruit me", (int8_t*)&cheatsActive[76], nil);
		DebugMenuAddVarBool8("Cheats|Player", "Recruit me AK-47", (int8_t*)&cheatsActive[77], nil);
		DebugMenuAddVarBool8("Cheats|Player", "Recruit me Rocket", (int8_t*)&cheatsActive[78], nil);
		DebugMenuAddVarBool8("Cheats|Player", "Respect", (int8_t*)&cheatsActive[79], nil);
		DebugMenuAddVarBool8("Cheats|Player", "Sex appeal", (int8_t*)&cheatsActive[80], nil);
		DebugMenuAddVarBool8("Cheats|Player", "Pimp", (int8_t*)&cheatsActive[90], nil);
		DebugMenuAddVarBool8("Cheats|Player", "High Jump", (int8_t*)&cheatsActive[60], nil);
		DebugMenuAddVarBool8("Cheats|Player", "Bulletproof", (int8_t*)&cheatsActive[61], nil);
		DebugMenuAddVarBool8("Cheats|Player", "Superpunch", (int8_t*)&cheatsActive[67], nil);

		// Spawn
		DebugMenuAddCmd("Cheats|Spawn", "Tank", [](){ ((void (*)(void))0x43A4A0)(); });
		DebugMenuAddCmd("Cheats|Spawn", "Stock Car", [](){ ((void (*)(void))0x43A4B0)(); });
		DebugMenuAddCmd("Cheats|Spawn", "Stock Car 2", [](){ ((void (*)(void))0x43A4C0)(); });
		DebugMenuAddCmd("Cheats|Spawn", "Stock Car 3", [](){ ((void (*)(void))0x43A4D0)(); });
		DebugMenuAddCmd("Cheats|Spawn", "Stock Car 4", [](){ ((void (*)(void))0x43A4E0)(); });
		DebugMenuAddCmd("Cheats|Spawn", "Hearse", [](){ ((void (*)(void))0x43A4F0)(); });
		DebugMenuAddCmd("Cheats|Spawn", "Lovefist", [](){ ((void (*)(void))0x43A500)(); });
		DebugMenuAddCmd("Cheats|Spawn", "Trashmaster", [](){ ((void (*)(void))0x43A510)(); });
		DebugMenuAddCmd("Cheats|Spawn", "Golfcart", [](){ ((void (*)(void))0x43A520)(); });
		DebugMenuAddCmd("Cheats|Spawn", "Flyboy", [](){ ((void (*)(void))0x43A530)(); });
		DebugMenuAddCmd("Cheats|Spawn", "Vortex", [](){ ((void (*)(void))0x43A540)(); });
//		DebugMenuAddCmd("Cheats|Spawn", "Predator", [](){ ((void (*)(void))0x4395A0)(); });	// nop
		DebugMenuAddCmd("Cheats|Spawn", "Apache", [](){ ((void (*)(void))0x43A550)(); });
		DebugMenuAddCmd("Cheats|Spawn", "Quad", [](){ ((void (*)(void))0x43A560)(); });
		DebugMenuAddCmd("Cheats|Spawn", "Tanker", [](){ ((void (*)(void))0x43A570)(); });
		DebugMenuAddCmd("Cheats|Spawn", "Dozer", [](){ ((void (*)(void))0x43A660)(); });
		DebugMenuAddCmd("Cheats|Spawn", "Stunt Plane", [](){ ((void (*)(void))0x43A670)(); });
		DebugMenuAddCmd("Cheats|Spawn", "Monster Truck", [](){ ((void (*)(void))0x43A680)(); });

		// Less interesting player stats
		DebugMenuAddCmd("Cheats|Stats", "Fat", [](){ ((void (*)(void))0x439110)(); });
		DebugMenuAddCmd("Cheats|Stats", "Muscle", [](){ ((void (*)(void))0x439150)(); });
		DebugMenuAddCmd("Cheats|Stats", "Skinny", [](){ ((void (*)(void))0x439190)(); });
		DebugMenuAddCmd("Cheats|Stats", "Stamina", [](){ ((void (*)(void))0x439930)(); });
		DebugMenuAddCmd("Cheats|Stats", "Weapon Skills", [](){ ((void (*)(void))0x439940)(); });
		DebugMenuAddCmd("Cheats|Stats", "Vehicle Skills", [](){ ((void (*)(void))0x4399D0)(); });

		// Time and weather
		DebugMenuAddCmd("Cheats|Time & Weather", "Sunny Weather", [](){ ((void (*)(void))0x438F40)(); });
		DebugMenuAddCmd("Cheats|Time & Weather", "Extrasunny Weather", [](){ ((void (*)(void))0x438F50)(); });
		DebugMenuAddCmd("Cheats|Time & Weather", "Cloudy Weather", [](){ ((void (*)(void))0x438F60)(); });
		DebugMenuAddCmd("Cheats|Time & Weather", "Rainy Weather", [](){ ((void (*)(void))0x438F70)(); });
		DebugMenuAddCmd("Cheats|Time & Weather", "Foggy Weather", [](){ ((void (*)(void))0x438F80)(); });
		DebugMenuAddCmd("Cheats|Time & Weather", "Storm", [](){ ((void (*)(void))0x439570)(); });
		DebugMenuAddCmd("Cheats|Time & Weather", "Sandstorm", [](){ ((void (*)(void))0x439590)(); });
		DebugMenuAddCmd("Cheats|Time & Weather", "Midnight", [](){ ((void (*)(void))0x439510)(); });
		DebugMenuAddCmd("Cheats|Time & Weather", "Dusk", [](){ ((void (*)(void))0x439540)(); });
		DebugMenuAddCmd("Cheats|Time & Weather", "Fast Time", [](){ ((void (*)(void))0x438F90)(); });
		DebugMenuAddCmd("Cheats|Time & Weather", "Slow Time", [](){ ((void (*)(void))0x438FC0)(); });
		DebugMenuAddVarBool8("Cheats|Time & Weather", "Fast Weather", (int8_t*)&cheatsActive[11], nil);

		// Misc stuff
		DebugMenuAddVarBool8("Cheats|Misc", "Back to the Future", (int8_t*)&cheatsActive[34], nil);
		DebugMenuAddVarBool8("Cheats|Misc", "Nitro", (int8_t*)&cheatsActive[53], nil);
		DebugMenuAddVarBool8("Cheats|Misc", "Taxi Nitro", (int8_t*)&cheatsActive[91], nil);
		DebugMenuAddVarBool8("Cheats|Misc", "Weapons for all", (int8_t*)&cheatsActive[16], nil);
		DebugMenuAddVarBool8("Cheats|Misc", "Only render Wheels", (int8_t*)&cheatsActive[27], nil);
		DebugMenuAddVarBool8("Cheats|Misc", "Strong Grip", (int8_t*)&cheatsActive[28], nil);
		DebugMenuAddVarBool8("Cheats|Misc", "Traffic Lights", (int8_t*)&cheatsActive[30], nil);
		DebugMenuAddVarBool8("Cheats|Misc", "Mad Drivers", (int8_t*)&cheatsActive[31], nil);
		DebugMenuAddVarBool8("Cheats|Misc", "Flying Fish", (int8_t*)&cheatsActive[35], nil);
		DebugMenuAddVarBool8("Cheats|Misc", "Flying Cars", (int8_t*)&cheatsActive[48], nil);
		DebugMenuAddVarBool8("Cheats|Misc", "CJ Phone Home", (int8_t*)&cheatsActive[49], nil);
		DebugMenuAddVarBool8("Cheats|Misc", "Vehicle of Death", (int8_t*)&cheatsActive[52], nil);
		DebugMenuAddVarBool8("Cheats|Misc", "Superpower Car", (int8_t*)&cheatsActive[54], nil);
		DebugMenuAddVarBool8("Cheats|Misc", "Scuba", (int8_t*)&cheatsActive[62], nil);
		DebugMenuAddVarBool8("Cheats|Misc", "Wasteland", (int8_t*)&cheatsActive[74], nil);
		DebugMenuAddCmd("Cheats|Misc", "Mayhem", [](){ ((void (*)(void))0x439B20)(); });
		DebugMenuAddCmd("Cheats|Misc", "Everybody attacks Player", [](){ ((void (*)(void))0x439C70)(); });
		DebugMenuAddCmd("Cheats|Misc", "Blow up Cars", [](){ ((void (*)(void))0x439D80)(); });
		DebugMenuAddCmd("Cheats|Misc", "Suicide", [](){ ((void (*)(void))0x438FF0)(); });
		DebugMenuAddCmd("Cheats|Misc", "Pink Cars", [](){ ((void (*)(void))0x4390D0)(); });
		DebugMenuAddCmd("Cheats|Misc", "Black Cars", [](){ ((void (*)(void))0x4390F0)(); });
		DebugMenuAddCmd("Cheats|Misc", "Elvis lives", [](){ ((void (*)(void))0x4391D0)(); });
		DebugMenuAddCmd("Cheats|Misc", "Village People", [](){ ((void (*)(void))0x439DD0)(); });
		DebugMenuAddCmd("Cheats|Misc", "Beach Party", [](){ ((void (*)(void))0x439230)(); });
		DebugMenuAddCmd("Cheats|Misc", "Gangs", [](){ ((void (*)(void))0x439360)(); });
		DebugMenuAddCmd("Cheats|Misc", "Gang Land", [](){ ((void (*)(void))0x4393D0)(); });
		DebugMenuAddCmd("Cheats|Misc", "Ninja", [](){ ((void (*)(void))0x439E50)(); });
		DebugMenuAddCmd("Cheats|Misc", "Love conquers all", [](){ ((void (*)(void))0x4393F0)(); });
		DebugMenuAddCmd("Cheats|Misc", "All Cars are shit", [](){ ((void (*)(void))0x4394B0)(); });
		DebugMenuAddCmd("Cheats|Misc", "All Cars are great", [](){ ((void (*)(void))0x4394E0)(); });
		DebugMenuAddCmd("Cheats|Misc", "Riot", [](){ ((void (*)(void))0x439710)(); });
		DebugMenuAddCmd("Cheats|Misc", "Funhouse", [](){ ((void (*)(void))0x439720)(); });
		DebugMenuAddCmd("Cheats|Misc", "Adrenaline", [](){ ((void (*)(void))0x439880)(); });
		DebugMenuAddCmd("Cheats|Misc", "Drive-by", [](){ ((void (*)(void))0x4398D0)(); });
		DebugMenuAddCmd("Cheats|Misc", "Countryside Invasion", [](){ ((void (*)(void))0x439F60)(); });


		InterceptVmethod(&CPlayerPed::ProcessControl, &CPlayerPed::ProcessControl_hooked, 0x86D190);
		DebugMenuAddVarBool32("Player", "Invincible", &isPlayerInvincible, nil);
		DebugMenuAddCmd("Player", "Clear Wanted Level", [](){ FindPlayerPed(-1)->SetWantedLevel(0); });
		DebugMenuAddVar("Player", "Allowed Territory", (int*)0xb790f4, nil, 1, 0, 4, nil);


		//
		// PostFX
		//

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
	patchDebugCam();
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
