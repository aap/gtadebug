#include "debug.h"

GlobalScene &Scene = *(GlobalScene*)0xC17038;
CCamera &TheCamera = *(CCamera*)0xB6F028;

WRAPPER double CGeneral::GetATanOfXY(float x, float y) { EAXJMP(0x53CC70); }

WRAPPER CVehicle *FindPlayerVehicle(int, bool) { EAXJMP(0x56E0D0); }
WRAPPER CPlayerPed *FindPlayerPed(int) { EAXJMP(0x56E210); }
WRAPPER void CPlayerPed::SetWantedLevel(int n) { EAXJMP(0x609F10); }

WRAPPER void CIplStore::AddIplsNeededAtPosn(CVector *pos) { EAXJMP(0x4045B0); }

WRAPPER void CDraw::SetFOV(float fov) { EAXJMP(0x6FF410); }


float &CTimer::ms_fTimeScale = *(float*)0xB7CB64;

int16 &CClock::ms_nGameClockSeconds = *(short*)0xB70150;
uint8 &CClock::ms_nGameClockMinutes = *(byte*)0xB70152;
uint8 &CClock::ms_nGameClockHours = *(byte*)0xB70153;
int16 &CWeather::OldWeatherType = *(short*)0xC81320;
int16 &CWeather::NewWeatherType = *(short*)0xC8131C;
float &CWeather::InterpolationValue = *(float*)0xC8130C;

float &CPostEffects::m_fNightVisionSwitchOnFXCount = *(float*)0xC40300;
int &CPostEffects::m_InfraredVisionGrainStrength = *(int*)0x8D50B4;
int &CPostEffects::m_NightVisionGrainStrength = *(int*)0x8D50A8;
bool &CPostEffects::m_bInfraredVision = *(bool*)0xC402B9;

bool &CPostEffects::m_bDisableAllPostEffect = *(bool*)0xC402CF;

bool &CPostEffects::m_bColorEnable = *(bool*)0x8D518C;
int &CPostEffects::m_colourLeftUOffset = *(int*)0x8D5150;
int &CPostEffects::m_colourRightUOffset = *(int*)0x8D5154;
int &CPostEffects::m_colourTopVOffset = *(int*)0x8D5158;
int &CPostEffects::m_colourBottomVOffset = *(int*)0x8D515C;
float &CPostEffects::m_colour1Multiplier = *(float*)0x8D5160;
float &CPostEffects::m_colour2Multiplier = *(float*)0x8D5164;
float &CPostEffects::SCREEN_EXTRA_MULT_CHANGE_RATE = *(float*)0x8D5168;
float &CPostEffects::SCREEN_EXTRA_MULT_BASE_CAP = *(float*)0x8D516C;
float &CPostEffects::SCREEN_EXTRA_MULT_BASE_MULT = *(float*)0x8D5170;

bool &CPostEffects::m_bRadiosity = *(bool*)0xC402CC;
bool &CPostEffects::m_bRadiosityDebug = *(bool*)0xC402CD;
int &CPostEffects::m_RadiosityFilterPasses = *(int*)0x8D510C;
int &CPostEffects::m_RadiosityRenderPasses = *(int*)0x8D5110;
int &CPostEffects::m_RadiosityIntensityLimit = *(int*)0x8D5114;
int &CPostEffects::m_RadiosityIntensity = *(int*)0x8D5118;
bool &CPostEffects::m_bRadiosityBypassTimeCycleIntensityLimit = *(bool*)0xC402CE;
int &CPostEffects::m_RadiosityFilterUCorrection = *(int*)0x8D511C;
int &CPostEffects::m_RadiosityFilterVCorrection = *(int*)0x8D5120;

bool &CPostEffects::m_bDarknessFilter = *(bool*)0xC402C4;
int &CPostEffects::m_DarknessFilterAlpha = *(int*)0x8D5204;
int &CPostEffects::m_DarknessFilterAlphaDefault = *(int*)0x8D50F4;
int &CPostEffects::m_DarknessFilterRadiosityIntensityLimit = *(int*)0x8D50F8;

bool &CPostEffects::m_bCCTV = *(bool*)0xC402C5;
bool &CPostEffects::m_bFog = *(bool*)0xC402C6;
bool &CPostEffects::m_bNightVision = *(bool*)0xC402B8;
bool &CPostEffects::m_bHeatHazeFX = *(bool*)0xC402BA;
bool &CPostEffects::m_bHeatHazeMaskModeTest = *(bool*)0xC402BB;
bool &CPostEffects::m_bGrainEnable = *(bool*)0xC402B4;
bool &CPostEffects::m_waterEnable = *(bool*)0xC402D3;

bool &CPostEffects::m_bSpeedFX = *(bool*)0x8D5100;
bool &CPostEffects::m_bSpeedFXTestMode = *(bool*)0xC402C7;
uint8 &CPostEffects::m_SpeedFXAlpha = *(uint8*)0x8D5104;


CMouseControllerState &CPad::NewMouseControllerState = *(CMouseControllerState*)0xB73418;
CMouseControllerState &CPad::OldMouseControllerState = *(CMouseControllerState*)0xB7342C;

WRAPPER CPad *CPad::GetPad(int id) { EAXJMP(0x53FB70); }

WRAPPER bool CControllerConfigManager::GetIsKeyboardKeyDown(RsKeyCodes key) { EAXJMP(0x52DDB0); }
WRAPPER bool CControllerConfigManager::GetIsKeyboardKeyJustDown(RsKeyCodes key) { EAXJMP(0x52E450); }



//
// RW
//

WRAPPER RwCamera *RwCameraSetNearClipPlane(RwCamera*, RwReal) { EAXJMP(0x7EE1D0); }