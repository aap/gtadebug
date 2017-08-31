#include "debug.h"
#include "debugmenu_public.h"

DebugMenuAPI gDebugMenuAPI;

HMODULE dllModule;
int gtaversion = -1;

int (*RsEventHandler_orig)(int a, int b);


void
changePlayerModel(int id)
{
	CEntity *playerped;

	playerped = FindPlayerPed();
}

int
delayedPatches10(int a, int b)
{
	if(DebugMenuLoad()){
		static const char *weathers[] = {
			"Sunny", "Cloudy", "Rainy", "Foggy"
		};
		DebugMenuAddVar("Time & Weather", "Current Hour", (uint8*)&CClock::ms_nGameClockHours, nil, 1, 0, 23, nil);
		DebugMenuAddVar("Time & Weather", "Current Minute", (uint8*)&CClock::ms_nGameClockMinutes,
			[](){ CWeather::InterpolationValue = CClock::ms_nGameClockMinutes/60.0f; }, 1, 0, 59, nil);
		DebugMenuAddVar("Time & Weather", "Old Weather", (int16*)&CWeather::OldWeatherType, nil, 1, 0, 3, weathers);
		DebugMenuAddVar("Time & Weather", "New Weather", (int16*)&CWeather::NewWeatherType, nil, 1, 0, 3, weathers);
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
