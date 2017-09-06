
// VC

class CTimer
{
public:
	static int &m_snTimeInMilliseconds;
};

class CClock
{
public:
	static int8  &ms_nGameClockHours;
	static int8  &ms_nGameClockMinutes;
	static int16 &ms_nGameClockSeconds;
};

class CWeather
{
public:
	static float &InterpolationValue;
	static int16 &OldWeatherType;
	static int16 &NewWeatherType;
};

struct CDraw
{
	static void SetFOV(float fov);
};


//
// Input stuff
//

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
static_assert(sizeof(CControllerState) == 0x2A, "CControllerState: wrong size");

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

enum RsKeyCodes
{
	rsESC = 0x3E8,
	rsF1 = 0x3E9,
	rsF2 = 0x3EA,
	rsF3 = 0x3EB,
	rsF4 = 0x3EC,
	rsF5 = 0x3ED,
	rsF6 = 0x3EE,
	rsF7 = 0x3EF,
	rsF8 = 0x3F0,
	rsF9 = 0x3F1,
	rsF10 = 0x3F2,
	rsF11 = 0x3F3,
	rsF12 = 0x3F4,
	rsINSERT = 0x3F5,
	rsDEL = 0x3F6,
	rsHOME = 0x3F7,
	rsEND = 0x3F8,
	rsPGUP = 0x3F9,
	rsPGDN = 0x3FA,
	rsUP = 0x3FB,
	rsDOWN = 0x3FC,
	rsLEFT = 0x3FD,
	rsRIGHT = 0x3FE,
	rsDIV = 0x3FF,
	rsMUL = 0x400,
	rsADD = 0x401,
	rsSUB = 0x402,
	rsEXTDEL = 0x403,
	rsEXT_END = 0x404,
	rsEXTDOWN = 0x405,
	rsEXTPGDN = 0x406,
	rsEXTLEFT = 0x407,
	rsNUM5 = 0x408,
	rsNUMLOCK = 0x409,
	rsEXTRIGHT = 0x40A,
	rsEXTHOME = 0x40B,
	rsEXTUP = 0x40C,
	rsEXTPGUP = 0x40D,
	rsEXTINSERT = 0x40E,
	rsENTER = 0x40F,
	rsSCROLL = 0x410,
	rsPAUSE = 0x411,
	rsBACK = 0x412,
	rsTAB = 0x413,
	rsCAPSLOCK = 0x414,
	rsEXTENTER = 0x415,
	rsLSHIFT = 0x416,
	rsRSHIFT = 0x417,
	rsSHIFT = 0x418,
	rsLCTRL = 0x419,
	rsRCTRL = 0x41A,
	rsLMENU = 0x41B,
	rsRMENU = 0x41C,
	rsLWIN = 0x41D,
	rsRWIN = 0x41E,
	rsAPPS = 0x41F,
	rsNULL = 0x420,
};

class CPad
{
public:
	CControllerState	NewState;
	CControllerState	OldState;
	short			SteeringLeftRightBuffer[10];
	int				DrunkDrivingBufferUsed;
	CControllerState	PCTempKeyState;
	CControllerState	PCTempJoyState;
	CControllerState	PCTempMouseState;
	BYTE			Phase;
	WORD			Mode;
	short			ShakeDur;
	WORD			DisablePlayerControls;
	BYTE			ShakeFreq;
	bool			bHornHistory[5];
	BYTE			iCurrHornHistory;
	bool			JustOutOfFrontEnd;
	bool			bApplyBrakes;
	BYTE			_pad1[12];
	int			LastTimeTouched;
	DWORD			AverageWeapon;
	DWORD			AverageEntries;

	static CMouseControllerState &NewMouseControllerState;
	static CMouseControllerState &OldMouseControllerState;

	static CPad *GetPad(int);
};
static_assert(sizeof(CPad) == 0x114, "CPad: wrong size");

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

extern CControllerConfigManager *ctrldummy;

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
static_assert(sizeof(CStreamingInfo) == 0x14, "CStreamingInfo: size error");

struct PathNode
{
	int16 wPathData1;
	int16 wPathData2;
	int16 x;
	int16 y;
	int16 z;
	char __f000A[10];
};
static_assert(sizeof(PathNode) == 0x14, "PathNode: size error");

struct CPathFind {
	PathNode nodes[9650];

	int FindNodeClosestToCoors(float x, float y, float z, char a7, float f8, char a9, char a10, char a11, char a12);
};
extern CPathFind &ThePaths;

struct CWanted
{
	void CheatWantedLevel(int level);
};

struct CPlaceable
{
	CMatrix matrix;
	CVector GetPosition(void){
		return *(CVector*)&matrix.matrix.pos;
	}
	void SetPosition(CVector &pos){
		*(CVector*)&matrix.matrix.pos = pos;
	}
};
static_assert(sizeof(CPlaceable) == 0x48, "CPlaceable: size error");

struct CEntity
{
	void *vtable;
	CPlaceable _;
	RpClump *pClump;
	char bfTypeStatus;
	char bfFlagsA;
	char bfFlagsB;
	char bfFlagsC;
	char bfFlagsD;
	char bfFlagsE;
	char gap_56[2];
	__int16 field_58;
	__int16 unk_randomSeed;
	int16 nModelIndex;
	char byteLevel;
	char byteInterior;
	int pFirstRef;
};
static_assert(sizeof(CEntity) == 0x64, "CEntity: size error");

struct CPhysical : public CEntity
{
};

struct CVehicle : public CPhysical
{
};

struct CAutomobile : public CVehicle
{
	CAutomobile *ctor(int id, uint8 type);
};

struct CPed : public CPhysical
{
};

struct CPlayerPed : public CPed
{
	static void (__thiscall *ProcessControl)(CPlayerPed*);
	void ProcessControl_hooked(void);
};

struct CEntityVtbl
{
	unsigned int (__thiscall *Add)(CEntity *self);
	unsigned int (__thiscall *Remove)(CEntity *self);
	void *(__thiscall *__vecDelDtor)(CEntity *self, unsigned int);
	unsigned int (__thiscall *SetModelIndex)(CEntity *self, unsigned int);
	unsigned int (__thiscall *SetModelIndexNoCreate)(CEntity *self, unsigned int);
	unsigned int (__thiscall *CreateRwObject)(CEntity *self);
	unsigned int (__thiscall *DeleteRwObject)(CEntity *self);
	CRect *(__thiscall *GetBoundRect)(CEntity *self, CRect *result);
	void (__thiscall *ProcessControl)(CEntity *self);
	unsigned int (__thiscall *ProcessCollision)(CEntity *self);
	unsigned int (__thiscall *ProcessShift)(CEntity *self);
	void (__thiscall *Teleport)(CEntity *self, CVector);
	void (__thiscall *PreRender)(CEntity *self);
	void (__thiscall *Render)(CEntity *self);
	unsigned int (__thiscall *SetupLighting)(CEntity *self);
	unsigned int (__thiscall *RemoveLighting)(CEntity *self, bool);
	void (__thiscall *FlagToDestroyWhenNextProcessed)(CEntity *self);
};

struct CGarage;

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
	float   f_rollSpeed; //camera on a string mode
	float   m_fSyphonModeTargetZOffSet;
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
	float   m_fTilt;
	float   m_fTiltSpeed;

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

	CVector m_cvecSourceSpeedOverOneFrame; // 324
	CVector m_cvecTargetSpeedOverOneFrame; // 336
	CVector m_cvecUpOverOneFrame; // 348
    
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

	void Process(void);
	void Process_Debug(float*, float, float, float);
	void GetVectorsReadyForRW(void);
};
static_assert(sizeof(CCam) == 0x1CC, "CCam: size error");

#define MAX_NUM_OF_SPLINETYPES (4)

struct CQueuedMode
{
	int16 Mode;
	float Duration;
	int16 MinZoom;
	int16 MaxZoom;
};

struct CCamPathSplines
{
	enum {MAXPATHLENGTH=800};
	float *m_arr_PathData;//    FLOAT m_arr_PathData[MAXPATHLENGTH];    
};

struct CCamera : CPlaceable
{
	bool    m_bAboveGroundTrainNodesLoaded;
	bool    m_bBelowGroundTrainNodesLoaded;
	bool    m_bCamDirectlyBehind;   
	bool    m_bCamDirectlyInFront;  
	bool    m_bCameraJustRestored;
	bool    m_bcutsceneFinished;
	bool    m_bCullZoneChecksOn;
	bool    m_bFirstPersonBeingUsed; // To indicate if the m_bFirstPersonBeingUsed viewer is being used.
	bool    m_bJustJumpedOutOf1stPersonBecauseOfTarget;
	bool    m_bIdleOn;
	bool    m_bInATunnelAndABigVehicle;
	bool    m_bInitialNodeFound;
	bool    m_bInitialNoNodeStaticsSet;
	bool    m_bIgnoreFadingStuffForMusic;
	bool    m_bPlayerIsInGarage;
	bool    m_bPlayerWasOnBike;
	bool    m_bJustCameOutOfGarage;
	bool    m_bJustInitalised;//Just so the speed thingy doesn't go mad right at the start
	uint8   m_bJust_Switched;//Variable to indicate that we have jumped somewhere, Raymond needs this for the audio engine
	bool    m_bLookingAtPlayer;
	bool    m_bLookingAtVector;
	bool    m_bMoveCamToAvoidGeom;
	bool    m_bObbeCinematicPedCamOn;
	bool    m_bObbeCinematicCarCamOn;
	bool    m_bRestoreByJumpCut;
	bool    m_bUseNearClipScript;
	bool    m_bStartInterScript;
	uint8   m_bStartingSpline;
	bool    m_bTargetJustBeenOnTrain; //this variable is needed to be able to restore the camera
	bool    m_bTargetJustCameOffTrain;
	bool    m_bUseSpecialFovTrain;
	bool    m_bUseTransitionBeta;
	bool    m_bUseScriptZoomValuePed;
	bool    m_bUseScriptZoomValueCar;
	bool    m_bWaitForInterpolToFinish;
	bool    m_bItsOkToLookJustAtThePlayer; //Used when interpolating
	bool    m_bWantsToSwitchWidescreenOff;  
	bool    m_WideScreenOn; 
	bool    m_1rstPersonRunCloseToAWall;
	bool    m_bHeadBob;
	bool    m_bVehicleSuspenHigh;
	bool    m_bEnable1rstPersonCamCntrlsScript; 

	bool    m_bAllow1rstPersonWeaponsCamera;
	bool    m_bFailedCullZoneTestPreviously;
	bool    m_FadeTargetIsSplashScreen;//used as hack for fading 
	bool    WorldViewerBeingUsed; // To indicate if the world viewer is being used.                                      
	uint8   ActiveCam;              // Which one at the moment (0 or 1)
                                        // Their is a fudge at the end when the renderware matrix will receive either
                                        // the active camera or the worldviewer camera
	uint32    m_uiCamShakeStart;          // When did the camera shake start.
	uint32    m_uiFirstPersonCamLastInputTime;
	uint32    m_uiLongestTimeInMill;
	uint32    m_uiNumberOfTrainCamNodes;

	uint8     m_uiTransitionJUSTStarted;  // This is the first frame of a transition.
	uint8     m_uiTransitionState;        // 0:one mode 1:transition
	uint32    m_uiTimeLastChange;
	uint32    m_uiTimeWeLeftIdle_StillNoInput;
	uint32    m_uiTimeWeEnteredIdle;
	uint32    m_uiTimeTransitionStart;    // When was the transition started ?
	uint32    m_uiTransitionDuration;     // How long does the transition take ?
	uint32    m_uiTransitionDurationTargetCoors;
	int     m_BlurBlue;
	int     m_BlurGreen;
	int     m_BlurRed;
	int     m_BlurType;
	int     m_iWorkOutSpeedThisNumFrames;//duh  
	int     m_iNumFramesSoFar; //counter
	int     m_iCurrentTrainCamNode;//variable indicating which camera node we are at for the train
	int     m_motionBlur;//to indicate that we are fading

	int     m_imotionBlurAddAlpha;
	int     m_iCheckCullZoneThisNumFrames; 
	int     m_iZoneCullFrameNumWereAt;
	int     WhoIsInControlOfTheCamera; //to discern between obbe and scripts
	float   CamFrontXNorm, CamFrontYNorm;
	float   CarZoomIndicator;
	float   CarZoomValue;
	float   CarZoomValueSmooth;
	float   DistanceToWater;
	float   FOVDuringInter;
	float   LODDistMultiplier;  // This takes into account the FOV and the standard LOD multiplier Smaller aperture->bigger LOD multipliers.
	float   GenerationDistMultiplier;   // This takes into account the FOV but noy the standard LOD multiplier

	float   m_fAlphaSpeedAtStartInter;
	float   m_fAlphaWhenInterPol;
	float   m_fAlphaDuringInterPol;
	float   m_fBetaDuringInterPol;
	float   m_fBetaSpeedAtStartInter;
	float   m_fBetaWhenInterPol;
	float   m_fFOVWhenInterPol;
	float   m_fFOVSpeedAtStartInter;
	float   m_fStartingBetaForInterPol;
	float   m_fStartingAlphaForInterPol;
	float   m_PedOrientForBehindOrInFront;

	float   m_CameraAverageSpeed; //this is an average depending on how many frames we work it out
	float   m_CameraSpeedSoFar; //this is a running total
	float   m_fCamShakeForce;           // How severe is the camera shake.
/**/	float m_fCarZoomValueScript;
	float   m_fFovForTrain;
	float   m_fFOV_Wide_Screen;
	float   m_fNearClipScript;
	float   m_fOldBetaDiff;         // Needed for interpolation between 2 modes
	float   m_fPedZoomValue;
	float   m_fPedZoomValueSmooth;
	float   m_fPedZoomValueScript;
	float   m_fPositionAlongSpline;//Variable used to indicate how far along the spline we are 0-1 for started to completed respectively
	float   m_ScreenReductionPercentage;
	float   m_ScreenReductionSpeed;
	float   m_AlphaForPlayerAnim1rstPerson;
	float   Orientation;            // The orientation of the camera. Used for peds walking.
	float   PedZoomIndicator;
	float   PlayerExhaustion;       // How tired is player (inaccurate sniping) 0.0f-1.0f
                    // The following things are used by the sound code to
                    // play reverb depending on the surroundings. From a point
                    // in front of the camera the disance is measured to the
                    // nearest obstacle (building)
	float   SoundDistUp; //, SoundDistLeft, SoundDistRight;     // These ones are buffered and should be used by the audio
	float   SoundDistUpAsRead; //, SoundDistLeftAsRead, SoundDistRightAsRead;
	float   SoundDistUpAsReadOld; //, SoundDistLeftAsReadOld, SoundDistRightAsReadOld;
                    // Very rough distance to the nearest water for the sound to use
                    // Front vector X&Y normalised to 1. Used by loads of stuff.


	float   m_fAvoidTheGeometryProbsTimer;
	int16   m_nAvoidTheGeometryProbsDirn;
 
	float   m_fWideScreenReductionAmount;//0 for not reduced 1 for fully reduced (Variable for Les)
	float   m_fStartingFOVForInterPol;
 
	CCam Cams[3];                // The actual cameras (usually only one of the two is active)
                            // And to complicate this we have a third camera, this camera is 
                            // used for debugging when we want to have a look at the world.
                            // We can't change the camera mode because other objects depend on their

	CGarage *pToGarageWeAreIn;
	CGarage *pToGarageWeAreInForHackAvoidFirstPerson;
	CQueuedMode m_PlayerMode;

	// The higher priority player camera mode. This one is used
	// for the sniper mode and rocket launcher mode.
	// This one overwrites the m_PlayerMode above.
	CQueuedMode PlayerWeaponMode;
	CVector m_PreviousCameraPosition; //needed to work out speed
	CVector m_RealPreviousCameraPosition; // This cane be used by stuff outside the camera code. The one above is the same as the current coordinates outwidth the camera code.
                                        // an active camera for range finding etc
	CVector m_cvecAimingTargetCoors;        // Coors to look at with Gordons aiming thing
	// The player camera that is waiting to be used
	// This camera can replace the default camera where this is
	// needed (in tricky situations like tunnels for instance)
	CVector m_vecFixedModeVector;
	CVector m_vecFixedModeSource;
	CVector m_vecFixedModeUpOffSet;
	CVector m_vecCutSceneOffset;

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

	CVector SourceDuringInter, TargetDuringInter, UpDuringInter;
	// RenderWare camera pointer
	RwCamera* m_pRwCamera; // was RwCamera *
	///stuff for cut scenes
	CEntity *pTargetEntity;
	CCamPathSplines m_arrPathArray[MAX_NUM_OF_SPLINETYPES]; //These only get created when the script calls the load splines function
	CMatrix m_cameraMatrix;
	bool    m_bGarageFixedCamPositionSet;
	bool    m_vecDoingSpecialInterPolation;
	bool    m_bScriptParametersSetForInterPol;
	bool    m_bFading;//to indicate that we are fading 
	bool    m_bMusicFading;
	CMatrix m_viewMatrix;
	CVector m_vecFrustumNormals[4];
	CVector m_vecOldSourceForInter;
	CVector m_vecOldFrontForInter;
	CVector m_vecOldUpForInter;
	float   m_vecOldFOVForInter;
	float   m_fFLOATingFade;//variable representing the FLOAT version of CDraw::Fade. Necessary to stop loss of precision
	float   m_fFLOATingFadeMusic;
	float   m_fTimeToFadeOut;
	float   m_fTimeToFadeMusic;
	float   m_fFractionInterToStopMoving; 
	float   m_fFractionInterToStopCatchUp;
	float   m_fFractionInterToStopMovingTarget; 
	float   m_fFractionInterToStopCatchUpTarget;

	float   m_fGaitSwayBuffer;
	float   m_fScriptPercentageInterToStopMoving;
	float   m_fScriptPercentageInterToCatchUp;
	uint32  m_fScriptTimeForInterPolation;

	int16   m_iFadingDirection;
	int     m_iModeObbeCamIsInForCar;
	int16   m_iModeToGoTo;
	int16   m_iMusicFadingDirection;
	int16   m_iTypeOfSwitch;

	uint32  m_uiFadeTimeStarted;
	uint32  m_uiFadeTimeStartedMusic;

	void InitialiseCameraForDebugMode(void);
};
static_assert(sizeof(CCamera) == 0x958, "CCamera: size error");
extern CCamera &TheCamera;

CVehicle *FindPlayerVehicle(void);
CPlayerPed *FindPlayerPed(void);
void FindPlayerCoors(CVector*);

extern CStreamingInfo *CStreaming__ms_aInfoForModel;
void CStreaming__RequestModel(int id, int flags);
void CStreaming__LoadAllRequestedModels(bool b);
void CStreaming__SetModelIsDeletable(int id);
void CStreaming__SetModelTxdIsDeletable(int id);

CVehicle *CVehicle__new(int size);
void CWorld__Add(CEntity *e);
