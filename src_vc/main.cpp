#include "debug.h"

// VC

DebugMenuAPI gDebugMenuAPI;

HMODULE dllModule;
int gtaversion = -1;
CControllerConfigManager *ctrldummy;

int (*RsEventHandler_orig)(int a, int b);

#define FIELD(type, var, offset) *(type*)((uint8*)var + offset)

void patchDebugCam(void);

static int isPlayerInvincible;
void (__thiscall *CPlayerPed::ProcessControl)(CPlayerPed*);
void
CPlayerPed::ProcessControl_hooked(void)
{
	if(isPlayerInvincible)
		FIELD(float, this, 0x354) = 100.0f;
	ProcessControl(this);
}


void
spawnCar(int id)
{
	CVector playerpos;
	CStreaming__RequestModel(id, 1);
	CStreaming__LoadAllRequestedModels(false);
	if(CStreaming__ms_aInfoForModel[id].loadState == 1){
		if(!(CStreaming__ms_aInfoForModel[id].flags & 1)){
			CStreaming__SetModelIsDeletable(id);
			CStreaming__SetModelTxdIsDeletable(id);
		}
		FindPlayerCoors(&playerpos);
		int node = ThePaths.FindNodeClosestToCoors(playerpos.x, playerpos.y, playerpos.z, 0, 100.0f, 0, 0, 0, 0);
		if(node >= 0){
			CAutomobile *v = (CAutomobile*)CVehicle__new(0x5DC);
			v = v->ctor(id, 1);

			v->_.matrix.matrix.pos.x = ThePaths.nodes[node].x*0.125f;
			v->_.matrix.matrix.pos.y = ThePaths.nodes[node].y*0.125f;
			v->_.matrix.matrix.pos.z = ThePaths.nodes[node].z*0.125f + 4.0f;
			float x = v->_.matrix.matrix.pos.x;
			float y = v->_.matrix.matrix.pos.y;
			float z = v->_.matrix.matrix.pos.z;
			v->_.matrix.SetRotate(0.0f, 0.0f, 3.49f);
			v->_.matrix.matrix.pos.x += x;
			v->_.matrix.matrix.pos.y += y;
			v->_.matrix.matrix.pos.z += z;
			v->bfTypeStatus = v->bfTypeStatus & 7 | 0x20;
			FIELD(int, v, 0x22C) = 1;
			CWorld__Add(v);
		}
	}
}

void
setWantedLevel(int n)
{
	CPlayerPed *p = FindPlayerPed();
	CWanted *wanted = FIELD(CWanted*, p, 0x5f4);
	wanted->CheatWantedLevel(n);
}

int
getWantedLevel(void)
{
	CPlayerPed *p = FindPlayerPed();
	CWanted *wanted = FIELD(CWanted*, p, 0x5f4);
	return FIELD(int, wanted, 0x20);
}

int
delayedPatches10(int a, int b)
{
	// Get some pointers that are likely to be changed by various limit adjusters
	CStreaming__ms_aInfoForModel = (CStreamingInfo*) (*(int*)(0x4AE922 + 3) - 9);

	if(DebugMenuLoad()){
		static const char *weathers[] = {
			"Sunny", "Cloudy", "Rainy", "Foggy", "Extrasunny", "Stormy"
		};
		static const char *vehnames[] = {
			"landstal", "idaho", "stinger", "linerun", "peren", "sentinel", "rio", "firetruk", "trash", "stretch",
			"manana", "infernus", "voodoo", "pony", "mule", "cheetah", "ambulan", "fbicar", "moonbeam", "esperant",
			"taxi", "washing", "bobcat", "mrwhoop", "bfinject", "hunter", "police", "enforcer", "securica", "banshee",
			"predator", "bus", "rhino", "barracks", "cuban", "chopper", "angel", "coach", "cabbie", "stallion",
			"rumpo", "rcbandit", "romero", "packer", "sentxs", "admiral", "squalo", "seaspar", "pizzaboy", "gangbur",
			"airtrain", "deaddodo", "speeder", "reefer", "tropic", "flatbed", "yankee", "caddy", "zebra", "topfun",
			"skimmer", "pcj600", "faggio", "freeway", "rcbaron", "rcraider", "glendale", "oceanic", "sanchez", "sparrow",
			"patriot", "lovefist", "coastg", "dinghy", "hermes", "sabre", "sabretur", "pheonix", "walton", "regina",
			"comet", "deluxo", "burrito", "spand", "marquis", "baggage", "kaufman", "maverick", "vcnmav", "rancher",
			"fbiranch", "virgo", "greenwoo", "jetmax", "hotring", "sandking", "blistac", "polmav", "boxville", "benson",
			"mesa", "rcgoblin", "hotrina", "hotrinb", "bloodra", "bloodrb", "vicechee",
		};
		DebugMenuEntry *e;
		e = DebugMenuAddVar("Time & Weather", "Current Hour", &CClock::ms_nGameClockHours, nil, 1, 0, 23, nil);
		DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddVar("Time & Weather", "Current Minute", &CClock::ms_nGameClockMinutes,
			[](){ CWeather::InterpolationValue = CClock::ms_nGameClockMinutes/60.0f + CClock::ms_nGameClockSeconds/3600.0f; }, 1, 0, 59, nil);
		DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddVar("Time & Weather", "Old Weather", &CWeather::OldWeatherType, nil, 1, 0, 5, weathers);
		DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddVar("Time & Weather", "New Weather", &CWeather::NewWeatherType, nil, 1, 0, 5, weathers);
		DebugMenuEntrySetWrap(e, true);
		DebugMenuAddVar("Time & Weather", "Time scale", (float*)0x97F264, nil, 0.1f, 0.0f, 10.0f);

		InterceptVmethod(&CPlayerPed::ProcessControl, &CPlayerPed::ProcessControl_hooked, 0x694D90);
		DebugMenuAddVarBool32("Player", "Invincible", &isPlayerInvincible, nil);
		DebugMenuAddCmd("Player", "Clear Wanted Level", [](){ setWantedLevel(0); });
		DebugMenuAddCmd("Player", "Increase Wanted Level", [](){ setWantedLevel(getWantedLevel()+2); });

		DebugMenuAddCmd("Cheats", "Weapons 1", [](){ ((void (*)(void))0x4AEE00)(); });
		DebugMenuAddCmd("Cheats", "Weapons 2", [](){ ((void (*)(void))0x4AEC70)(); });
		DebugMenuAddCmd("Cheats", "Weapons 3", [](){ ((void (*)(void))0x4AEAD0)(); });

		static int vehId = 130;
		DebugMenuAddVar("Spawn", "Spawn Vehicle ID", &vehId, nil, 1, 130, 236, vehnames);
		DebugMenuAddCmd("Spawn", "Spawn Vehicle", [](){ spawnCar(vehId); });
		DebugMenuAddCmd("Spawn", "Spawn Infernus", [](){ spawnCar(141); });
		DebugMenuAddCmd("Spawn", "Spawn Cheetah", [](){ spawnCar(145); });
		DebugMenuAddCmd("Spawn", "Spawn Stinger", [](){ spawnCar(132); });
		DebugMenuAddCmd("Spawn", "Spawn Phoenix", [](){ spawnCar(207); });
		DebugMenuAddCmd("Spawn", "Spawn Deluxo", [](){ spawnCar(211); });
		DebugMenuAddCmd("Spawn", "Spawn Hotring", [](){ spawnCar(224); });
		DebugMenuAddCmd("Spawn", "Spawn PCJ 600", [](){ spawnCar(191); });
		DebugMenuAddCmd("Spawn", "Spawn Admiral", [](){ spawnCar(175); });
		DebugMenuAddCmd("Spawn", "Spawn Stretch", [](){ spawnCar(139); });
		DebugMenuAddCmd("Spawn", "Spawn Lovefist", [](){ spawnCar(201); });
		DebugMenuAddCmd("Spawn", "Spawn BF Injection", [](){ spawnCar(154); });
		DebugMenuAddCmd("Spawn", "Spawn Cuban", [](){ spawnCar(164); });
		DebugMenuAddCmd("Spawn", "Spawn Voodoo", [](){ spawnCar(142); });
		DebugMenuAddCmd("Spawn", "Spawn Hearse", [](){ spawnCar(172); });
		DebugMenuAddCmd("Spawn", "Spawn Taxi", [](){ spawnCar(150); });
		DebugMenuAddCmd("Spawn", "Spawn Police", [](){ spawnCar(156); });
		DebugMenuAddCmd("Spawn", "Spawn FBI Rancher", [](){ spawnCar(220); });
		DebugMenuAddCmd("Spawn", "Spawn Rhino", [](){ spawnCar(162); });
		DebugMenuAddCmd("Spawn", "Spawn Sparrow", [](){ spawnCar(199); });
		DebugMenuAddCmd("Spawn", "Spawn Sea Sparrow", [](){ spawnCar(177); });
		DebugMenuAddCmd("Spawn", "Spawn Hunter", [](){ spawnCar(155); });
	}
	patchDebugCam();
	return RsEventHandler_orig(a, b);
}

void
patchVC10(void)
{
	InterceptCall(&RsEventHandler_orig, delayedPatches10, 0x5FFAFE);
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH){
		dllModule = hInst;

		AddressByVersion<uint32_t>(0, 0, 0, 0, 0, 0);
		if (gtaversion == VC_10)
			patchVC10();
	}

	return TRUE;
}
