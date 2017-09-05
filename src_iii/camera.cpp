#include "debug.h"

#define FOV 80.0f
bool &CHud__m_Wants_To_Draw_Hud = *(bool*)0x95CD89;

WRAPPER void CCam::GetVectorsReadyForRW(void) { EAXJMP(0x4582F0); }

CControllerConfigManager *ctrldummy = nil;

enum Controlmode {
	CONTROL_CAMERA,
	CONTROL_PLAYER,
};
bool controlMode = CONTROL_CAMERA;
bool keytemp;
bool activatedFromKeyboard;

void
CCam::Process_Debug(float*, float, float, float)
{
	static float speed = 0.0f;
	CVector vec;

	if(CTRLJUSTDOWN('C')){
		if(controlMode == CONTROL_CAMERA) controlMode = CONTROL_PLAYER;
		else if(controlMode == CONTROL_PLAYER) controlMode = CONTROL_CAMERA;
	}


	RwCameraSetNearClipPlane(Scene.camera, 0.9f);
	this->fFOV = FOV;
	this->fAlpha += DEG2RAD(CPad::GetPad(1)->NewState.LEFTSTICKY*0.02f); // magic
	this->fBeta  += CPad::GetPad(1)->NewState.LEFTSTICKX * 0.02617994f * 0.052631579f; // magic
	if(controlMode == CONTROL_CAMERA && CPad::NewMouseControllerState.lmb){
		this->fAlpha += DEG2RAD(CPad::NewMouseControllerState.Y/2.0f);
		this->fBeta += DEG2RAD(CPad::NewMouseControllerState.X/2.0f);
	}

	if(this->fAlpha > DEG2RAD(89.5f)) this->fAlpha = DEG2RAD(89.5f);
	if(this->fAlpha < DEG2RAD(-89.5f)) this->fAlpha = DEG2RAD(-89.5f);

	vec.x = this->vecPos.x + sin(this->fBeta) * cos(this->fAlpha) * 3.0f;
	vec.y = this->vecPos.y + cos(this->fBeta) * cos(this->fAlpha) * 3.0f;
	vec.z = this->vecPos.z + sin(this->fAlpha) * 3.0f;

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

	this->vecFront = vec - this->vecPos;
	this->vecFront.Normalize();
	this->vecPos = this->vecPos + this->vecFront*speed;
	if(this->vecPos.z < -450.0f)
		this->vecPos.z = -450.0f;

	// stay inside sectors
	while(this->vecPos.x * 0.025f + 50.0f > 95.0f)
		this->vecPos.x -= 1.0f;
	while(this->vecPos.x * 0.025f + 50.0f < 5.0f)
		this->vecPos.x += 1.0f;
	while(this->vecPos.y * 0.025f + 50.0f > 95.0f)
		this->vecPos.y -= 1.0f;
	while(this->vecPos.y * 0.025f + 50.0f < 5.0f)
		this->vecPos.y += 1.0f;
	this->GetVectorsReadyForRW();

	CPad *pad = CPad::GetPad(1);
	if(JUSTDOWN(RIGHTSHOULDER2) ||
	   KEYJUSTDOWN(rsEXTENTER) && controlMode == CONTROL_CAMERA){
		if(FindPlayerVehicle()){
			CEntity *e = FindPlayerVehicle();
			CEntityVMT *vmt = (CEntityVMT*)e->vtable;
			vmt->Teleport(e, this->vecPos.x, this->vecPos.y, this->vecPos.z);
		}else if(FindPlayerPed()){
			CEntity *e = FindPlayerPed();
			e->matrix.matrix.pos.x = this->vecPos.x;
			e->matrix.matrix.pos.y = this->vecPos.y;
			e->matrix.matrix.pos.z = this->vecPos.z;
		}
	}
	if(controlMode == CONTROL_CAMERA && activatedFromKeyboard)
		CPad::GetPad(0)->DisablePlayerControls = 1;
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
}

void
CCamera::InitialiseCameraForDebugMode(void)
{
	CEntity *e;
	if(e = FindPlayerVehicle()){
		this->Cams[2].vecPos.x = e->matrix.matrix.pos.x;
		this->Cams[2].vecPos.y = e->matrix.matrix.pos.y;
		this->Cams[2].vecPos.z = e->matrix.matrix.pos.z;
	}else if(e = FindPlayerPed()){
		this->Cams[2].vecPos.x = e->matrix.matrix.pos.x;
		this->Cams[2].vecPos.y = e->matrix.matrix.pos.y;
		this->Cams[2].vecPos.z = e->matrix.matrix.pos.z;
	}
	this->Cams[2].fAlpha = 0;
	this->Cams[2].fBeta = 0;
	this->Cams[2].Mode = 6;
	activatedFromKeyboard = keytemp;
//	CPad::m_bMapPadOneToPadTwo = 1;
}

bool
enableDebugCam(void)
{
	CPad *pad = CPad::GetPad(1);
	keytemp = CTRLJUSTDOWN('D');
	return JUSTDOWN(CIRCLE) || keytemp;
}

bool
toggleHUD(void)
{
	CPad *pad = CPad::GetPad(1);
	return JUSTDOWN(START) || CTRLJUSTDOWN('H');
}

bool
toggleWeather(void)
{
	CPad *pad = CPad::GetPad(1);
	return JUSTDOWN(RIGHTSHOCK) || CTRLJUSTDOWN('W');
}

bool
advanceTime(void)
{
	CPad *pad = CPad::GetPad(1);
	return pad->NewState.RIGHTSHOULDER1 || CTRLDOWN('T');
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

float pedval1 = 0.8; //1.25f;
float pedval2 = 1.35; //1.6f;

void
debughooks(void)
{
	InjectHook(0x459ABA, &CCam::Process_Debug);
	InjectHook(0x46D56B, &CCamera::InitialiseCameraForDebugMode);
	InjectHook(0x46D532, debugcamhook, PATCH_JUMP);
	InjectHook(0x5052AA, togglehudhook, PATCH_JUMP);
	InjectHook(0x522CB6, toggleweatherhook, PATCH_JUMP);
	InjectHook(0x473460, advancetimehook, PATCH_JUMP);
}

void
camhooks(void)
{
// ped
// near = 1
// normal = 2
// far = 3
// topdown = 4

// car
// first person = 0
// near = 1
// normal = 2
// far = 3
// top down = 4
// cinematic = 5

	// default distance for car and ped
	Patch<float>(0x46BC61+6, 1.0f);
//	Patch<float>(0x46BC6B+6, 1.0f);

	// ped near dist, default 2.0905559 (mid 3.34973)
//	Patch<float>(0x45E85F+6, 3.34973f);
//	*(float*)0x5F0794 = 3.34973f;
//
//	Patch(0x45E513+2, &pedval1);
//	Patch(0x45E51D+2, &pedval2);

	// ped distances
//	Patch<float>(0x468FA0+6, 0.5f); // 0.25
//	Patch<float>(0x468FC2+6, 1.5f);
//	Patch<float>(0x468FE2+6, 2.9f);
//
//	// car distances
//	Patch<float>(0x468953+6, 0.05f);
//	Patch<float>(0x468972+6, 1.9f);
//	Patch<float>(0x468992+6, 3.9f);
}


/*
void
CCam::Process_Debug(float*, float, float, float)
{
	static float speed = 0.0f;
	CVector vec;

	RwCameraSetNearClipPlane(pRwCamera, 0.9f);
	this->fFOV = FOV;
	this->fAlpha += DEG2RAD(CPad::GetPad(1)->NewState.LEFTSTICKY*0.02f); // magic
	this->fBeta  += CPad::GetPad(1)->NewState.LEFTSTICKX * 0.02617994f * 0.052631579f; // magic
	if(CPad::NewMouseControllerState.lmb){
		this->fAlpha += DEG2RAD(CPad::NewMouseControllerState.Y/2.0f);
		this->fBeta += DEG2RAD(CPad::NewMouseControllerState.X/2.0f);
	}

	if(this->fAlpha > DEG2RAD(89.5f)) this->fAlpha = DEG2RAD(89.5f);
	if(this->fAlpha < DEG2RAD(-89.5f)) this->fAlpha = DEG2RAD(-89.5f);

	vec.x = this->vecPos.x + sin(this->fBeta) * cos(this->fAlpha) * 3.0f;
	vec.y = this->vecPos.y + cos(this->fBeta) * cos(this->fAlpha) * 3.0f;
	vec.z = this->vecPos.z + sin(this->fAlpha) * 3.0f;

	if(CPad::GetPad(1)->NewState.SQUARE || KEYDOWN((RsKeyCodes)'W'))
		speed += 0.1f;
	else if(CPad::GetPad(1)->NewState.CROSS || KEYDOWN((RsKeyCodes)'S'))
		speed -= 0.1f;
	else
		speed = 0.0f;
	if(speed > 70.0f) speed = 70.0f;
	if(speed < -70.0f) speed = -70.0f;

	this->vecFront = vec - this->vecPos;
	this->vecFront.Normalize();
	this->vecPos = this->vecPos + this->vecFront*speed;
	if(this->vecPos.z < -450.0f)
		this->vecPos.z = -450.0f;

	// stay inside sectors
	while(this->vecPos.x * 0.025f + 50.0f > 95.0f)
		this->vecPos.x -= 1.0f;
	while(this->vecPos.x * 0.025f + 50.0f < 5.0f)
		this->vecPos.x += 1.0f;
	while(this->vecPos.y * 0.025f + 50.0f > 95.0f)
		this->vecPos.y -= 1.0f;
	while(this->vecPos.y * 0.025f + 50.0f < 5.0f)
		this->vecPos.y += 1.0f;
	this->GetVectorsReadyForRW();

	CPad *pad = CPad::GetPad(1);
	if(JUSTDOWN(RIGHTSHOULDER2) || KEYJUSTDOWN(rsEXTENTER)){
		if(FindPlayerVehicle()){
			CEntity *e = FindPlayerVehicle();
			CEntityVMT *vmt = (CEntityVMT*)e->vtable;
			vmt->Teleport(e, this->vecPos.x, this->vecPos.y, this->vecPos.z);
		}else if(FindPlayerPed()){
			CEntity *e = FindPlayerPed();
			e->matrix.matrix.pos.x = this->vecPos.x;
			e->matrix.matrix.pos.y = this->vecPos.y;
			e->matrix.matrix.pos.z = this->vecPos.z;
		}
	}
	CPad::GetPad(0)->DisablePlayerControls = 1;
	if(0 && CHud__m_Wants_To_Draw_Hud){
		static ushort unibuf[200];
		static char buf[200];
		float scx = RsGlobal.maximumWidth/640.0f;
		float scy = RsGlobal.maximumHeight/448.0f;
		CFont::SetBackgroundOff();
		CFont::SetScale(scx*0.7f, scy*0.7f);
		CFont::SetBackGroundOnlyTextOff();
		sprintf(buf, "%f %f", CPad::NewMouseControllerState.X, CPad::NewMouseControllerState.Y);
//		sprintf(buf, "CamX: %.3f CamY: %.3f CamZ: %.3f", this->vecPos.x, this->vecPos.y, this->vecPos.z);
		AsciiToUnicode(buf, unibuf);
		CFont::SetJustifyOff();
		CFont::SetPropOn();
		CFont::SetFontStyle(CFont::REGULAR);
		CFont::SetColor(&CRGBA(255, 255, 255, 255));
		CFont::PrintString(scx*10, scy*400, unibuf);
	}
}

*/