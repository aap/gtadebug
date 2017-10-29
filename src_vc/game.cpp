#include "debug.h"

// GTA VC

GlobalScene &Scene = *(GlobalScene*)0x8100B8;
CCamera &TheCamera = *(CCamera*)0x7E4688;

WRAPPER CVehicle *FindPlayerVehicle(void) { EAXJMP(0x4BC1E0); }
WRAPPER CPlayerPed *FindPlayerPed(void) { EAXJMP(0x4BC120); }
WRAPPER void FindPlayerCoors(CVector*) { EAXJMP(0x4BC240); }

WRAPPER void CVector::CrossProduct(CVector *out, CVector *a, CVector *b) { EAXJMP(0x4E00B0); }
WRAPPER void CVector::Normalise(void) { EAXJMP(0x4DFEA0); }

WRAPPER void CMatrix::SetRotate(float x, float y, float z) { EAXJMP(0x4DF240); }

CStreamingInfo *CStreaming__ms_aInfoForModel;
WRAPPER void CStreaming__RequestModel(int id, int flags) { EAXJMP(0x40E310); }
WRAPPER void CStreaming__LoadAllRequestedModels(bool b) { EAXJMP(0x40B5F0); }
WRAPPER void CStreaming__SetModelIsDeletable(int id) { EAXJMP(0x40AEA0); }
WRAPPER void CStreaming__SetModelTxdIsDeletable(int id) { EAXJMP(0x40ADF0); }

WRAPPER bool CPed::IsPedInControl(void) { EAXJMP(0x501950); }
WRAPPER CVehicle *CVehicle__new(int size) { EAXJMP(0x5BAB20); }
WRAPPER void CWorld__Add(CEntity *e) { EAXJMP(0x4DB3F0); }

CPathFind &ThePaths = *(CPathFind*)0x9B6E5C;
WRAPPER int CPathFind::FindNodeClosestToCoors(float x, float y, float z, char a7, float f8, char a9, char a10, char a11, char a12) {  EAXJMP(0x437150); }


WRAPPER void CDraw::SetFOV(float fov) { EAXJMP(0x54A2E0); }


int &CTimer::m_snTimeInMilliseconds = *(int*)0x974B2C;

int8  &CClock::ms_nGameClockHours = *(int8*)0xA10B6B;
int8  &CClock::ms_nGameClockMinutes = *(int8*)0xA10B92;
int16 &CClock::ms_nGameClockSeconds = *(int16*)0xA10A3C;

float &CWeather::InterpolationValue = *(float*)0x9787D8;
int16 &CWeather::OldWeatherType = *(int16*)0xA10AAA;
int16 &CWeather::NewWeatherType = *(int16*)0xA10A2E;

CMouseControllerState &CPad::NewMouseControllerState = *(CMouseControllerState*)0x94D788;
CMouseControllerState &CPad::OldMouseControllerState = *(CMouseControllerState*)0x936908;

WRAPPER CPad *CPad::GetPad(int id) { EAXJMP(0x4AB060); }

WRAPPER bool CControllerConfigManager::GetIsKeyboardKeyDown(RsKeyCodes key) { EAXJMP(0x617E10); }
WRAPPER bool CControllerConfigManager::GetIsKeyboardKeyJustDown(RsKeyCodes key) { EAXJMP(0x617430); }

WRAPPER void CWanted::CheatWantedLevel(int level) { EAXJMP(0x4D1F80); }

WRAPPER CAutomobile *CAutomobile::ctor(int id, uint8 type) { EAXJMP(0x59E620); }

WRAPPER CBike *CBike::ctor(int id, uint8 createBy) { EAXJMP(0x615740); }

WRAPPER CBoat *CBoat::ctor(int id, uint8 createBy) { EAXJMP(0x5A6470); }

WRAPPER bool CModelInfo::IsBikeModel(int index) { EAXJMP(0x55F4E0); }
WRAPPER bool CModelInfo::IsBoatModel(int index) { EAXJMP(0x55F540); }
WRAPPER CBaseModelInfo *CModelInfo::GetModelInfo(const char *name, int *id) { EAXJMP(0x55F7D0); }

#if 0
//
// from SkyGfx:
//

static uint32_t RwTextureRead_A = AddressByVersion<uint32_t>(0x5A7580, 0x5A7840, 0x5A8E00, 0x64E110, 0, 0);
WRAPPER RwTexture *RwTextureRead(const RwChar*, const RwChar*) { VARJMP(RwTextureRead_A); }
static uint32_t RwTexDictionaryGetCurrent_A = AddressByVersion<uint32_t>(0x5A7570, 0x5A7830, 0x5A8DA0, 0x64E100, 0, 0);
WRAPPER RwTexDictionary *RwTexDictionaryGetCurrent(void) { VARJMP(RwTexDictionaryGetCurrent_A); }
static uint32_t RwTexDictionarySetCurrent_A = AddressByVersion<uint32_t>(0x5A7550, 0x5A7810, 0x5A8D80, 0, 0, 0);
WRAPPER RwTexDictionary *RwTexDictionarySetCurrent(RwTexDictionary * dict) { VARJMP(RwTexDictionarySetCurrent_A); }
static uint32_t RwTexDictionaryForAllTextures_A = AddressByVersion<uint32_t>(0, 0, 0, 0x64DE20, 0, 0);
WRAPPER const RwTexDictionary *RwTexDictionaryForAllTextures(const RwTexDictionary*, RwTextureCallBack, void*) { VARJMP(RwTexDictionaryForAllTextures_A); }

// ADDRESS
static uint32_t RwV3dLength_A = AddressByVersion<uint32_t>(0x5A36A0, 0, 0, 0x647030, 0, 0);
WRAPPER RwReal RwV3dLength(const RwV3d*) { VARJMP(RwV3dLength_A); }
WRAPPER RwV3d *RwV3dTransformPoints(RwV3d*, const RwV3d*, RwInt32, const RwMatrix*) { EAXJMP(0x5A37D0); }



static uint32_t RwMatrixCreate_A = AddressByVersion<uint32_t>(0x5A3330, 0x5A35F0, 0x5A3FA0, 0x644620, 0x644670, 0x6435D0);
WRAPPER RwMatrix *RwMatrixCreate(void) { VARJMP(RwMatrixCreate_A); }
static uint32_t RwMatrixDestroy_A = AddressByVersion<uint32_t>(0x5A3300, 0x5A35C0, 0x5A3F70, 0x6445F0, 0x644640, 0x6435A0);
WRAPPER RwBool RwMatrixDestroy(RwMatrix*) { VARJMP(RwMatrixDestroy_A); }
static uint32_t RwMatrixMultiply_A = AddressByVersion<uint32_t>(0x5A28F0, 0x5A2BB0, 0x5A2E10, 0x6437C0, 0x643810, 0x642770);
WRAPPER RwMatrix *RwMatrixMultiply(RwMatrix*, const RwMatrix*, const RwMatrix*) { VARJMP(RwMatrixMultiply_A); }
static uint32_t RwMatrixInvert_A = AddressByVersion<uint32_t>(0x5A2C90, 0x5A2F50, 0x5A35A0, 0x643F40, 0x643F90, 0x642EF0);
WRAPPER RwMatrix *RwMatrixInvert(RwMatrix*, const RwMatrix*) { VARJMP(RwMatrixInvert_A); }
#endif
static uint32_t RwMatrixUpdate_A = AddressByVersion<uint32_t>(0x5A28E0, 0x5A2BA0, 0x5A2DF0, 0x6437B0, 0x643800, 0x642760);
WRAPPER RwMatrix *RwMatrixUpdate(RwMatrix*) { VARJMP(RwMatrixUpdate_A); }
#if 0
static uint32_t RwMatrixRotate_A = AddressByVersion<uint32_t>(0x5A2BF0, 0x5A2EB0, 0x5A3510, 0x643EA0, 0x643EF0, 0x642E50); //TODO
WRAPPER RwMatrix *RwMatrixRotate(RwMatrix*, const RwV3d*, RwReal, RwOpCombineType) { VARJMP(RwMatrixRotate_A); }

static uint32_t RwFrameCreate_A = AddressByVersion<uint32_t>(0x5A1A00, 0x5A1CC0, 0x5A2270, 0x644AA0, 0x644AF0, 0x643A50);
WRAPPER RwFrame *RwFrameCreate(void) { VARJMP(RwFrameCreate_A); }
static uint32_t RwFrameUpdateObjects_A = AddressByVersion<uint32_t>(0x5A1C60, 0x5A1F20, 0x5A23B4, 0x644D00, 0x644D50, 0x643CB0);
WRAPPER RwFrame *RwFrameUpdateObjects(RwFrame* a1)
{
	if (gtaversion != III_STEAM)
		VARJMP(RwFrameUpdateObjects_A);

	__asm
	{
		mov eax, a1
		jmp RwFrameUpdateObjects_A
	}
}
static uint32_t RwFrameGetLTM_A = AddressByVersion<uint32_t>(0x5A1CE0, 0x5A1FA0, 0x5A2430, 0x644D80, 0x644DD0, 0x643D30);
WRAPPER RwMatrix *RwFrameGetLTM(RwFrame*) { VARJMP(RwFrameGetLTM_A); }
static uint32_t RwFrameTransform_A = AddressByVersion<uint32_t>(0x5A2140, 0x5A2400, 0x5A26E0, 0x6451E0, 0x645230, 0x644190);
WRAPPER RwFrame *RwFrameTransform(RwFrame*, const RwMatrix*, RwOpCombineType) { VARJMP(RwFrameTransform_A); }

static uint32_t RwCameraCreate_A = AddressByVersion<uint32_t>(0x5A5360, 0x5A5620, 0x5A74E0, 0x64AB50, 0x64ABA0, 0x649B00);
WRAPPER RwCamera *RwCameraCreate(void) { VARJMP(RwCameraCreate_A); }
static uint32_t RwCameraBeginUpdate_A = AddressByVersion<uint32_t>(0x5A5030, 0x5A52F0, 0x5A7220, 0x64A820, 0x64A870, 0x6497D0);
WRAPPER RwCamera *RwCameraBeginUpdate(RwCamera*) { VARJMP(RwCameraBeginUpdate_A); }
static uint32_t RwCameraEndUpdate_A = AddressByVersion<uint32_t>(0x5A5020, 0x5A52E0, 0x5A7200, 0x64A810, 0x64A860, 0x6497C0);
WRAPPER RwCamera *RwCameraEndUpdate(RwCamera*) { VARJMP(RwCameraEndUpdate_A); }
#endif
static uint32_t RwCameraSetNearClipPlane_A = AddressByVersion<uint32_t>(0x5A5070, 0x5A5330, 0x5A7270, 0x64A860, 0x64A8B0, 0x649810);
WRAPPER RwCamera *RwCameraSetNearClipPlane(RwCamera*, RwReal) { VARJMP(RwCameraSetNearClipPlane_A); }
#if 0
static uint32_t RwCameraSetFarClipPlane_A = AddressByVersion<uint32_t>(0x5A5140, 0x5A5400, 0x5A72A0, 0x64A930, 0x64A980, 0x6498E0);
WRAPPER RwCamera *RwCameraSetFarClipPlane(RwCamera*, RwReal) { VARJMP(RwCameraSetFarClipPlane_A); }
static uint32_t RwCameraSetViewWindow_A = AddressByVersion<uint32_t>(0x5A52B0, 0x5A5570, 0x5A7440, 0x64AAA0, 0x64AAF0, 0x649A50);
WRAPPER RwCamera *RwCameraSetViewWindow(RwCamera*, const RwV2d*) { VARJMP(RwCameraSetViewWindow_A); }
static uint32_t RwCameraClear_A = AddressByVersion<uint32_t>(0x5A51E0, 0x5A54A0, 0x5A7350, 0x64A9D0, 0x64AA20, 0x649980);
WRAPPER RwCamera *RwCameraClear(RwCamera*, RwRGBA*, RwInt32) { VARJMP(RwCameraClear_A); }

static uint32_t RwRasterCreate_A = AddressByVersion<uint32_t>(0x5AD930, 0x5ADBF0, 0x5B0580, 0x655490, 0x6554E0, 0x654440);
WRAPPER RwRaster *RwRasterCreate(RwInt32, RwInt32, RwInt32, RwInt32) { VARJMP(RwRasterCreate_A); }
static uint32_t RwRasterSetFromImage_A = AddressByVersion<uint32_t>(0x5BBF50, 0x5BC210, 0x5C0BF0, 0x6602B0, 0x660300, 0x65F260);
WRAPPER RwRaster *RwRasterSetFromImage(RwRaster*, RwImage*) { VARJMP(RwRasterSetFromImage_A); }
static uint32_t RwRasterPushContext_A = AddressByVersion<uint32_t>(0x5AD7C0, 0x5ADA80, 0x5B03C0, 0x655320, 0x655370, 0x6542D0);
WRAPPER RwRaster *RwRasterPushContext(RwRaster*) { VARJMP(RwRasterPushContext_A); }
static uint32_t RwRasterPopContext_A = AddressByVersion<uint32_t>(0x5AD870, 0x5ADB30, 0x5B0460, 0x6553D0, 0x655420, 0x654380);
WRAPPER RwRaster *RwRasterPopContext(void) { VARJMP(RwRasterPopContext_A); }
static uint32_t RwRasterRenderFast_A = AddressByVersion<uint32_t>(0x5AD710, 0x5AD9D0, 0x5B0800, 0x655270, 0x6552C0, 0x654220);
WRAPPER RwRaster *RwRasterRenderFast(RwRaster*, RwInt32, RwInt32) { VARJMP(RwRasterRenderFast_A); }

static uint32_t RwTextureCreate_A = AddressByVersion<uint32_t>(0x5A72D0, 0x5A7590, 0x5A8AC0, 0x64DE60, 0x64DEB0, 0x64CE10);
WRAPPER RwTexture *RwTextureCreate(RwRaster*) { VARJMP(RwTextureCreate_A); }

static uint32_t RwRenderStateGet_A = AddressByVersion<uint32_t>(0x5A4410, 0x5A46D0, 0x5A53B0, 0x649BF0, 0x649C40, 0x648BA0);
WRAPPER RwBool RwRenderStateGet(RwRenderState, void*) { VARJMP(RwRenderStateGet_A); }
static uint32_t RwRenderStateSet_A = AddressByVersion<uint32_t>(0x5A43C0, 0x5A4680, 0x5A5360, 0x649BA0, 0x649BF0, 0x648B50);
WRAPPER RwBool RwRenderStateSet(RwRenderState, void*) { VARJMP(RwRenderStateSet_A); }

static uint32_t RwD3D8SetTexture_A = AddressByVersion<uint32_t>(0x5B53A0, 0x5B5660, 0x5BA2D0, 0x659850, 0x6598A0, 0x658800);
WRAPPER RwBool RwD3D8SetTexture(RwTexture*, RwUInt32) { VARJMP(RwD3D8SetTexture_A); }
static uint32_t RwD3D8SetRenderState_A = AddressByVersion<uint32_t>(0x5B3CF0, 0x5B3FB0, 0x5B84B0, 0x6582A0, 0x6582F0, 0x657250);
WRAPPER RwBool RwD3D8SetRenderState(RwUInt32, RwUInt32) { VARJMP(RwD3D8SetRenderState_A); }
static uint32_t RwD3D8GetRenderState_A = AddressByVersion<uint32_t>(0x5B3D40, 0x5B4000, 0x5B8510, 0x6582F0, 0x658340, 0x6572A0);
WRAPPER void RwD3D8GetRenderState(RwUInt32, void*) { VARJMP(RwD3D8GetRenderState_A); }
static uint32_t RwD3D8SetTextureStageState_A = AddressByVersion<uint32_t>(0x5B3D60, 0x5B4020, 0x5B8530, 0x658310, 0x658360, 0x6572C0);
WRAPPER RwBool RwD3D8SetTextureStageState(RwUInt32, RwUInt32, RwUInt32) { VARJMP(RwD3D8SetTextureStageState_A); }
static uint32_t RwD3D8SetVertexShader_A = AddressByVersion<uint32_t>(0x5BAF90, 0x5BB250, 0x5BF440, 0x65F2F0, 0x65F340, 0x65E2A0);
WRAPPER RwBool RwD3D8SetVertexShader(RwUInt32) { VARJMP(RwD3D8SetVertexShader_A); }
static uint32_t RwD3D8SetPixelShader_A = AddressByVersion<uint32_t>(0x5BAFD0, 0x5BB290, 0x5BF4A0, 0x65F330, 0x65F380, 0x65E2E0);
WRAPPER RwBool RwD3D8SetPixelShader(RwUInt32 handle) { VARJMP(RwD3D8SetPixelShader_A); }
static uint32_t RwD3D8SetStreamSource_A = AddressByVersion<uint32_t>(0x5BB010, 0x5BB2D0, 0x5BF500, 0x65F370, 0x65F3C0, 0x65E320);
WRAPPER RwBool RwD3D8SetStreamSource(RwUInt32, void*, RwUInt32) { VARJMP(RwD3D8SetStreamSource_A); }
static uint32_t RwD3D8SetIndices_A = AddressByVersion<uint32_t>(0x5BB060, 0x5BB320, 0x5BF590, 0x65F3C0, 0x65F410, 0x65E370);
WRAPPER RwBool RwD3D8SetIndices(void*, RwUInt32) { VARJMP(RwD3D8SetIndices_A); }
static uint32_t RwD3D8DrawIndexedPrimitive_A = AddressByVersion<uint32_t>(0x5BB0B0, 0x5BB370, 0x5BF610, 0x65F410, 0x65F460, 0x65E3C0);
WRAPPER RwBool RwD3D8DrawIndexedPrimitive(RwUInt32, RwUInt32, RwUInt32, RwUInt32, RwUInt32) { VARJMP(RwD3D8DrawIndexedPrimitive_A); }
static uint32_t RwD3D8DrawPrimitive_A = AddressByVersion<uint32_t>(0x5BB140, 0x5BB400, 0x5BF6C0, 0x65F4A0, 0x65F4F0, 0x65E450);
WRAPPER RwBool RwD3D8DrawPrimitive(RwUInt32, RwUInt32, RwUInt32) { VARJMP(RwD3D8DrawPrimitive_A); }
static uint32_t RwD3D8SetSurfaceProperties_A = AddressByVersion<uint32_t>(0x5BB490, 0x5BB750, 0x5BFB30, 0x65F7F0, 0x65F840, 0x65E7A0);
WRAPPER RwBool RwD3D8SetSurfaceProperties(const RwRGBA*, const RwSurfaceProperties*, RwBool) { VARJMP(RwD3D8SetSurfaceProperties_A); }
static uint32_t RwD3D8SetTransform_A = AddressByVersion<uint32_t>(0x5BB1D0, 0x5BB490, 0x5BF768, 0x65F530, 0x65F580, 0x65E4E0);
WRAPPER RwBool RwD3D8SetTransform(RwUInt32 a1, const void* a2)
{
	if (gtaversion != III_STEAM)
		VARJMP(RwD3D8SetTransform_A);

	__asm
	{
		mov edx, [esp+8]
		mov eax, [esp+4]
		jmp RwD3D8SetTransform_A
	}
}
static uint32_t RwD3D8GetTransform_A = AddressByVersion<uint32_t>(0x5BB310, 0x5BB5D0, 0x5BF930, 0x65F670, 0x65F6C0, 0x65E620);
WRAPPER void RwD3D8GetTransform(RwUInt32, void*) { VARJMP(RwD3D8GetTransform_A); }

//WRAPPER RwBool RwD3D8SetLight(RwInt32, const void*) { EAXJMP(0x65FB20); }
//WRAPPER RwBool RwD3D8EnableLight(RwInt32, RwBool) { EAXJMP(0x65FC10); }

static uint32_t rwD3D8RenderStateIsVertexAlphaEnable_A = AddressByVersion<uint32_t>(0x5B5A50, 0x5B5D10, 0x5BA970, 0x659F60, 0x659FB0, 0x658F10);
WRAPPER RwBool rwD3D8RenderStateIsVertexAlphaEnable(void) { VARJMP(rwD3D8RenderStateIsVertexAlphaEnable_A); };
static uint32_t rwD3D8RenderStateVertexAlphaEnable_A = AddressByVersion<uint32_t>(0x5B57E0, 0x5B5AA0, 0x5BA840, 0x659CF0, 0x659D40, 0x658CA0);
WRAPPER void rwD3D8RenderStateVertexAlphaEnable(RwBool x) { VARJMP(rwD3D8RenderStateVertexAlphaEnable_A); };
static uint32_t RpAtomicGetWorldBoundingSphere_A = AddressByVersion<uint32_t>(0x59E800, 0x59EAC0, 0x59EB20, 0x640710, 0x640760, 0x63F6C0);
WRAPPER const RwSphere *RpAtomicGetWorldBoundingSphere(RpAtomic*) { VARJMP(RpAtomicGetWorldBoundingSphere_A); };
static uint32_t RwD3D8CameraIsSphereFullyInsideFrustum_A = AddressByVersion<uint32_t>(0x5BBC40, 0x5BBF00, 0x5C0450, 0x65FFB0, 0x660000, 0x65EF60);
WRAPPER RwBool RwD3D8CameraIsSphereFullyInsideFrustum(const void*, const void*) { VARJMP(RwD3D8CameraIsSphereFullyInsideFrustum_A); };
static uint32_t RwD3D8CameraIsBBoxFullyInsideFrustum_A = AddressByVersion<uint32_t>(0x5BBCA0, 0x5BBF60, 0x5C04B0, 0x660010, 0x660060, 0x65EFC0);
WRAPPER RwBool RwD3D8CameraIsBBoxFullyInsideFrustum(const void*, const void*) { VARJMP(RwD3D8CameraIsBBoxFullyInsideFrustum_A); };

static uint32_t RtBMPImageRead_A = AddressByVersion<uint32_t>(0x5AFE70, 0x5B0130, 0x5B3390, 0x657870, 0x6578C0, 0x656820);
WRAPPER RwImage *RtBMPImageRead(const RwChar*) { VARJMP(RtBMPImageRead_A); }
static uint32_t RwImageFindRasterFormat_A = AddressByVersion<uint32_t>(0x5BBF80, 0x5BC240, 0x5C0C40, 0x6602E0, 0x660330, 0x65F290);
WRAPPER RwImage *RwImageFindRasterFormat(RwImage*, RwInt32, RwInt32*, RwInt32*, RwInt32*, RwInt32*) { VARJMP(RwImageFindRasterFormat_A); }
static uint32_t RwImageDestroy_A = AddressByVersion<uint32_t>(0x5A9180, 0x5A9440, 0x5AB6A0, 0x6512B0, 0x651300, 0x650260);
WRAPPER RwBool RwImageDestroy(RwImage*) { VARJMP(RwImageDestroy_A); }

// ADDRESS III and VC 1.0
static uint32_t RwImageCreate_A = AddressByVersion<uint32_t>(0x5A9120, 0, 0, 0x651250, 0, 0);
WRAPPER RwImage *RwImageCreate(RwInt32, RwInt32, RwInt32) { VARJMP(RwImageCreate_A); }
static uint32_t RwImageAllocatePixels_A = AddressByVersion<uint32_t>(0x5A91E0, 0, 0, 0x651310, 0, 0);
WRAPPER RwImage *RwImageAllocatePixels(RwImage *) { VARJMP(RwImageAllocatePixels_A); }
static uint32_t RwStreamOpen_A = AddressByVersion<uint32_t>(0x5A3FE0, 0, 0, 0x6459C0, 0, 0);
WRAPPER RwStream *RwStreamOpen(RwStreamType, RwStreamAccessType, const void*) { VARJMP(RwStreamOpen_A); }
static uint32_t RwStreamClose_A = AddressByVersion<uint32_t>(0x5A3F10, 0, 0, 0x6458F0, 0, 0);
WRAPPER RwBool RwStreamClose(RwStream*, void*) { VARJMP(RwStreamClose_A); }
static uint32_t RwStreamRead_A = AddressByVersion<uint32_t>(0x5A3AD0, 0, 0, 0x6454B0, 0, 0);
WRAPPER RwUInt32 RwStreamRead(RwStream*, void*, RwUInt32) { VARJMP(RwStreamRead_A); }
static uint32_t RwStreamSkip_A = AddressByVersion<uint32_t>(0x5A3DF0, 0, 0, 0x6457D0, 0, 0);
WRAPPER RwStream *RwStreamSkip(RwStream*, RwUInt32) { VARJMP(RwStreamSkip_A); }

static uint32_t RwIm2DGetNearScreenZ_A = AddressByVersion<uint32_t>(0x5A43A0, 0x5A4660, 0x5A5340, 0x649B80, 0x649BD0, 0x648B30);
WRAPPER RwReal RwIm2DGetNearScreenZ(void) { VARJMP(RwIm2DGetNearScreenZ_A); }
static uint32_t RwIm2DRenderIndexedPrimitive_A = AddressByVersion<uint32_t>(0x5A4440, 0x5A4700, 0x5A5440, 0x649C20, 0x649C70, 0x648BD0);
WRAPPER RwBool RwIm2DRenderIndexedPrimitive(RwPrimitiveType, RwIm2DVertex*, RwInt32, RwImVertexIndex*, RwInt32) { VARJMP(RwIm2DRenderIndexedPrimitive_A); }

static uint32_t _rwObjectHasFrameSetFrame_A = AddressByVersion<uint32_t>(0x5BC950, 0x5BCC10, 0x5C1820, 0x660CC0, 0x660D10, 0x65FC70);
WRAPPER void _rwObjectHasFrameSetFrame(void*, RwFrame*) { VARJMP(_rwObjectHasFrameSetFrame_A); }

static uint32_t RxPipelineCreate_A = AddressByVersion<uint32_t>(0x5C2E00, 0x5C30C0, 0x5C8FC0, 0x668FC0, 0x669010, 0x667F70);
WRAPPER RxPipeline *RxPipelineCreate(void) { VARJMP(RxPipelineCreate_A); }
static uint32_t RxPipelineLock_A = AddressByVersion<uint32_t>(0x5D29F0, 0x5D2CB0, 0x5DDBF0, 0x67AC50, 0x67ACA0, 0x679C00);
WRAPPER RxLockedPipe *RxPipelineLock(RxPipeline*) { VARJMP(RxPipelineLock_A); }
static uint32_t RxLockedPipeUnlock_A = AddressByVersion<uint32_t>(0x5D1FA0, 0x5D2260, 0x5DD0C0, 0x67A200, 0x67A250, 0x6791B0);
WRAPPER RxPipeline *RxLockedPipeUnlock(RxLockedPipe*) { VARJMP(RxLockedPipeUnlock_A); }
static uint32_t RxNodeDefinitionGetD3D8AtomicAllInOne_A = AddressByVersion<uint32_t>(0x5DC500, 0x5DC7C0, 0x5EC470, 0x67CB90, 0x67CBE0, 0x67BB40);
WRAPPER RxNodeDefinition *RxNodeDefinitionGetD3D8AtomicAllInOne(void) { VARJMP(RxNodeDefinitionGetD3D8AtomicAllInOne_A); }
static uint32_t RxLockedPipeAddFragment_A = AddressByVersion<uint32_t>(0x5D2BA0, 0x5D2E60, 0x5DE000, 0x67AE00, 0x67AE50, 0x679DB0);
WRAPPER RxLockedPipe *RxLockedPipeAddFragment(RxLockedPipe*, RwUInt32*, RxNodeDefinition*, ...) { VARJMP(RxLockedPipeAddFragment_A); }
static uint32_t _rxPipelineDestroy_A = AddressByVersion<uint32_t>(0x5C2E70, 0x5C3130, 0x5C9030, 0x669030, 0x669080, 0x667FE0);
WRAPPER void _rxPipelineDestroy(RxPipeline*) { VARJMP(_rxPipelineDestroy_A); }
static uint32_t RxPipelineFindNodeByName_A = AddressByVersion<uint32_t>(0x5D2B10, 0x5D2DD0, 0x5DDF40, 0x67AD70, 0x67ADC0, 0x679D20);
WRAPPER RxPipelineNode *RxPipelineFindNodeByName(RxPipeline*, const RwChar*, RxPipelineNode*, RwInt32*) { VARJMP(RxPipelineFindNodeByName_A); }
static uint32_t RxD3D8AllInOneSetRenderCallBack_A = AddressByVersion<uint32_t>(0x5DFC60, 0x5DFF20, 0x5EE330, 0x678E30, 0x678E80, 0x677DE0);
WRAPPER void RxD3D8AllInOneSetRenderCallBack(RxPipelineNode*, RxD3D8AllInOneRenderCallBack) { VARJMP(RxD3D8AllInOneSetRenderCallBack_A); }

static uint32_t rxD3D8DefaultRenderCallback_A = AddressByVersion<uint32_t>(0x5DF960, 0x5DFC20, 0x5EE350, 0x678B30, 0x678B80, 0x677AE0);
WRAPPER void rxD3D8DefaultRenderCallback(RwResEntry*, void*, RwUInt8, RwUInt32) { VARJMP(rxD3D8DefaultRenderCallback_A); }
static uint32_t rwD3D8AtomicMatFXRenderCallback_A = AddressByVersion<uint32_t>(0x5D0B80, 0x5D0E40, 0x5D8B20, 0x676460, 0x6764B0, 0x675410);
WRAPPER void rwD3D8AtomicMatFXRenderCallback(RwResEntry*, void*, RwUInt8, RwUInt32) { VARJMP(rwD3D8AtomicMatFXRenderCallback_A); }

static uint32_t RpClumpForAllAtomics_A = AddressByVersion<uint32_t>(0x59EDD0, 0x59F090, 0x59EFC0, 0x640D00, 0x640D50, 0x63FCB0);
WRAPPER RpClump *RpClumpForAllAtomics(RpClump*, RpAtomicCallBack, void*) { VARJMP(RpClumpForAllAtomics_A); }

#endif
