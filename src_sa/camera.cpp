#include "debug.h"

#define GINPUT_COMPILE_SA_VERSION
#include <GInputAPI.h>

// SA

static uintptr thisptr;
static uintptr baseptr;

WRAPPER void CCam::Process(void) { EAXJMP(0x526FC0); }
WRAPPER void CCam::GetVectorsReadyForRW(void) { EAXJMP(0x509CE0); }
WRAPPER void CCamera::CopyCameraMatrixToRWCam(bool) { EAXJMP(0x50AFA0); }

static int toggleDebugCamSwitch;
static int toggleDebugCam;

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
	if(e = FindPlayerVehicle(-1, true))
		this->Cams[2].Source = e->GetPosition();
	else if(e = FindPlayerPed(-1))
		this->Cams[2].Source = e->GetPosition();
	this->Cams[2].Alpha = 0;
	this->Cams[2].Beta = 0;
	this->Cams[2].Mode = 6;
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

void
CCam::Process_Debug(float*, float, float, float)
{
	static float speed = 0.0f;
	static float panspeedX = 0.0f;
	static float panspeedY = 0.0f;

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

	CVector vec;
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

	CPad *pad = CPad::GetPad(1);
	if(JUSTDOWN(RIGHTSHOULDER2) ||
	   KEYJUSTDOWN(rsEXTENTER) && controlMode == CONTROL_CAMERA){
		if(FindPlayerVehicle(-1, false)){
			CEntity *e = FindPlayerVehicle(-1, false);
			CEntity_VTBL *vmt = (CEntity_VTBL*)e->vtable;
			vmt->Teleport(e, this->Source.x, this->Source.y, this->Source.z, true);
		}else if(FindPlayerPed(-1)){
			CEntity *e = FindPlayerPed(-1);
			e->SetPosition(this->Source);
		}
	}

	if(controlMode == CONTROL_CAMERA && activatedFromKeyboard)
		CPad::GetPad(0)->DisablePlayerControls = 1;

	GetVectorsReadyForRW();
}

static void __declspec(naked)
switchDefaultHook(void)
{
	_asm{
		movsx	eax, word ptr [esi+0Ch]
		cmp	eax,6
		jne	def

		// standard code for most cases:
		mov     ecx, [esp+0x10]
		mov     edx, [esi+0D4h]
		mov     eax, [esp+0x14]
		push    ecx
		push    edx
		push    eax
		lea     ecx, [esp+0x2C]
		push    ecx
		mov     ecx, esi
		call	CCam::Process_Debug
		push	0x527CF3
		retn

	def:
		mov	[esp+0x38], 0
		push	0x527C65
		retn
	}
}


static void
toggleCam(void)
{
	CCamera *cam = (CCamera*)thisptr;
	CPad *pad = CPad::GetPad(1);
	int keydown = CTRLJUSTDOWN('B') || toggleDebugCam;
	if(JUSTDOWN(CIRCLE) || keydown){
		toggleDebugCam = 0;
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
		mov	thisptr, esi
		call	toggleCam

		mov	ecx, ds:0xC1703C	// Scene.camera
		push	0x52B893
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
		mov	thisptr, esi
		call	processCam

		movzx	ecx, [esi+0x59]	// active Cam
		imul	ecx, 238h
		push	0x52BA10
		retn
	}
}

static void
copyVectors(void)
{
	CCamera *cam = (CCamera*)thisptr;
	CVector *source = (CVector*)(baseptr - 0x80);
	CVector *front = (CVector*)(baseptr - 0x74);
	CVector *up = (CVector*)(baseptr - 0x8c);
	float *fov = (float*)(baseptr - 0x58);

	*source = cam->Cams[2].Source;
	*front = cam->Cams[2].Front;
	*up = cam->Cams[2].Up;
	*fov = cam->Cams[2].FOV;

	*(bool*)0xB70143 = 0;
}

static void __declspec(naked)
copyVectorsHook(void)
{
	_asm{
		mov	al,[esi + 0x56]	// WorldViewerBeingUsed
		test	al,al
		jz	nodebug

		mov	thisptr, esi
		lea	eax, [esp+0xb0]
		mov	baseptr, eax
		call	copyVectors
		push	0x52C53D
		retn

	nodebug:
		movzx	eax, [esi+0x59]	// active Cam
		imul	eax, 238h
		push	0x52C3E3
		retn
	}
}

static
void copyToRw(void)
{
	CCamera *cam = (CCamera*)thisptr;

	CVector right;
	right = CrossProduct(cam->Cams[2].Up, cam->Cams[2].Front);
	cam->m_pCoords->matrix.right = *(RwV3d*)&right;
	cam->m_pCoords->matrix.up = *(RwV3d*)&cam->Cams[2].Front;
	cam->m_pCoords->matrix.at = *(RwV3d*)&cam->Cams[2].Up;
	cam->m_pCoords->matrix.pos = *(RwV3d*)&cam->Cams[2].Source;
	CDraw::SetFOV(cam->Cams[2].FOV);
	cam->m_vecGameCamPos = cam->Cams[2].Source;
	cam->CopyCameraMatrixToRWCam(false);
	CIplStore::AddIplsNeededAtPosn(&cam->m_vecGameCamPos);
}

static void __declspec(naked)
copyToRWHook(void)
{
	_asm{
		mov	al,[esi + 0x56]	// WorldViewerBeingUsed
		test	al,al
		jz	nodebug

		pop	ebp	// remove argument to thiscall
		mov	thisptr, esi
		call	copyToRw
		mov	ecx, esi
		push	0x52C9B6
		retn

	nodebug:
		mov	eax,0x50AFA0
		call	eax
		push	0x52C990
		retn
	}
}


// TEMP TEMP TEMP
IGInputPad *ginput;
void
switchPad(void)
{
	ginput->SendEvent(GINPUT_EVENT_FORCE_MAP_PAD_ONE_TO_PAD_TWO, (void*)TRUE);
}

void
patchDebugCam(void)
{
	InjectHook(0x52B88D, toggleCamHook, PATCH_JUMP);
	InjectHook(0x52BA06, processCamHook, PATCH_JUMP);
	InjectHook(0x52C3D9, copyVectorsHook, PATCH_JUMP);
	InjectHook(0x52C98B, copyToRWHook, PATCH_JUMP);
	InjectHook(0x527C5D, switchDefaultHook, PATCH_JUMP);

	if(DebugMenuLoad()){
		DebugMenuEntry *e;
		static const char *controlStr[] = { "Camera", "Player" };
		DebugMenuAddCmd("Debug", "Toggle Debug Camera", [](){ toggleDebugCam = 1; });
		e = DebugMenuAddVar("Debug", "Debug Camera Control", &controlMode, nil, 1, CONTROL_CAMERA, CONTROL_PLAYER, controlStr);
		DebugMenuEntrySetWrap(e, true);
		DebugMenuAddVar("Debug", "FOV", &gFOV, nil, 1.0f, 5.0f, 180.0f);
		DebugMenuAddCmd("Debug", "Save Camera Position", [](){ SaveCam(&TheCamera.Cams[2]); });
		DebugMenuAddCmd("Debug", "Cycle Next", [](){ NextSavedCam(&TheCamera.Cams[2]); });
		DebugMenuAddCmd("Debug", "Cycle Prev", [](){ PrevSavedCam(&TheCamera.Cams[2]); });
		DebugMenuAddCmd("Debug", "Delete Camera Positions", DeleteSavedCams);

// TEMP TEMP TEMP
		if(GInput_Load(&ginput))
			DebugMenuAddCmd("Debug", "Switch Pad 1", switchPad);


		DebugMenuAddVarBool8("Debug", "Black bars", (int8*)0xB6F065, nil);
	}
}
