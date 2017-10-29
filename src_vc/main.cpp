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

DebugMenuEntry *carCol1;
DebugMenuEntry *carCol2;

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
			CVehicle* pVehicle;
		
			if(CModelInfo::IsBoatModel(id)){
				CBoat* pVeh = (CBoat*)CVehicle__new(0x4C0);
				pVeh = pVeh->ctor(id, 1);
				pVehicle = dynamic_cast<CVehicle*>(pVeh);
			}else if (CModelInfo::IsBikeModel(id)){
				CBike* pVeh = (CBike*)CVehicle__new(0x4EC);
				pVeh = pVeh->ctor(id, 1);
				pVehicle = dynamic_cast<CVehicle*>(pVeh);
			}else{
				CAutomobile* pVeh = (CAutomobile*)CVehicle__new(0x5DC);
				pVeh = pVeh->ctor(id, 1);
				pVehicle = dynamic_cast<CVehicle*>(pVeh);
			}

			DebugMenuEntrySetAddress(carCol1, &FIELD(uchar, pVehicle, 0x1A0));
			DebugMenuEntrySetAddress(carCol2, &FIELD(uchar, pVehicle, 0x1A1));

			pVehicle->_.matrix.matrix.pos.x = ThePaths.nodes[node].x*0.125f;
			pVehicle->_.matrix.matrix.pos.y = ThePaths.nodes[node].y*0.125f;
			pVehicle->_.matrix.matrix.pos.z = ThePaths.nodes[node].z*0.125f + 4.0f;
			float x = pVehicle->_.matrix.matrix.pos.x;
			float y = pVehicle->_.matrix.matrix.pos.y;
			float z = pVehicle->_.matrix.matrix.pos.z;
			pVehicle->_.matrix.SetRotate(0.0f, 0.0f, 3.49f);
			pVehicle->_.matrix.matrix.pos.x += x;
			pVehicle->_.matrix.matrix.pos.y += y;
			pVehicle->_.matrix.matrix.pos.z += z;
			pVehicle->bfTypeStatus = pVehicle->bfTypeStatus & 7 | 0x20;
			CWorld__Add(pVehicle);
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

WRAPPER void CStreaming__RemoveBuildingsNotInArea(int) { EAXJMP(0x40DAA0); }
int &currArea = *(int*)0x978810;
void
areaChanged(void)
{
	CStreaming__RemoveBuildingsNotInArea(currArea);
}

struct CDirectory
{
	struct DirectoryInfo {
		uint32 offset;
		uint32 size;
		char name[24];
	};
	DirectoryInfo *entries;
	int32 maxFiles;
	int32 numFiles;
	bool FindItem(const char *name, uint32 *offset, uint32 *size);
};
WRAPPER bool CDirectory::FindItem(const char *name, uint32 *offset, uint32 *size) { EAXJMP(0x487220); }
CDirectory *&CStreaming__ms_pExtraObjectsDir = *(CDirectory**)0xA10730;

WRAPPER void ChangePlayerModel(const char *name) { EAXJMP(0x4AE8C0); }
const char *playermodels[] = {
	"player", "player2", "player3", "player4", "player5", "player6", "player7",
	"player8", "player9", "play10", "play11", "play12",
	"igken", "igcolon",	// friends
	"igbuddy", "igbudy2", "igbudy3",	// lance
	"igjezz", "igdick", "igpercy",	// love fist
	"igphil", "ighlary", "igmike",	// heist
	"igmerc", "igmerc2",	// mercedes
	"igcandy",
	"igsonny", "igdiaz", "iggonz",	// enemies

	"mba", "mbb",	// sonny goons
	"pga", "pgb",	// player goons
	"dgoona", "dgoonb",	// CS player/diaz goons
	"cla", "clb",	// diaz goons
	"cba", "cbb",	// cubans
	"hna", "hnb",	// haitians
	"bka", "bkb",	// bikers
	"sga", "sgb",	// wanna be gangsters
	"gda",	// security
	"vice1", "vice2", "vice3", "vice4", "vice5", "vice6", "vice7",
	"csassa", "csassb", "csassc",	// assassins
};
const char *csplayermodels[] = {
	"sfrenda", "sfrendb",	// sonny's friends
	"csavery", "cscolo", "cskent", "csbigm", "csbj", 
	"csdeal", "csdirec", "csdlove", "csdoris", "csdwayn", "csjetro",
	"csgonz", "cskelly", "csrich", "csumbto"
};
int playermodel;
int csplayermodel;
void
changePlayerModel(void)
{
	uint32 foo;
	// ChangePlayerModel only works with extra objects
	if(CStreaming__ms_pExtraObjectsDir->FindItem(playermodels[playermodel], &foo, &foo))
		ChangePlayerModel(playermodels[playermodel]);
	else{
		// ... so if it's not an extra object, the easiest way is to make it one temporarily
		// and hope that everything goes well
		int modelindex;
		CBaseModelInfo *mi = CModelInfo::GetModelInfo(playermodels[playermodel], &modelindex);
		if(mi){
			CDirectory::DirectoryInfo direntry;
			direntry.offset = CStreaming__ms_aInfoForModel[modelindex].position;
			direntry.size = CStreaming__ms_aInfoForModel[modelindex].size;
			strcpy(direntry.name, playermodels[playermodel]);

			assert(CStreaming__ms_pExtraObjectsDir->numFiles < CStreaming__ms_pExtraObjectsDir->maxFiles);
			CStreaming__ms_pExtraObjectsDir->entries[CStreaming__ms_pExtraObjectsDir->numFiles++] = direntry;
			ChangePlayerModel(playermodels[playermodel]);
			CStreaming__ms_pExtraObjectsDir->numFiles--;
		}
	}
}
void
changeCsPlayerModel(void)
{
	uint32 foo;
	if(CStreaming__ms_pExtraObjectsDir->FindItem(playermodels[playermodel], &foo, &foo))
		ChangePlayerModel(csplayermodels[csplayermodel]);
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
		static const char *areaNames[] = {
			"Outside", "Hotel", "Mansion", "Bank", "Mall", "Strip club",
			"Lawyer", "Coffee shop", "Concert hall", "Studio", "Rifle range",
			"Biker bar", "Police station", "Everywhere", "Dirt", "Blood", "Oval ring",
			"Malibu", "Print works"
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
		DebugMenuAddVar("Time & Weather", "Current Area", &currArea, areaChanged, 1, 0, 18, areaNames);

		InterceptVmethod(&CPlayerPed::ProcessControl, &CPlayerPed::ProcessControl_hooked, 0x694D90);
		DebugMenuAddVarBool32("Player", "Invincible", &isPlayerInvincible, nil);
		DebugMenuAddCmd("Player", "Clear Wanted Level", [](){ setWantedLevel(0); });
		DebugMenuAddCmd("Player", "Increase Wanted Level", [](){ setWantedLevel(getWantedLevel()+2); });
		e = DebugMenuAddInt32("Player", "Player model", &playermodel, changePlayerModel, 1, 0, nelem(playermodels)-1, playermodels);
		DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddInt32("Player", "Player model (CS)", &csplayermodel, changeCsPlayerModel, 1, 0, nelem(csplayermodels)-1, csplayermodels);
		DebugMenuEntrySetWrap(e, true);
		DebugMenuAddCmd("Player", "Switch to Cutscene model", [](){ ChangePlayerModel("CSplay"); });

		DebugMenuAddCmd("Cheats", "Weapons 1", [](){ ((void (*)(void))0x4AEE00)(); });
		DebugMenuAddCmd("Cheats", "Weapons 2", [](){ ((void (*)(void))0x4AEC70)(); });
		DebugMenuAddCmd("Cheats", "Weapons 3", [](){ ((void (*)(void))0x4AEAD0)(); });

		static int vehId = 130;
		DebugMenuAddVar("Spawn", "Spawn Vehicle ID", &vehId, nil, 1, 130, 236, vehnames);
		static uchar dummy;
		carCol1 = DebugMenuAddVar("Spawn", "First colour", &dummy, nil, 1, 0, 255, nil);
		carCol2 = DebugMenuAddVar("Spawn", "Second colour", &dummy, nil, 1, 0, 255, nil);
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
