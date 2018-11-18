#include "debug.h"

#define GINPUT_COMPILE_III_VERSION
#include <GInputAPI.h>

// III

static int toggleDebugCamSwitch;
static int toggleHudSwitch;

float gFOV = 80.0f;
bool &CHud__m_Wants_To_Draw_Hud = *(bool*)0x95CD89;

WRAPPER void CCam::GetVectorsReadyForRW(void) { EAXJMP(0x4582F0); }

CControllerConfigManager *ctrldummy = nil;

enum Controlmode {
	CONTROL_CAMERA,
	CONTROL_PLAYER,
};
int controlMode = CONTROL_CAMERA;
bool keytemp;
bool activatedFromKeyboard;

float sensPos = 1.0f;
float sensRot = 1.0f;
int mode;

void
CCam::Process_Kalvin(float*, float, float, float)
{
	float frontspeed = 0.0;
	float rightspeed = 0.0;
	float upspeed = 0.0;
	CPad *pad = CPad::GetPad(1);

	if(JUSTDOWN(SELECT))
		mode = !mode;
	if(mode == 0){
		Process_Debug(nil, 0, 0, 0);
		return;
	}

	if(CTRLJUSTDOWN('C')){
		if(controlMode == CONTROL_CAMERA) controlMode = CONTROL_PLAYER;
		else if(controlMode == CONTROL_PLAYER) controlMode = CONTROL_CAMERA;
	}

	if(KEYJUSTDOWN('Z') && controlMode == CONTROL_CAMERA)
		SaveCam(this);
	if(KEYJUSTDOWN('X') && controlMode == CONTROL_CAMERA)
		DeleteSavedCams();
	if(KEYJUSTDOWN('Q') && controlMode == CONTROL_CAMERA)
		PrevSavedCam(this);
	if(KEYJUSTDOWN('E') && controlMode == CONTROL_CAMERA)
		NextSavedCam(this);

	RwCameraSetNearClipPlane(Scene.camera, 0.1f);
	this->FOV = gFOV;

	this->Alpha += DEG2RAD(pad->NewState.LEFTSTICKY/64.0f) * sensRot;
	this->Beta  -= DEG2RAD(pad->NewState.LEFTSTICKX/64.0f) * sensRot;
	if(KEYDOWN((RsKeyCodes)'I') && controlMode == CONTROL_CAMERA)
		this->Alpha += DEG2RAD(sensRot);
	if(KEYDOWN((RsKeyCodes)'K') && controlMode == CONTROL_CAMERA)
		this->Alpha -= DEG2RAD(sensRot);
	if(KEYDOWN((RsKeyCodes)'J') && controlMode == CONTROL_CAMERA)
		this->Beta += DEG2RAD(sensRot);
	if(KEYDOWN((RsKeyCodes)'L') && controlMode == CONTROL_CAMERA)
		this->Beta -= DEG2RAD(sensRot);

	if(this->Alpha > DEG2RAD(89.5f)) this->Alpha = DEG2RAD(89.5f);
	if(this->Alpha < DEG2RAD(-89.5f)) this->Alpha = DEG2RAD(-89.5f);

	CVector vec;
	vec.x = this->Source.x + cos(this->Beta) * cos(this->Alpha) * 3.0f;
	vec.y = this->Source.y + sin(this->Beta) * cos(this->Alpha) * 3.0f;
	vec.z = this->Source.z + sin(this->Alpha) * 3.0f;

	frontspeed -= pad->NewState.RIGHTSTICKY/128.0f * sensPos;
	rightspeed += pad->NewState.RIGHTSTICKX/128.0f * sensPos;

	if(KEYDOWN((RsKeyCodes)'W') && controlMode == CONTROL_CAMERA)
		frontspeed += sensPos;
	if(KEYDOWN((RsKeyCodes)'S') && controlMode == CONTROL_CAMERA)
		frontspeed -= sensPos;
	if(KEYDOWN((RsKeyCodes)'A') && controlMode == CONTROL_CAMERA)
		rightspeed -= sensPos;
	if(KEYDOWN((RsKeyCodes)'D') && controlMode == CONTROL_CAMERA)
		rightspeed += sensPos;
	if(pad->NewState.SQUARE)
		upspeed += sensPos;
	if(pad->NewState.CROSS)
		upspeed -= sensPos;

	this->Front = vec - this->Source;
	this->Front.Normalise();
	this->Source = this->Source + this->Front*frontspeed;

	CVector up = { 0.0f, 0.0f, 1.0f };
	CVector right;
	right = CrossProduct(Front, up);
	up = CrossProduct(right, Front);
	Source = Source + up*upspeed + right*rightspeed;

	if(this->Source.z < -450.0f)
		this->Source.z = -450.0f;

	// stay inside sectors
	while(this->Source.x * 0.025f + 50.0f > 95.0f)
		this->Source.x -= 1.0f;
	while(this->Source.x * 0.025f + 50.0f < 5.0f)
		this->Source.x += 1.0f;
	while(this->Source.y * 0.025f + 50.0f > 95.0f)
		this->Source.y -= 1.0f;
	while(this->Source.y * 0.025f + 50.0f < 5.0f)
		this->Source.y += 1.0f;

	if(JUSTDOWN(RIGHTSHOULDER2) ||
	   KEYJUSTDOWN(rsEXTENTER) && controlMode == CONTROL_CAMERA){
		if(FindPlayerVehicle()){
			CEntity *e = FindPlayerVehicle();
			CEntityVMT *vmt = (CEntityVMT*)e->vtable;
			vmt->Teleport(e, this->Source.x, this->Source.y, this->Source.z);
		}else if(FindPlayerPed()){
			CEntity *e = FindPlayerPed();
			e->matrix.matrix.pos.x = this->Source.x;
			e->matrix.matrix.pos.y = this->Source.y;
			e->matrix.matrix.pos.z = this->Source.z;
		}
	}

	if(controlMode == CONTROL_CAMERA && activatedFromKeyboard)
		CPad::GetPad(0)->DisablePlayerControls = 1;

	GetVectorsReadyForRW();
}

void
CCam::Process_Debug(float*, float, float, float)
{
	static float speed = 0.0f;
	static float panspeedX = 0.0f;
	static float panspeedY = 0.0f;
	CVector vec;

	if(CTRLJUSTDOWN('C')){
		if(controlMode == CONTROL_CAMERA) controlMode = CONTROL_PLAYER;
		else if(controlMode == CONTROL_PLAYER) controlMode = CONTROL_CAMERA;
	}

	if(KEYJUSTDOWN('Z') && controlMode == CONTROL_CAMERA)
		SaveCam(this);
	if(KEYJUSTDOWN('X') && controlMode == CONTROL_CAMERA)
		DeleteSavedCams();
	if(KEYJUSTDOWN('Q') && controlMode == CONTROL_CAMERA)
		PrevSavedCam(this);
	if(KEYJUSTDOWN('E') && controlMode == CONTROL_CAMERA)
		NextSavedCam(this);

	RwCameraSetNearClipPlane(Scene.camera, 0.9f);
	this->FOV = gFOV;
	this->Alpha += DEG2RAD(CPad::GetPad(1)->NewState.LEFTSTICKY*0.02f); // magic
	this->Beta  -= CPad::GetPad(1)->NewState.LEFTSTICKX * 0.02617994f * 0.052631579f; // magic
	if(controlMode == CONTROL_CAMERA && CPad::NewMouseControllerState.lmb){
		this->Alpha += DEG2RAD(CPad::NewMouseControllerState.Y/2.0f);
		this->Beta -= DEG2RAD(CPad::NewMouseControllerState.X/2.0f);
	}

	if(this->Alpha > DEG2RAD(89.5f)) this->Alpha = DEG2RAD(89.5f);
	if(this->Alpha < DEG2RAD(-89.5f)) this->Alpha = DEG2RAD(-89.5f);

	vec.x = this->Source.x + cos(this->Beta) * cos(this->Alpha) * 3.0f;
	vec.y = this->Source.y + sin(this->Beta) * cos(this->Alpha) * 3.0f;
	vec.z = this->Source.z + sin(this->Alpha) * 3.0f;

	if(CPad::GetPad(1)->NewState.SQUARE ||
	   KEYDOWN((RsKeyCodes)'W') && controlMode == CONTROL_CAMERA)
		speed += 0.1f;
	else if(CPad::GetPad(1)->NewState.CROSS ||
	        KEYDOWN((RsKeyCodes)'S') && controlMode == CONTROL_CAMERA)
		speed -= 0.1f;
	else
		speed = 0.0f;
	if(speed > 70.0f) speed = 70.0f;
	if(speed < -70.0f) speed = -70.0f;

	if((KEYDOWN((RsKeyCodes)rsRIGHT) || KEYDOWN((RsKeyCodes)'D')) && controlMode == CONTROL_CAMERA)
		panspeedX += 0.1f;
	else if((KEYDOWN((RsKeyCodes)rsLEFT) || KEYDOWN((RsKeyCodes)'A')) && controlMode == CONTROL_CAMERA)
		panspeedX -= 0.1f;
	else
		panspeedX = 0.0f;
	if(panspeedX > 70.0f) panspeedX = 70.0f;
	if(panspeedX < -70.0f) panspeedX = -70.0f;

	if(KEYDOWN((RsKeyCodes)rsUP) && controlMode == CONTROL_CAMERA)
		panspeedY += 0.1f;
	else if(KEYDOWN((RsKeyCodes)rsDOWN) && controlMode == CONTROL_CAMERA)
		panspeedY -= 0.1f;
	else
		panspeedY = 0.0f;
	if(panspeedY > 70.0f) panspeedY = 70.0f;
	if(panspeedY < -70.0f) panspeedY = -70.0f;


	this->Front = vec - this->Source;
	this->Front.Normalise();
	this->Source = this->Source + this->Front*speed;

	CVector up = { 0.0f, 0.0f, 1.0f };
	CVector right;
	right = CrossProduct(Front, up);
	up = CrossProduct(right, Front);
	Source = Source + up*panspeedY + right*panspeedX;

	if(this->Source.z < -450.0f)
		this->Source.z = -450.0f;

	// stay inside sectors
	while(this->Source.x * 0.025f + 50.0f > 95.0f)
		this->Source.x -= 1.0f;
	while(this->Source.x * 0.025f + 50.0f < 5.0f)
		this->Source.x += 1.0f;
	while(this->Source.y * 0.025f + 50.0f > 95.0f)
		this->Source.y -= 1.0f;
	while(this->Source.y * 0.025f + 50.0f < 5.0f)
		this->Source.y += 1.0f;
	this->GetVectorsReadyForRW();

	CPad *pad = CPad::GetPad(1);
	if(JUSTDOWN(RIGHTSHOULDER2) ||
	   KEYJUSTDOWN(rsEXTENTER) && controlMode == CONTROL_CAMERA){
		if(FindPlayerVehicle()){
			CEntity *e = FindPlayerVehicle();
			CEntityVMT *vmt = (CEntityVMT*)e->vtable;
			vmt->Teleport(e, this->Source.x, this->Source.y, this->Source.z);
		}else if(FindPlayerPed()){
			CEntity *e = FindPlayerPed();
			e->matrix.matrix.pos.x = this->Source.x;
			e->matrix.matrix.pos.y = this->Source.y;
			e->matrix.matrix.pos.z = this->Source.z;
		}
	}
	if(controlMode == CONTROL_CAMERA && activatedFromKeyboard)
		CPad::GetPad(0)->DisablePlayerControls = 1;
#if 0
	if(0 && CHud__m_Wants_To_Draw_Hud){
		static ushort unibuf[200];
		static char buf[200];
		float scx = RsGlobal.maximumWidth/640.0f;
		float scy = RsGlobal.maximumHeight/448.0f;
		CFont::SetBackgroundOff();
		CFont::SetScale(scx*0.7f, scy*0.7f);
		CFont::SetBackGroundOnlyTextOff();
//		sprintf(buf, "%f %f", CPad::NewMouseControllerState.X, CPad::NewMouseControllerState.Y);
//		sprintf(buf, "CamX: %.3f CamY: %.3f CamZ: %.3f", this->vecPos.x, this->vecPos.y, this->vecPos.z);
		sprintf(buf, "%d %d", controlMode, activatedFromKeyboard);
		AsciiToUnicode(buf, unibuf);
		CFont::SetJustifyOff();
		CFont::SetPropOn();
		CFont::SetFontStyle(CFont::REGULAR);
		CFont::SetColor(&CRGBA(255, 255, 255, 255));
		CFont::PrintString(scx*10, scy*400, unibuf);
	}
#endif
}

void
CCamera::InitialiseCameraForDebugMode(void)
{
	CEntity *e;
	if(e = FindPlayerVehicle()){
		this->Cams[2].Source.x = e->matrix.matrix.pos.x;
		this->Cams[2].Source.y = e->matrix.matrix.pos.y;
		this->Cams[2].Source.z = e->matrix.matrix.pos.z;
	}else if(e = FindPlayerPed()){
		this->Cams[2].Source.x = e->matrix.matrix.pos.x;
		this->Cams[2].Source.y = e->matrix.matrix.pos.y;
		this->Cams[2].Source.z = e->matrix.matrix.pos.z;
	}
	this->Cams[2].Alpha = 0;
	this->Cams[2].Beta = 0;
	this->Cams[2].Mode = 6;
	activatedFromKeyboard = keytemp;
//	CPad::m_bMapPadOneToPadTwo = 1;

	Cams[2].Source = Cams[ActiveCam].Source;
	CVector nfront = Cams[ActiveCam].Front;
	float groundDist = sqrt(nfront.x*nfront.x + nfront.y*nfront.y);
	Cams[2].Beta = CGeneral::GetATanOfXY(nfront.x, nfront.y);
	Cams[2].Alpha = CGeneral::GetATanOfXY(groundDist, nfront.z);
	while(Cams[2].Beta >= PI) Cams[2].Beta -= 2.0f*PI;
	while(Cams[2].Beta < -PI) Cams[2].Beta += 2.0f*PI;
	while(Cams[2].Alpha >= PI) Cams[2].Alpha -= 2.0f*PI;
	while(Cams[2].Alpha < -PI) Cams[2].Alpha += 2.0f*PI;

	LoadSavedCams();
}

bool
enableDebugCam(void)
{
	CPad *pad = CPad::GetPad(1);
	keytemp = CTRLJUSTDOWN('B') || toggleDebugCamSwitch;
	toggleDebugCamSwitch = 0;
	return JUSTDOWN(CIRCLE) || keytemp;
}

bool
toggleHUD(void)
{
	CPad *pad = CPad::GetPad(1);
	bool ret = JUSTDOWN(START) || (CTRLJUSTDOWN('H') && TheCamera.WorldViewerBeingUsed) || toggleHudSwitch;
	toggleHudSwitch = 0;
	return ret;
}

bool
toggleWeather(void)
{
	CPad *pad = CPad::GetPad(1);
	return JUSTDOWN(RIGHTSHOCK) || (CTRLJUSTDOWN('W') && TheCamera.WorldViewerBeingUsed);
}

bool
advanceTime(void)
{
	CPad *pad = CPad::GetPad(1);
	return pad->NewState.RIGHTSHOULDER1 || (CTRLDOWN('T') && TheCamera.WorldViewerBeingUsed);
}

void __declspec(naked)
debugcamhook(void)
{
	_asm{
		call	enableDebugCam
		mov	cl,al
		push	0x46D553
		retn
	}
}

void __declspec(naked)
togglehudhook(void)
{
	_asm{
		call	toggleHUD
		mov	cl,al
		push	0x5052CB
		retn
	}
}

void __declspec(naked)
toggleweatherhook(void)
{
	_asm{
		call	toggleWeather
		mov	cl,al
		push	0x522CD7
		retn
	}
}

void __declspec(naked)
advancetimehook(void)
{
	_asm{
		call	advanceTime
		test	al,al
		jz	notpressed
		push	0x47346F
		retn
	notpressed:
		push	0x4734B0
		retn
	}
}

IGInputPad *ginput;
bool padswitch;
void
switchPad(void)
{
	ginput->SendEvent(GINPUT_EVENT_FORCE_MAP_PAD_ONE_TO_PAD_TWO, (void*)padswitch);
}

void
debughooks(void)
{
//	InjectHook(0x459ABA, &CCam::Process_Debug);
	InjectHook(0x459ABA, &CCam::Process_Kalvin);
	InjectHook(0x46D56B, &CCamera::InitialiseCameraForDebugMode);
	InjectHook(0x46D532, debugcamhook, PATCH_JUMP);
	InjectHook(0x5052AA, togglehudhook, PATCH_JUMP);
	InjectHook(0x522CB6, toggleweatherhook, PATCH_JUMP);
	InjectHook(0x473460, advancetimehook, PATCH_JUMP);

	if(DebugMenuLoad()){
		DebugMenuEntry *e;
		static const char *controlStr[] = { "Camera", "Player" };
		DebugMenuAddCmd("Debug", "Toggle Debug Camera", [](){ toggleDebugCamSwitch = 1; });
		e = DebugMenuAddVar("Debug", "Debug Camera Control", &controlMode, nil, 1, CONTROL_CAMERA, CONTROL_PLAYER, controlStr);
		DebugMenuEntrySetWrap(e, true);
		DebugMenuAddVar("Debug", "Debug Camera FOV", &gFOV, nil, 1.0f, 5.0f, 180.0f);
		DebugMenuAddCmd("Debug", "Save Camera Position", [](){ SaveCam(&TheCamera.Cams[2]); });
		DebugMenuAddCmd("Debug", "Cycle Next", [](){ NextSavedCam(&TheCamera.Cams[2]); });
		DebugMenuAddCmd("Debug", "Cycle Prev", [](){ PrevSavedCam(&TheCamera.Cams[2]); });
		DebugMenuAddCmd("Debug", "Delete Camera Positions", DeleteSavedCams);

		DebugMenuAddCmd("Debug", "Toggle HUD", [](){ toggleHudSwitch = 1; });

		if(GInput_Load(&ginput))
			DebugMenuAddVarBool8("Debug", "Map Pad 1 -> Pad 2", (int8_t*)&padswitch, switchPad);

		DebugMenuAddFloat32("Debug", "KALVINCAM sensitivity translate", &sensPos, nil, 0.1f, 0.0f, 100.0f);
		DebugMenuAddFloat32("Debug", "KALVINCAM sensitivity rotate", &sensRot, nil, 0.1f, 0.0f, 100.0f);
	}
}
