#include "debug.h"

DebugMenuAPI gDebugMenuAPI;

HMODULE dllModule;
int gtaversion = -1;

int (*RsEventHandler_orig)(int a, int b);
CStreamingInfo *CStreaming__ms_aInfoForModel;
WRAPPER void CStreaming__RequestModel(int id, int flags) { EAXJMP(0x407EA0); }
WRAPPER void CStreaming__LoadAllRequestedModels(bool b) { EAXJMP(0x40A440); }
WRAPPER void CStreaming__SetModelIsDeletable(int id) { EAXJMP(0x40A790); }

WRAPPER CVehicle *CVehicle__new(int size) { EAXJMP(0x551120); }
WRAPPER void CWorld__Add(CEntity *e) { EAXJMP(0x4AE930); }

struct PathNode
{
	float x, y, z;
	float a, b, c, d, e;	// no idea what this is yet
};

struct CPathFind {
	PathNode nodes[1];	// no no idea how many

	int FindNodeClosestToCoors(float x, float y, float z, char a7, float f8, char a9, char a10);
};
CPathFind &ThePaths = *(CPathFind*)0x8F6754;
WRAPPER int CPathFind::FindNodeClosestToCoors(float x, float y, float z, char a7, float f8, char a9, char a10) {  EAXJMP(0x42CC30); }

#define FIELD(type, var, offset) *(type*)((uint8*)var + offset)

static const char *carnames[] = {
	"landstal", "idaho", "stinger", "linerun", "peren", "sentinel", "patriot", "firetruk", "trash", "stretch", "manana", "infernus", "blista", "pony",
	"mule", "cheetah", "ambulan", "fbicar", "moonbeam", "esperant", "taxi", "kuruma", "bobcat", "mrwhoop", "bfinject", "corpse", "police", "enforcer",
	"securica", "banshee", "predator", "bus", "rhino", "barracks", "train", "chopper", "dodo", "coach", "cabbie", "stallion", "rumpo", "rcbandit",
	"bellyup", "mrwongs", "mafia", "yardie", "yakuza", "diablos", "columb", "hoods", "airtrain", "deaddodo", "speeder", "reefer", "panlant", "flatbed",
	"yankee", "escape", "borgnine", "toyz", "ghost",
};

static const char *pednames[] = {
	"null", "cop", "swat", "fbi", "army", "medic", "fireman", "male01", "taxi_d",
	"pimp", "gang01", "gang02", "gang03", "gang04", "gang05", "gang06", "gang07",
	"gang08", "gang09", "gang10", "gang11", "gang12", "gang13", "gang14",
	"criminal01", "criminal02", "special01", "special02", "special03", "special04",
	"male02", "male03", "fatmale01", "fatmale02", "female01", "female02", "female03",
	"fatfemale01", "fatfemale02", "prostitute", "prostitute2", "p_man1", "p_man2",
	"p_wom1", "p_wom2", "ct_man1", "ct_man2", "ct_wom1", "ct_wom2", "li_man1",
	"li_man2", "li_wom1", "li_wom2", "docker1", "docker2", "scum_man", "scum_wom",
	"worker1", "worker2", "b_man1", "b_man2", "b_man3", "b_wom1", "b_wom2",
	"b_wom3", "mod_man", "mod_wom", "st_man", "st_wom", "fan_man1", "fan_man2",
	"fan_wom", "hos_man", "hos_wom", "const1", "const2", "shopper1", "shopper2",
	"shopper3", "stud_man", "stud_wom", "cas_man", "cas_wom",
};

void
changePlayerModel(int id)
{
	CPed *playerped;
	int flags;

	playerped = (CPed*)FindPlayerPed();
	if(playerped->IsPedInControl() && CModelInfo::GetModelInfo("player", 0) && CModelInfo::ms_modelInfoPtrs[id]){
		int animGroup = FIELD(int, playerped, 0x1d4);
		flags = CStreaming__ms_aInfoForModel[playerped->nModelIndex].flags;
		((CEntityVMT*)playerped->vtable)->DeleteRwObject(playerped);
		CStreaming__RequestModel(id, 5);
		CStreaming__LoadAllRequestedModels(false);
		playerped->nModelIndex = -1;
		((CEntityVMT*)playerped->vtable)->SetModelIndex(playerped, id);
		FIELD(int, playerped, 0x1d4) = animGroup;
		if(id != 0 && !(flags & 1))
			CStreaming__SetModelIsDeletable(id);
	}
}

void
spawnCar(int id)
{
	CVector playerpos;
	CStreaming__RequestModel(id, 0);
	CStreaming__LoadAllRequestedModels(false);
	if(CStreaming__ms_aInfoForModel[id].loadState == 1){
		FindPlayerCoors(&playerpos);
		int node = ThePaths.FindNodeClosestToCoors(playerpos.x, playerpos.y, playerpos.z, 0, 100.0f, 0, 0);
		if(node >= 0){
			CAutomobile *v = (CAutomobile*)CVehicle__new(0x5A8);
			v = v->ctor(id, 2);

			v->matrix.matrix.pos.x = ThePaths.nodes[node].x;
			v->matrix.matrix.pos.y = ThePaths.nodes[node].y;
			v->matrix.matrix.pos.z = ThePaths.nodes[node].z + 4.0f;
			float x = v->matrix.matrix.pos.x;
			float y = v->matrix.matrix.pos.y;
			float z = v->matrix.matrix.pos.z;
			v->matrix.SetRotate(0.0f, 0.0f, 3.49f);
			v->matrix.matrix.pos.x += x;
			v->matrix.matrix.pos.y += y;
			v->matrix.matrix.pos.z += z;
			v->bfTypeStatus = v->bfTypeStatus & 7 | 0x20;
			FIELD(int, v, 0x224) = 1;
			CWorld__Add(v);
		}
	}
}

int
delayedPatches10(int a, int b)
{
	// Get some pointers that are likely to be changed by various limit adjusters
	CModelInfo::ms_modelInfoPtrs = (CBaseModelInfo**) *(int*)(0x49114F + 3);
	CStreaming__ms_aInfoForModel = (CStreamingInfo*) (*(int*)(0x491171 + 4) - 9);

	if(DebugMenuLoad()){
		static const char *weathers[] = {
			"Sunny", "Cloudy", "Rainy", "Foggy"
		};
		MenuEntry *e;
		e = DebugMenuAddVar("Time & Weather", "Current Hour", (uint8*)&CClock::ms_nGameClockHours, nil, 1, 0, 23, nil);
		DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddVar("Time & Weather", "Current Minute", (uint8*)&CClock::ms_nGameClockMinutes,
			[](){ CWeather::InterpolationValue = CClock::ms_nGameClockMinutes/60.0f; }, 1, 0, 59, nil);
			DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddVar("Time & Weather", "Old Weather", (int16*)&CWeather::OldWeatherType, nil, 1, 0, 3, weathers);
		DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddVar("Time & Weather", "New Weather", (int16*)&CWeather::NewWeatherType, nil, 1, 0, 3, weathers);
		DebugMenuEntrySetWrap(e, true);
		DebugMenuAddVar("Time & Weather", "Time scale", (float*)0x8F2C20, nil, 0.1f, 0.0f, 10.0f);
	
		DebugMenuAddCmd("Cheats", "Weapons", [](){ ((void (*)(void))0x490D90)(); });
		DebugMenuAddCmd("Cheats", "Money", [](){ ((void (*)(void))0x491430)(); });
		DebugMenuAddCmd("Cheats", "Health", [](){ ((void (*)(void))0x490E70)(); });
		DebugMenuAddCmd("Cheats", "Wanted level up", [](){ ((void (*)(void))0x491490)(); });
		DebugMenuAddCmd("Cheats", "Wanted level down", [](){ ((void (*)(void))0x4914F0)(); });
		DebugMenuAddCmd("Cheats", "Tank", [](){ ((void (*)(void))0x490EE0)(); });
		DebugMenuAddCmd("Cheats", "Blow up cars", [](){ ((void (*)(void))0x491040)(); });
		DebugMenuAddCmd("Cheats", "Change player", [](){ ((void (*)(void))0x4910B0)(); });
		DebugMenuAddCmd("Cheats", "Mayhem", [](){ ((void (*)(void))0x4911C0)(); });
		DebugMenuAddCmd("Cheats", "Everybody attacks player", [](){ ((void (*)(void))0x491270)(); });
		DebugMenuAddCmd("Cheats", "Weapons for all", [](){ ((void (*)(void))0x491370)(); });
		DebugMenuAddCmd("Cheats", "Fast time", [](){ ((void (*)(void))0x4913A0)(); });
		DebugMenuAddCmd("Cheats", "Slow time", [](){ ((void (*)(void))0x4913F0)(); });
		DebugMenuAddCmd("Cheats", "Armour", [](){ ((void (*)(void))0x491460)(); });
		DebugMenuAddCmd("Cheats", "Sunny weather", [](){ ((void (*)(void))0x491520)(); });
		DebugMenuAddCmd("Cheats", "Cloudy weather", [](){ ((void (*)(void))0x491550)(); });
		DebugMenuAddCmd("Cheats", "Rainy weather", [](){ ((void (*)(void))0x491580)(); });
		DebugMenuAddCmd("Cheats", "Foggy weather", [](){ ((void (*)(void))0x4915B0)(); });
		DebugMenuAddCmd("Cheats", "Fast weather", [](){ ((void (*)(void))0x4915E0)(); });
		DebugMenuAddCmd("Cheats", "Only render wheels", [](){ ((void (*)(void))0x491610)(); });
		DebugMenuAddCmd("Cheats", "Chitty chitty bang bang", [](){ ((void (*)(void))0x491640)(); });
		DebugMenuAddCmd("Cheats", "Strong grip", [](){ ((void (*)(void))0x491670)(); });
		DebugMenuAddCmd("Cheats", "Nasty limbs", [](){ ((void (*)(void))0x4916A0)(); });

		static uint8 test;
		DebugMenuAddVar("Misc", "test", &test, nil, 1, 0, 255, nil);

		static int playerId = 0;
		e = DebugMenuAddVar("Misc", "Player model", &playerId, [](){
				if(playerId >= 26 && playerId <= 29 || playerId == 8)
					return;
				changePlayerModel(playerId);
			}, 1, 0, 82, pednames);
		DebugMenuEntrySetWrap(e, true);

		static int spawnCarId = 90;
		e = DebugMenuAddVar("Misc", "Spawn Car ID", &spawnCarId, nil, 1, 90, 150, carnames);
		DebugMenuEntrySetWrap(e, true);
		DebugMenuAddCmd("Misc", "Spawn Car", [](){
			if(spawnCarId == 124 || // train
			   spawnCarId == 125 || // chopper
			   spawnCarId == 140 || // airtrain
			   spawnCarId == 141 || // deaddodo
			   spawnCarId == 147 ||  // escape
				// boats crash too
			   spawnCarId == 120 ||  // predator
			   spawnCarId == 142 ||  // speeder
			   spawnCarId == 143 ||  // reefer
			   spawnCarId == 150)  // ghost
				return;
			spawnCar(spawnCarId);
		});

		installColDebug();
	}
	return RsEventHandler_orig(a, b);
}

void
patchIII10(void)
{
	InterceptCall(&RsEventHandler_orig, delayedPatches10, 0x58275E);
	// camera and similar stuff that's still in the game
	debughooks();
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH){
		dllModule = hInst;

		AddressByVersion<uint32_t>(0, 0, 0, 0, 0, 0);
		if (gtaversion == III_10)
			patchIII10();
	}

	return TRUE;
}
