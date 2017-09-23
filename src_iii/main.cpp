#include "debug.h"

// III

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

int drawLODs = 1;
bool (*CRenderer__SetupBigBuildingVisibility_orig)(CEntity *e);
bool CRenderer__SetupBigBuildingVisibility(CEntity *e)
{
	if(drawLODs)
		return CRenderer__SetupBigBuildingVisibility_orig(e);
	return false;
}


enum ModelIDs {
	MODELID_STINGER = 92,
	MODELID_INFERNUS = 101,
	MODELID_CHEETAH = 105,
	MODELID_ESPERANTO = 109,
	MODELID_TAXI = 110,
	MODELID_POLICE = 116,
	MODELID_BANSHEE = 119,
	MODELID_DODO = 126,
	MODELID_YAKUZA = 136,

	// these crash when spawned
	MODELID_TRAIN = 124,
	MODELID_CHOPPER = 125,
	MODELID_AIRTRAIN = 140,
	MODELID_DEADDODO = 141,
	MODELID_ESCAPE = 147,
	MODELID_PREDATOR = 120,
	MODELID_SPEEDER = 142,
	MODELID_REEFER = 143,
	MODELID_GHOST = 150,
};

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

static int isPlayerInvincible;

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

DebugMenuEntry *carCol1;
DebugMenuEntry *carCol2;

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

			DebugMenuEntrySetAddress(carCol1, &FIELD(uchar, v, 0x19C));
			DebugMenuEntrySetAddress(carCol2, &FIELD(uchar, v, 0x19D));
			//if(id == MODELID_ESPERANTO)
			//	FIELD(uchar, v, 0x19C) = 54;

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

void __declspec(naked)
PlayerPedInvincible(void)
{
	_asm{
		mov	eax, isPlayerInvincible
		test	eax, eax
		jz	done
		mov	dword ptr [esi+0x2c0], 0x42C80000 // 100.0f
	done:
		add	esp,20h
		pop	ebp
		pop	esi
		pop	ebx
		retn
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
		DebugMenuEntry *e;
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

		DebugMenuAddVarBool32("Player", "Invincible", &isPlayerInvincible, nil);

		DebugMenuAddVarBool32("Misc", "Draw LODs", &drawLODs, nil);
		InterceptCall(&CRenderer__SetupBigBuildingVisibility_orig, CRenderer__SetupBigBuildingVisibility, 0x4A931D);

		static int playerId = 0;
		e = DebugMenuAddVar("Player", "Player model", &playerId, [](){
				if(playerId >= 26 && playerId <= 29 || playerId == 8)
					return;
				changePlayerModel(playerId);
			}, 1, 0, 82, pednames);
		DebugMenuEntrySetWrap(e, true);

		static int spawnCarId = 90;
		e = DebugMenuAddVar("Spawn", "Spawn Car ID", &spawnCarId, nil, 1, 90, 150, carnames);
		DebugMenuEntrySetWrap(e, true);
		DebugMenuAddCmd("Spawn", "Spawn Car", [](){
			if(spawnCarId == MODELID_TRAIN ||
			   spawnCarId == MODELID_CHOPPER ||
			   spawnCarId == MODELID_AIRTRAIN ||
			   spawnCarId == MODELID_DEADDODO ||
			   spawnCarId == MODELID_ESCAPE ||
				// boats crash too
			   spawnCarId == MODELID_PREDATOR ||
			   spawnCarId == MODELID_SPEEDER ||
			   spawnCarId == MODELID_REEFER ||
			   spawnCarId == MODELID_GHOST)
				return;
			spawnCar(spawnCarId);
		});
		static uchar dummy;
		carCol1 = DebugMenuAddVar("Spawn", "First colour", &dummy, nil, 1, 0, 255, nil);
		carCol2 = DebugMenuAddVar("Spawn", "Second colour", &dummy, nil, 1, 0, 255, nil);
		DebugMenuAddCmd("Spawn", "Spawn Stinger", [](){ spawnCar(MODELID_STINGER); });
		DebugMenuAddCmd("Spawn", "Spawn Internus", [](){ spawnCar(MODELID_INFERNUS); });
		DebugMenuAddCmd("Spawn", "Spawn Cheetah", [](){ spawnCar(MODELID_CHEETAH); });
		DebugMenuAddCmd("Spawn", "Spawn Esperanto", [](){ spawnCar(MODELID_ESPERANTO); });
		DebugMenuAddCmd("Spawn", "Spawn Taxi", [](){ spawnCar(MODELID_TAXI); });
		DebugMenuAddCmd("Spawn", "Spawn Police", [](){ spawnCar(MODELID_POLICE); });
		DebugMenuAddCmd("Spawn", "Spawn Banshee", [](){ spawnCar(MODELID_BANSHEE); });
		DebugMenuAddCmd("Spawn", "Spawn Yakuza", [](){ spawnCar(MODELID_YAKUZA); });
		DebugMenuAddCmd("Spawn", "Spawn Dodo", [](){ spawnCar(MODELID_DODO); });

		installColDebug();

		InjectHook(0x4F0758, PlayerPedInvincible, PATCH_JUMP);
	}
	//void privatehooks(void);
	//privatehooks();

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
