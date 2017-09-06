#include "debug.h"

// VC

static uintptr thisptr;
static uintptr baseptr;

WRAPPER void CCam::Process(void) { EAXJMP(0x48351A); }

static int toggleDebugCamSwitch;
static int toggleHudSwitch;

// inlined in VC
void
CCam::GetVectorsReadyForRW(void)
{
	CVector right;
	Up = CVector(0.0f, 0.0f, 1.0f);
	Front.Normalise();
	if(Front.x == 0.0f && Front.y == 0.0f){
		Front.x = 0.0001f;
		Front.y = 0.0001f;
	}
	CVector::CrossProduct(&right, &Front, &Up);
	right.Normalise();
	CVector::CrossProduct(&Up, &right, &Front);
}

enum Controlmode {
	CONTROL_CAMERA,
	CONTROL_PLAYER,
};
int controlMode = CONTROL_CAMERA;
int activatedFromKeyboard;

float gFOV = 70.0f;

void
CCamera::InitialiseCameraForDebugMode(void)
{
	CEntity *e;
	if(e = FindPlayerVehicle())
		this->Cams[2].Source = e->_.GetPosition();
	else if(e = FindPlayerPed())
		this->Cams[2].Source = e->_.GetPosition();
	this->Cams[2].Alpha = 0;
	this->Cams[2].Beta = 0;
	this->Cams[2].Mode = 6;
//	activatedFromKeyboard = keytemp;
//	CPad::m_bMapPadOneToPadTwo = 1;
}

void
CCam::Process_Debug(float*, float, float, float)
{
	static float speed = 0.0f;

	if(CTRLJUSTDOWN('C')){
		if(controlMode == CONTROL_CAMERA) controlMode = CONTROL_PLAYER;
		else if(controlMode == CONTROL_PLAYER) controlMode = CONTROL_CAMERA;
	}

	RwCameraSetNearClipPlane(Scene.camera, 0.9f);
	this->FOV = gFOV;
	this->Alpha += DEG2RAD(CPad::GetPad(1)->NewState.LEFTSTICKY*0.02f); // magic
	this->Beta  += CPad::GetPad(1)->NewState.LEFTSTICKX * 0.02617994f * 0.052631579f; // magic
	if(controlMode == CONTROL_CAMERA && CPad::NewMouseControllerState.lmb){
		this->Alpha += DEG2RAD(CPad::NewMouseControllerState.Y/2.0f);
		this->Beta += DEG2RAD(CPad::NewMouseControllerState.X/2.0f);
	}

	if(this->Alpha > DEG2RAD(89.5f)) this->Alpha = DEG2RAD(89.5f);
	if(this->Alpha < DEG2RAD(-89.5f)) this->Alpha = DEG2RAD(-89.5f);

	CVector vec;
	vec.x = this->Source.x + sin(this->Beta) * cos(this->Alpha) * 3.0f;
	vec.y = this->Source.y + cos(this->Beta) * cos(this->Alpha) * 3.0f;
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

	this->Front = vec - this->Source;
	this->Front.Normalise();
	this->Source = this->Source + this->Front*speed;
	if(this->Source.z < -450.0f)
		this->Source.z = -450.0f;

	// stay inside sectors
	while(this->Source.x * 0.02f + 48.0f > 75.0f)
		this->Source.x -= 1.0f;
	while(this->Source.x * 0.02f + 48.0f < 5.0f)
		this->Source.x += 1.0f;
	while(this->Source.y * 0.02f + 40.0f > 75.0f)
		this->Source.y -= 1.0f;
	while(this->Source.y * 0.02f + 40.0f < 5.0f)
		this->Source.y += 1.0f;

	CPad *pad = CPad::GetPad(1);
	if(JUSTDOWN(RIGHTSHOULDER2) ||
	   KEYJUSTDOWN(rsEXTENTER) && controlMode == CONTROL_CAMERA){
		if(FindPlayerVehicle()){
			CEntity *e = FindPlayerVehicle();
			CEntityVtbl *vmt = (CEntityVtbl*)e->vtable;
			vmt->Teleport(e, Source);
		}else if(FindPlayerPed()){
			CEntity *e = FindPlayerPed();
			e->_.SetPosition(this->Source);
		}
	}

	if(controlMode == CONTROL_CAMERA && activatedFromKeyboard)
		CPad::GetPad(0)->DisablePlayerControls = 1;

	GetVectorsReadyForRW();
}

static void __declspec(naked)
switchDebugHook(void)
{
	_asm{
		// standard code for most cases:
		lea	eax, [esp+40h]
		mov	ecx, ebp
		push	[esp+2ch]
		push	dword ptr [ebp+0D4h]
		push	[esp+38h]
		push	eax
		call	CCam::Process_Debug
		push	0x483E10
		retn
	}
}

static void
toggleCam(void)
{
	CCamera *cam = (CCamera*)thisptr;
	assert(cam == (CCamera*)0x7E4688);
	CPad *pad = CPad::GetPad(1);
	int keydown = CTRLJUSTDOWN('D') || toggleDebugCamSwitch;
	if(JUSTDOWN(CIRCLE) || keydown){
		toggleDebugCamSwitch = 0;
		cam->WorldViewerBeingUsed = !cam->WorldViewerBeingUsed;
		if(cam->WorldViewerBeingUsed){
			activatedFromKeyboard = keydown;
			cam->InitialiseCameraForDebugMode();
		}
	}
}

static void __declspec(naked)
toggleCamHook(void)
{
	_asm{
		mov	thisptr, ebx
		call	toggleCam

		push	ds:0x68ADC8
		push	0x46C723
		retn
	}
}

static void
processCam(void)
{
	CCamera *cam = (CCamera*)thisptr;
	if(cam->WorldViewerBeingUsed)
		cam->Cams[2].Process();
}

static void __declspec(naked)
processCamHook(void)
{
	_asm{
		pusha
		mov	thisptr, ebx
		sub	esp,108
		fsave	[esp]
		call	processCam
		frstor	[esp]
		add	esp,108
		popa

		movzx	eax, [ebx+0x76]	// active Cam
		imul	eax, 1CCh
		push	0x46C8DF
		retn
	}
}

static void
copyVectors(void)
{
	CCamera *cam = (CCamera*)thisptr;

	CVector *source = (CVector*)(baseptr - 0x174);
	CVector *front = (CVector*)(baseptr - 0x168);
	CVector *up = (CVector*)(baseptr - 0x15C);
	float *fov = (float*)(baseptr - 0x138);

	*source = cam->Cams[2].Source;
	*front = cam->Cams[2].Front;
	*up = cam->Cams[2].Up;
	*fov = cam->Cams[2].FOV;

	*(bool*)0x7038D1 = 0;
}

static void __declspec(naked)
copyVectorsHook(void)
{
	_asm{
		mov	al,[ebx + 0x75]	// WorldViewerBeingUsed
		test	al,al
		jz	nodebug

		mov	thisptr, ebx
		lea	eax, [esp+0x1d0]
		mov	baseptr, eax
		call	copyVectors
		push	0x46D73A
		retn

	nodebug:
		movzx	eax, [ebx+0x76]	// active Cam
		imul	eax, 1CCh
		lea	edi, [eax+ebx+2FCh]
		push	0x46C918
		retn
	}
}

static RwFrame*
copyToRw(void)
{
	CCamera *cam = (CCamera*)thisptr;
	RwCamera *rwcam = cam->m_pRwCamera;
	RwFrame *frm = RwCameraGetFrame(rwcam);
	RwMatrix *mat = RwFrameGetMatrix(frm);

	CVector right;
	CVector::CrossProduct(&right, &cam->Cams[2].Up, &cam->Cams[2].Front);
	cam->matrix.matrix.right = *(RwV3d*)&right;
	cam->matrix.matrix.up = *(RwV3d*)&cam->Cams[2].Front;
	cam->matrix.matrix.at = *(RwV3d*)&cam->Cams[2].Up;
	cam->matrix.matrix.pos = *(RwV3d*)&cam->Cams[2].Source;
	CDraw::SetFOV(cam->Cams[2].FOV);
	cam->m_vecGameCamPos = cam->Cams[2].Source;

	mat->pos = cam->matrix.matrix.pos;
	mat->at = cam->matrix.matrix.up;
	mat->up = cam->matrix.matrix.at;
	mat->right = cam->matrix.matrix.right;
	RwMatrixUpdate(mat);
	return frm;
}

static void __declspec(naked)
copyToRWHook(void)
{
	_asm{
		mov	al,[ebx + 0x75]	// WorldViewerBeingUsed
		test	al,al
		jz	nodebug

		pop	ecx	// clean up argument
		mov	thisptr, ebx
		sub	esp,108
		fsave	[esp]
		call	copyToRw
		frstor	[esp]
		add	esp,108
		mov	edi,eax
		push	0x46DD42
		retn

	nodebug:
		mov     eax, [ebx+808h]
		push	0x46DCD3
		retn
	}
}

static bool
toggleHUD(void)
{
	CPad *pad = CPad::GetPad(1);
	bool ret = JUSTDOWN(START) || (CTRLJUSTDOWN('H') && TheCamera.WorldViewerBeingUsed) || toggleHudSwitch;
	toggleHudSwitch = 0;
	return ret;
}

// Not strictly cam but whatever
void __declspec(naked)
togglehudhook(void)
{
	_asm{
		call	toggleHUD
		mov	dl,al
		push	0x55736C
		retn
	}
}

void
patchDebugCam(void)
{
	InjectHook(0x46C71D, toggleCamHook, PATCH_JUMP);
	InjectHook(0x46C8D5, processCamHook, PATCH_JUMP);
	InjectHook(0x46C911, copyVectorsHook, PATCH_JUMP);
	InjectHook(0x46DCCD, copyToRWHook, PATCH_JUMP);
	*(uintptr*)0x68b274 = (uintptr)switchDebugHook;

	InjectHook(0x55734B, togglehudhook, PATCH_JUMP);
	
	if(DebugMenuLoad()){
		DebugMenuEntry *e;
		static const char *controlStr[] = { "Camera", "Player" };
		DebugMenuAddCmd("Debug", "Toggle Debug Camera", [](){ toggleDebugCamSwitch = 1; });
		e = DebugMenuAddVar("Debug", "Debug Camera Control", &controlMode, nil, 1, CONTROL_CAMERA, CONTROL_PLAYER, controlStr);
		DebugMenuEntrySetWrap(e, true);
		DebugMenuAddVar("Debug", "Debug Camera FOV", &gFOV, nil, 1.0f, 5.0f, 180.0f);
		DebugMenuAddCmd("Debug", "Toggle HUD", [](){ toggleHudSwitch = 1; });
	}
}
