
struct CColSphere
{
	CVector center;
	float radius;
	uchar mat;
	uchar flag;

	void Set(float radius, CVector *center, uchar mat, uchar flag);
};

struct CColBox
{
	CVector min;
	CVector max;
	uchar mat;
	uchar flag;

	void Set(CVector *min, CVector *max, uchar mat, uchar flag);
};

struct CColLine
{
	CVector p0;
	int pad0;
	CVector p1;
	int pad1;

	void Set(CVector *p0, CVector *p1);
};

struct CColTriangle
{
	short a;
	short b;
	short c;
	uchar mat;

	void Set(int a, int b, int c, uchar mat);
};

struct CColPoint
{
	CVector point;
	int pad1;
	CVector normal;
	int pad2;
	byte surfaceTypeA;
	byte pieceTypeA;
	byte pieceTypeB;
	byte surfaceTypeB;
	float depth;
};

struct CColModel
{
	CColSphere boundingSphere;
	CColBox boundingBox;
	short numSpheres;
	short numLines;
	short numBoxes;
	short numTriangles;
	int level;
	byte unk1;
	CColSphere *spheres;
	CColLine *lines;
	CColBox *boxes;
	CVector *vertices;
	CColTriangle *triangles;
	int unk2;

	void ctor(void);
};



struct CBaseModelInfo
{
	void **vtable;
	char     name[24];
	CColModel *colModel;
	void *twodeffect;
	short id;
	ushort refCount;
	short txdSlot;
	uchar type;
	uchar num2dEffects;
	bool freeCol;
};
static_assert(sizeof(CBaseModelInfo) == 0x30, "CBaseModelInfo: wrong size");

struct CClumpModelInfo : public CBaseModelInfo
{
	RpClump *clump;

	RpClump *CreateInstance(void);
	void SetClump(RpClump*);
	void SetFrameIds(int ids);

	static RpAtomic *SetAtomicRendererCB(RpAtomic*, void*);
};
static_assert(sizeof(CClumpModelInfo) == 0x34, "CBaseModelInfo: wrong size");

struct CPedModelInfo : public CClumpModelInfo
{
	int animGroup;
	int unk1;
	int pedStats;
	int unk2;
	CColModel *hitColModel;
	// skin extension
	RpAtomic *head;
	RpAtomic *lhand;
	RpAtomic *rhand;

	CPedModelInfo(void);
	void SetClump(RpClump *clump);
	void DeleteRwObject(void);
	CColModel *AnimatePedColModelSkinned(RpClump *clump);
	void CreateHitColModel(void);
	void CreateHitColModelSkinned(RpClump *clump);
};

struct CModelInfo
{
	static CBaseModelInfo **ms_modelInfoPtrs;
	static CBaseModelInfo *GetModelInfo(const char *s, int id);
};


struct CStreamingInfo
{
	CStreamingInfo *next;
	CStreamingInfo *prev;
	uint8 loadState;
	uint8 flags;
	int16 nextID;
	int position;
	int size;
};

struct CPlaceable
{
	void **vtable;
	CMatrix matrix;
};
static_assert(sizeof(CPlaceable) == 0x4C, "CPlaceable: wrong size");

struct CEntity : CPlaceable
{
	RpClump *clump;
	char bfTypeStatus;
	char bfFlagsA;
	char bfFlagsB;
	char bfFlagsC;
	char bfFlagsD;
	char bfFlagsE;
	char __f0052[2];
	short unk1;
	short uiPathMedianRand;
	short nModelIndex;
	short level;
	int pFirstRef;

	void SetModelIndex(int id);
	void Render(void);
	void UpdateRpHAnim(void);
	void UpdateRwFrame(void);
};
static_assert(sizeof(CEntity) == 0x64, "CEntity: wrong size");

struct CEntityVMT
{
	void (__cdecl *dtor)(void *);
	int Add;
	int Remove;
	void (__thiscall *SetModelIndex)(CEntity *e, int16 id);
	void (__thiscall *SetModelIndexNoCreate)(CEntity *e, uint);
	int CreateRwObject;
	void (__thiscall *DeleteRwObject)(CEntity *e);
	CRect *(__thiscall *GetBoundRect)(CEntity *e, CRect *ret);
	int ProcessControl;
	int ProcessCollision;
	int ProcessShift;
	void (__thiscall *Teleport)(CEntity *e, float x, float y, float z);
	int PreRender;
	int Render;
	int SetupLighting;
	int RemoveLighting;
	int FlagToDestroyWhenNextProcessed;
};

struct CPhysical : CEntity
{
	int uAudioEntityId;
	int unk1;
	int field_6C;
	int field_70;
	int m_dwLastTimeCollided;
	CVector vecMoveSpeed;
	CVector vecTurnSpeed;
	CVector vecShiftVector1;
	CVector vecShiftVector2;
	CVector vecShiftVector3;
	CVector vecShiftVector4;
	float fMass;
	float fTurnMass;
	float fForceMultiplier;
	float fAirResistance;
	int fElasticity;
	float fPercentSubmerged;
	CVector vecCentreOfMass;
	void *pEntryInfoNode;
	int pMovingListNode;
	char field_EC;
	char field_ED;
	char uCollidingNum;
	char field_EF;
	CEntity *pCollisionRecords[6];
	float fTotSpeed;
	float fCollisionPower;
	float pPhysColliding;
	CVector vecCollisionPower;
	short wComponentCol;
	char byteMoveFlags;
	char byteCollFlags;
	char byteLastCollType;
	char byteZoneLevel;
	short __padding;

	void ProcessControl(void);
};
static_assert(sizeof(CPhysical) == 0x128, "CPhysical: wrong size");

struct CPed : CEntity
{
	int pad;

	bool IsPedInControl(void);
};

struct CVehicle : CEntity
{
	int pad;
};

struct CAutomobile : CVehicle
{
	int pad;

	CAutomobile *ctor(int id, uint8 type);
};

//
// Camera
//

#define NUMBER_OF_VECTORS_FOR_AVERAGE 2

struct CCam
{
	bool    bBelowMinDist; //used for follow ped mode
	bool    bBehindPlayerDesired; //used for follow ped mode
	bool    m_bCamLookingAtVector;
	bool    m_bCollisionChecksOn;
	bool    m_bFixingBeta; //used for camera on a string
	bool    m_bTheHeightFixerVehicleIsATrain;
	bool    LookBehindCamWasInFront;
	bool    LookingBehind;
	bool    LookingLeft; // 32
	bool    LookingRight;
	bool    ResetStatics; //for interpolation type stuff to work
	bool    Rotating;

	int16   Mode;                   // CameraMode
	uint32  m_uiFinishTime; // 52

	int     m_iDoCollisionChecksOnFrameNum; 
	int     m_iDoCollisionCheckEveryNumOfFrames;
	int     m_iFrameNumWereAt;  // 64
	int     m_iRunningVectorArrayPos;
	int     m_iRunningVectorCounter;
	int     DirectionWasLooking;

	float   f_max_role_angle; //=DEGTORAD(5.0f);    
	float   f_Roll; //used for adding a slight roll to the camera in the
	float	f_rollSpeed;
	float   m_fSyphonModeTargetZOffSet;
float unknown;
	float   m_fAmountFractionObscured;
	float   m_fAlphaSpeedOverOneFrame; // 100
	float   m_fBetaSpeedOverOneFrame;
	float   m_fBufferedTargetBeta;
	float   m_fBufferedTargetOrientation;
	float   m_fBufferedTargetOrientationSpeed;
	float   m_fCamBufferedHeight;
	float   m_fCamBufferedHeightSpeed;
	float   m_fCloseInPedHeightOffset;
	float   m_fCloseInPedHeightOffsetSpeed; // 132
	float   m_fCloseInCarHeightOffset;
	float   m_fCloseInCarHeightOffsetSpeed;
	float   m_fDimensionOfHighestNearCar;       
	float   m_fDistanceBeforeChanges;
	float   m_fFovSpeedOverOneFrame;
	float   m_fMinDistAwayFromCamWhenInterPolating;
	float   m_fPedBetweenCameraHeightOffset;
	float   m_fPlayerInFrontSyphonAngleOffSet; // 164
	float   m_fRadiusForDead;
	float   m_fRealGroundDist; //used for follow ped mode
	float   m_fTargetBeta;
	float   m_fTimeElapsedFloat;  
  
	float   m_fTransitionBeta;
	float   m_fTrueBeta;
	float   m_fTrueAlpha; // 200
	float   m_fInitialPlayerOrientation; //used for first person

	float   Alpha;
	float   AlphaSpeed;
	float   FOV;
	float   FOVSpeed;
	float   Beta;
	float   BetaSpeed;
	float   Distance; // 232
	float   DistanceSpeed;
	float   CA_MIN_DISTANCE;
	float   CA_MAX_DISTANCE;
	float   SpeedVar;

	// ped onfoot zoom distance
	float m_fTargetZoomGroundOne;
	float m_fTargetZoomGroundTwo; // 256
	float m_fTargetZoomGroundThree;
	// ped onfoot alpha angle offset
	float m_fTargetZoomOneZExtra;
	float m_fTargetZoomTwoZExtra;
	float m_fTargetZoomThreeZExtra;
    
	float m_fTargetZoomZCloseIn;
	float m_fMinRealGroundDist;
	float m_fTargetCloseInDist;

	CVector m_cvecTargetCoorsForFudgeInter; // 360
	CVector m_cvecCamFixedModeVector; // 372
	CVector m_cvecCamFixedModeSource; // 384
	CVector m_cvecCamFixedModeUpOffSet; // 396
	CVector m_vecLastAboveWaterCamPosition; //408  //helper for when the player has gone under the water
	CVector m_vecBufferedPlayerBodyOffset; // 420

	// The three vectors that determine this camera for this frame
	CVector Front;  // 432                                              // Direction of looking in
	CVector Source;                                                 // Coors in world space
	CVector SourceBeforeLookBehind;
	CVector Up;                                                     // Just that
	CVector m_arrPreviousVectors[NUMBER_OF_VECTORS_FOR_AVERAGE];    // used to average stuff
	CEntity *CamTargetEntity;

	float       m_fCameraDistance;
	float       m_fIdealAlpha;
	float       m_fPlayerVelocity;
	CAutomobile  *m_pLastCarEntered; // So interpolation works
	CPed         *m_pLastPedLookedAt;// So interpolation works 
	bool        m_bFirstPersonRunAboutActive;


	void Process_Debug(float *vec, float a, float b, float c);
	void GetVectorsReadyForRW(void);
};
static_assert(sizeof(CCam) == 0x1A4, "CCam: wrong size");
static_assert(offsetof(CCam, Alpha) == 0xA8, "CCam: error");
static_assert(offsetof(CCam, Front) == 0x140, "CCam: error");

struct CCamPathSplines
{
	float m_arr_PathData[800];
};

struct CTrainCamNode
{
	CVector m_cvecCamPosition;
	CVector m_cvecPointToLookAt;
	CVector m_cvecMinPointInRange;
	CVector m_cvecMaxPointInRange;
	float m_fDesiredFOV;
	float m_fNearClip;
};

struct CQueuedMode
{
	int16 Mode;
	float Duration;
	int16 MinZoom;
	int16 MaxZoom;
};

struct CCamera : public CPlaceable
{
	bool m_bAboveGroundTrainNodesLoaded;
	bool m_bBelowGroundTrainNodesLoaded;
	bool m_bCamDirectlyBehind;
	bool m_bCamDirectlyInFront;
	bool m_bCameraJustRestored;
	bool m_bcutsceneFinished;
	bool m_bCullZoneChecksOn;
	bool m_bFirstPersonBeingUsed;
	bool m_bJustJumpedOutOf1stPersonBecauseOfTarget;
	bool m_bIdleOn;
	bool m_bInATunnelAndABigVehicle;
	bool m_bInitialNodeFound;
	bool m_bInitialNoNodeStaticsSet;
	bool m_bIgnoreFadingStuffForMusic;
	bool m_bPlayerIsInGarage;
	bool m_bJustCameOutOfGarage;
	bool m_bJustInitalised;
	bool m_bJust_Switched;
	bool m_bLookingAtPlayer;
	bool m_bLookingAtVector;
	bool m_bMoveCamToAvoidGeom;
	bool m_bObbeCinematicPedCamOn;
	bool m_bObbeCinematicCarCamOn;
	bool m_bRestoreByJumpCut;
	bool m_bUseNearClipScript;
	bool m_bStartInterScript;
	bool m_bStartingSpline;
	bool m_bTargetJustBeenOnTrain;
	bool m_bTargetJustCameOffTrain;
	bool m_bUseSpecialFovTrain;
	bool m_bUseTransitionBeta;
	bool m_bUseScriptZoomValuePed;
	bool m_bUseScriptZoomValueCar;
	bool m_bWaitForInterpolToFinish;
	bool m_bItsOkToLookJustAtThePlayer;
	bool m_bWantsToSwitchWidescreenOff;
	bool m_WideScreenOn;
	bool m_1rstPersonRunCloseToAWall;
	bool m_bHeadBob;
	bool m_bFailedCullZoneTestPreviously;

bool m_FadeTargetIsSplashScreen;

	bool WorldViewerBeingUsed;
	uint8 ActiveCam;
	uint32 m_uiCamShakeStart;

bool m_bVehicleSuspenHigh;
bool m_bEnable1rstPersonCamCntrlsScript;
bool m_bAllow1rstPersonWeaponsCamera;

	uint32 m_uiLongestTimeInMill;
	uint32 m_uiNumberOfTrainCamNodes;
	uint8   m_uiTransitionJUSTStarted;
	uint8   m_uiTransitionState;        // 0:one mode 1:transition

	uint32 m_uiTimeLastChange;
	uint32 m_uiTimeWeEnteredIdle;
	uint32 m_uiTimeTransitionStart;
	uint32 m_uiTransitionDuration;
	int m_BlurBlue;
	int m_BlurGreen;
	int m_BlurRed;
	int m_BlurType;

uint32    unknown;
	int m_iWorkOutSpeedThisNumFrames;
	int m_iNumFramesSoFar;


	int m_iCurrentTrainCamNode;
	int m_motionBlur;
	int m_imotionBlurAddAlpha;
	int m_iCheckCullZoneThisNumFrames;
	int m_iZoneCullFrameNumWereAt;
	int WhoIsInControlOfTheCamera;
	float CamFrontXNorm;
	float CamFrontYNorm;
	float CarZoomIndicator;
	float CarZoomValue;
	float CarZoomValueSmooth;
	float DistanceToWater;
	float FOVDuringInter;
	float LODDistMultiplier;
	float GenerationDistMultiplier;
	float m_fAlphaSpeedAtStartInter;
	float m_fAlphaWhenInterPol;
	float m_fAlphaDuringInterPol;
	float m_fBetaDuringInterPol;
	float m_fBetaSpeedAtStartInter;
	float m_fBetaWhenInterPol;
	float m_fFOVWhenInterPol;
	float m_fFOVSpeedAtStartInter;
	float m_fStartingBetaForInterPol;
	float m_fStartingAlphaForInterPol;
	float m_PedOrientForBehindOrInFront;
	float m_CameraAverageSpeed;
	float m_CameraSpeedSoFar;
	float m_fCamShakeForce;
	float m_fCarZoomValueScript;
	float m_fFovForTrain;
	float m_fFOV_Wide_Screen;
	float m_fNearClipScript;
	float m_fOldBetaDiff;
	float m_fPedZoomValue;

  float m_fPedZoomValueScript;
  float m_fPedZoomValueSmooth;
  float m_fPositionAlongSpline;
  float m_ScreenReductionPercentage;
  float m_ScreenReductionSpeed;
  float m_AlphaForPlayerAnim1rstPerson;
  float Orientation;
  float PedZoomIndicator;
  float PlayerExhaustion;
  float SoundDistUp, SoundDistLeft, SoundDistRight;
  float SoundDistUpAsRead, SoundDistLeftAsRead, SoundDistRightAsRead;
  float SoundDistUpAsReadOld, SoundDistLeftAsReadOld, SoundDistRightAsReadOld;
  float m_fWideScreenReductionAmount;
  float m_fStartingFOVForInterPol;

	// not static yet
	float m_fMouseAccelHorzntl;// acceleration multiplier for 1st person controls
	float m_fMouseAccelVertical;// acceleration multiplier for 1st person controls
	float m_f3rdPersonCHairMultX;
	float m_f3rdPersonCHairMultY;


	CCam Cams[3];
	void *pToGarageWeAreIn;
	void *pToGarageWeAreInForHackAvoidFirstPerson;
	CQueuedMode m_PlayerMode;
	CQueuedMode PlayerWeaponMode;
	CVector m_PreviousCameraPosition;
	CVector m_RealPreviousCameraPosition;
	CVector m_cvecAimingTargetCoors;
	CVector m_vecFixedModeVector;

	// one of those has to go
  CVector m_vecFixedModeSource;
  CVector m_vecFixedModeUpOffSet;
//  CVector m_vecCutSceneOffset;
  CVector m_cvecStartingSourceForInterPol;
  CVector m_cvecStartingTargetForInterPol;
  CVector m_cvecStartingUpForInterPol;
  CVector m_cvecSourceSpeedAtStartInter;
  CVector m_cvecTargetSpeedAtStartInter;
  CVector m_cvecUpSpeedAtStartInter;
  CVector m_vecSourceWhenInterPol;
  CVector m_vecTargetWhenInterPol;
  CVector m_vecUpWhenInterPol;
  CVector m_vecClearGeometryVec;

	CVector m_vecGameCamPos;
	CVector SourceDuringInter;
	CVector TargetDuringInter;
	CVector UpDuringInter;
	RwCamera *m_pRwCamera;
	CEntity *pTargetEntity;
	CCamPathSplines m_arrPathArray[4];
	CTrainCamNode m_arrTrainCamNode[800];
	CMatrix m_cameraMatrix;
	bool m_bGarageFixedCamPositionSet;
	bool m_vecDoingSpecialInterPolation;
	bool m_bScriptParametersSetForInterPol;
	bool m_bFading;
	bool m_bMusicFading;
	CMatrix m_viewMatrix;
	CVector m_vecFrustumNormals[4];
	CVector m_vecOldSourceForInter;
	CVector m_vecOldFrontForInter;
	CVector m_vecOldUpForInter;

	float m_vecOldFOVForInter;
	float m_fFLOATingFade;
	float m_fFLOATingFadeMusic;
	float m_fTimeToFadeOut;
	float m_fTimeToFadeMusic;
	float m_fFractionInterToStopMovingTarget;
	float m_fFractionInterToStopCatchUpTarget;
	float m_fGaitSwayBuffer;
	float m_fScriptPercentageInterToStopMoving;
	float m_fScriptPercentageInterToCatchUp;

uint32  m_fScriptTimeForInterPolation;


int16   m_iFadingDirection;
int     m_iModeObbeCamIsInForCar;
	int16   m_iModeToGoTo;
	int16   m_iMusicFadingDirection;
	int16   m_iTypeOfSwitch;

	uint32 m_uiFadeTimeStarted;
	uint32 m_uiFadeTimeStartedMusic;

	void InitialiseCameraForDebugMode(void);
};
static_assert(offsetof(CCamera, m_WideScreenOn) == 0x70, "CCamera: error");
static_assert(offsetof(CCamera, WorldViewerBeingUsed) == 0x75, "CCamera: error");
static_assert(offsetof(CCamera, m_uiNumberOfTrainCamNodes) == 0x84, "CCamera: error");
static_assert(offsetof(CCamera, m_uiTransitionState) == 0x89, "CCamera: error");
static_assert(offsetof(CCamera, m_uiTimeTransitionStart) == 0x94, "CCamera: error");
static_assert(offsetof(CCamera, m_BlurBlue) == 0x9C, "CCamera: error");
static_assert(offsetof(CCamera, Cams) == 0x1A4, "CCamera: error");
static_assert(sizeof(CCamera) == 0xE9D8, "CCamera: wrong size");
extern CCamera &TheCamera;

class CFileMgr
{
public:
	static void SetDir(const char *path);
};

class CGame
{
public:
	static void ReloadIPLs(void);
	static void Process(void);
};

class CTimer
{
public:
	static uchar &m_UserPause;
	static uchar &m_CodePause;
	static int &m_snTimeInMilliseconds;
	static void Stop(void);
	static void Update(void);
};

class CClock
{
public:
	static char  &ms_nGameClockHours;
	static char  &ms_nGameClockMinutes;
	static short &ms_nGameClockSeconds;
};

class CWeather
{
public:
	static float &InterpolationValue;
	static short &OldWeatherType;
	static short &NewWeatherType;
};

class CCoronas
{
public:
	static void DoSunAndMoon(void);
	static void Update(void);
};

struct CControllerState
{
	short LEFTSTICKX;
	short LEFTSTICKY;
	short RIGHTSTICKX;
	short RIGHTSTICKY;
	short LEFTSHOULDER1;
	short LEFTSHOULDER2;
	short RIGHTSHOULDER1;
	short RIGHTSHOULDER2;
	short DPADUP;
	short DPADDOWN;
	short DPADLEFT;
	short DPADRIGHT;
	short START;
	short SELECT;
	short SQUARE;
	short TRIANGLE;
	short CROSS;
	short CIRCLE;
	short LEFTSHOCK;
	short RIGHTSHOCK;
	short NETWORK_TALK;
};

struct CMouseControllerState
{
	uchar lmb;
	uchar rmb;
	uchar mmb;
	uchar wheelUp;
	uchar wheelDown;
	uchar bmx1;
	uchar bmx2;
	float X;
	float Y;
};


class CPad
{
public:
	CControllerState NewState;
	CControllerState OldState;
	CControllerState PCTempKeyState;
	CControllerState PCTempJoyState;
	CControllerState PCTempMouseState;
	char Phase;
	char gap_d3[1];
	short Mode;
	short ShakeDur;
	char ShakeFreq;
	char bHornHistory[5];
	char iCurrHornHistory;
	char DisablePlayerControls;
	char JustOutOfFrontEnd;
	char bApplyBrakes;
	char field_e2[12];
	char gap_ee[2];
	int LastTimeTouched;
	int AverageWeapon;
	int AverageEntries;

	static short *NewKeyState;	// 312
	static short *OldKeyState;	// 312
	static CMouseControllerState &NewMouseControllerState;
	static CMouseControllerState &OldMouseControllerState;
	static bool &m_bMapPadOneToPadTwo;

	static CPad *GetPad(int);
};
static_assert(sizeof(CPad) == 0xFC, "CPad: wrong size");

class CControllerConfigManager
{
public:
	bool GetIsKeyboardKeyDown(RsKeyCodes key);
	bool GetIsKeyboardKeyJustDown(RsKeyCodes key);
};

#define JUSTDOWN(b) (pad->NewState.b && !pad->OldState.b)
#define KEYJUSTDOWN(k) ctrldummy->GetIsKeyboardKeyJustDown((RsKeyCodes)k)
#define KEYDOWN(k) ctrldummy->GetIsKeyboardKeyDown((RsKeyCodes)k)
#define CTRLJUSTDOWN(key) \
	       ((KEYDOWN(rsLCTRL) || KEYDOWN(rsRCTRL)) && KEYJUSTDOWN((RsKeyCodes)key) || \
	        (KEYJUSTDOWN(rsLCTRL) || KEYJUSTDOWN(rsRCTRL)) && KEYDOWN((RsKeyCodes)key))
#define CTRLDOWN(key) ((KEYDOWN(rsLCTRL) || KEYDOWN(rsRCTRL)) && KEYDOWN((RsKeyCodes)key))

struct CRGBA
{
	uchar r, g, b, a;
	CRGBA(uchar r, uchar g, uchar b, uchar a) : r(r), g(g), b(b), a(a) {}
};

struct CRect
{
	float left, top, right, bottom;
	CRect(float l, float t, float r, float b) : left(l), top(t), right(r), bottom(b) {}
};

struct CSprite2d
{
	static void Draw(CRect const &,CRGBA const &);
	static void DrawRect(CRect const &,CRGBA const &);
};

struct CFont
{
	enum {
		REGULAR,
		PAGER,
		PRICEDOWN
	};
	// everything that is in my IDB
	static void Initialise(void);
	static void Shutdown(void);
	static void InitPerFrame(void);
	static void PrintChar(float,float,short);
	static void PrintString(float,float,ushort *);
	static void PrintString(float,float,ushort *,ushort *,float);
	static void ParseToken(ushort *,ushort *);
	static void DrawFonts(void);
	static void SetScale(float,float);
	static void SetColor(CRGBA*);
	static void SetJustifyOff(void);
	static void SetCentreOn(void);
	static void SetCentreOff(void);
	static void SetCentreSize(float);
	static void SetBackgroundOff(void);
	static void SetBackGroundOnlyTextOff(void);
	static void SetRightJustifyOn(void);
	static void SetPropOff(void);
	static void SetPropOn(void);
	static void SetFontStyle(short);
	static void SetRightJustifyWrap(float);
	static void SetDropShadowPosition(short);
	static double GetStringWidth(ushort *str, bool spc);
};

void AsciiToUnicode(char const *,ushort *);
void debug(const char *fmt, ...);

struct CHud
{
	static void Draw(void);
};

CEntity *FindPlayerPed(void);
CEntity *FindPlayerVehicle(void);
void FindPlayerCoors(CVector*);
