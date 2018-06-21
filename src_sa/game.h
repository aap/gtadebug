// SA

class CGeneral
{
public:
	static double GetATanOfXY(float x, float y);
};

struct CTimer
{
	static float &ms_fTimeScale;
};

struct CClock
{
	static int16 &ms_nGameClockSeconds;
	static uint8 &ms_nGameClockMinutes;
	static uint8 &ms_nGameClockHours;
};

struct CWeather
{
	static int16 &OldWeatherType;
	static int16 &NewWeatherType;
	static float &InterpolationValue;
};

struct CDraw
{
	static void SetFOV(float fov);
};

struct CIplStore
{
	static void AddIplsNeededAtPosn(CVector *pos);
};

/*
 * Input stuff
 */

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
	short m_bPedWalk;
	short m_bVehicleMouseLook;
	short m_bRadioTrackSkip;
};
static_assert(sizeof(CControllerState) == 0x30, "CControllerState: wrong size");

struct CMouseControllerState
{
	uchar lmb;
	uchar rmb;
	uchar mmb;
	uchar wheelUp;
	uchar wheelDown;
	uchar bmx1;
	uchar bmx2;
	float Z;
	float X;
	float Y;
};

class CPad
{
public:
	CControllerState	NewState;
	CControllerState	OldState;
	WORD			SteeringLeftRightBuffer[10];
	DWORD			DrunkDrivingBufferUsed;
	CControllerState	PCTempKeyState;
	CControllerState	PCTempJoyState;
	CControllerState	PCTempMouseState;
	BYTE			Phase;
	WORD			Mode;
	short			ShakeDur;
	WORD			DisablePlayerControls;
	BYTE			ShakeFreq;
	BYTE			bHornHistory[5];
	BYTE			iCurrHornHistory;
	bool			JustOutOfFrontEnd;
	bool			bApplyBrakes;
	bool			bDisablePlayerEnterCar;
	bool			bDisablePlayerDuck;
	bool			bDisablePlayerFireWeapon;
	bool			bDisablePlayerFireWeaponWithL1;
	bool			bDisablePlayerCycleWeapon;
	bool			bDisablePlayerJump;
	bool			bDisablePlayerDisplayVitalStats;
	int			LastTimeTouched;
	DWORD			AverageWeapon;
	DWORD			AverageEntries;
	DWORD			NoShakeBeforeThis;
	BYTE			NoShakeFreq;

	static CMouseControllerState &NewMouseControllerState;
	static CMouseControllerState &OldMouseControllerState;

	static CPad *GetPad(int);
};
static_assert(sizeof(CPad) == 0x134, "CPad: wrong size");

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








struct CSimpleTransform
{
	CVector pos;
	float angle;
};

struct CPlaceable
{
	void *vtable;
	CSimpleTransform placement;
	CMatrix *m_pCoords;
	CVector GetPosition(void){
		if(m_pCoords)
			return *(CVector*)&m_pCoords->matrix.pos;
		else
			return placement.pos;
	}
	void SetPosition(CVector &pos){
		if(m_pCoords)
			*(CVector*)&m_pCoords->matrix.pos = pos;
		else
			placement.pos = pos;
	}
};

struct CEntity : public CPlaceable
{
};

struct CPhysical : public CEntity
{
};

struct CVehicle : public CPhysical
{
};

struct CAutomobile : public CVehicle
{
};

struct CPed : public CPhysical
{
};

struct CPlayerPed : public CPed
{
	static void (__thiscall *ProcessControl)(CPlayerPed*);
	void ProcessControl_hooked(void);
	void SetWantedLevel(int n);
};

struct CEntity_VTBL
{
	int (__thiscall *__dtor)(int, int);
	int Add_CRect;
	int Add;
	int Remove;
	void (__thiscall *SetIsStatic)(CEntity *, bool);
	void (__thiscall *SetModelIndex)(CPed *, DWORD);
	void (*SetModelIndexNoCreate)(void);
	int (__thiscall *CreateRwObject)(CEntity *);
	void (__thiscall *DeleteRwObject)(CEntity *);
	void (__thiscall *GetBoundRect)(CEntity *);
	int ProcessControl;
	int ProcessCollision;
	int ProcessShift;
	int TestCollision;
	void (__thiscall *Teleport)(CEntity *, float x, float y, float z, bool resetOrientation);
	int SpecialEntityPreCollisionStuff;
	int SpecialEntityCalcCollisionSteps;
	int PreRender;
	int Render;
	int SetupLighting;
	int RemoveLighting;
	int FlagToDestroyWhenNextProcessed;
};




#define NUMBER_OF_VECTORS_FOR_AVERAGE	2
#define CAM_NUM_TARGET_HISTORY          4

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
	float   m_fCameraHeightMultiplier; //used by TwoPlayer_Separate_Cars_TopDown
    
	// ped onfoot zoom distance
	float m_fTargetZoomGroundOne;
	float m_fTargetZoomGroundTwo; // 256
	float m_fTargetZoomGroundThree;
	// ped onfoot alpha angle offset
	float m_fTargetZoomOneZExtra;
	float m_fTargetZoomTwoZExtra;
	float m_fTargetZoomTwoInteriorZExtra; //extra one for interior
	float m_fTargetZoomThreeZExtra;
    
	float m_fTargetZoomZCloseIn;
	float m_fMinRealGroundDist;
	float m_fTargetCloseInDist;

	// For targetting in cooperative mode.
	float   Beta_Targeting; // 292
	float   X_Targetting, Y_Targetting;
	int CarWeAreFocussingOn; //which car is closer to the camera in 2 player cooperative mode with separate cars.
	float   CarWeAreFocussingOnI; //interpolated version
    
	float m_fCamBumpedHorz; // 312
	float m_fCamBumpedVert;
	int m_nCamBumpedTime; // 320
	static int CAM_BUMPED_SWING_PERIOD;
	static int CAM_BUMPED_END_TIME;
	static float CAM_BUMPED_DAMP_RATE;
	static float CAM_BUMPED_MOVE_MULT;

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

	CVector m_aTargetHistoryPos[CAM_NUM_TARGET_HISTORY];
	DWORD m_nTargetHistoryTime[CAM_NUM_TARGET_HISTORY];
	DWORD m_nCurrentHistoryPoints;

	CEntity *CamTargetEntity;

	float       m_fCameraDistance;
	float       m_fIdealAlpha;
	float       m_fPlayerVelocity;
	//CVector TempRight;
	CAutomobile  *m_pLastCarEntered; // So interpolation works
	CPed         *m_pLastPedLookedAt;// So interpolation works 
	bool        m_bFirstPersonRunAboutActive;


	void Process(void);
	void Process_Debug(float*, float, float, float);
	void GetVectorsReadyForRW(void);
};
static_assert(sizeof(CCam) == 0x238, "CCam: size error");

struct CGarage;
struct Surface;

#define NUMBER_OF_VECTORS_FOR_AVERAGE 2
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

struct CCamera : public CPlaceable
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
	bool    m_bCooperativeCamMode;
	bool    m_bAllowShootingWith2PlayersInCar;
	bool    m_bDisableFirstPersonInCar;
	static bool m_bUseMouse3rdPerson;
#if 0
//#ifndef FINALBUILD
	bool    bStaticFrustum;
#endif  

    // for debug keyboard stuff
#if 0
//#ifndef MASTER
	    unsigned char display_kbd_debug;
	    float kbd_fov_value;
#endif // MASTER

        // The following fields allow the level designers to specify the camera for 2 player games.
	int16   m_ModeForTwoPlayersSeparateCars;
	int16   m_ModeForTwoPlayersSameCarShootingAllowed;
	int16   m_ModeForTwoPlayersSameCarShootingNotAllowed;
	int16   m_ModeForTwoPlayersNotBothInCar;

	bool    m_bGarageFixedCamPositionSet;
	bool    m_vecDoingSpecialInterPolation;
	bool    m_bScriptParametersSetForInterPol;

    
	bool    m_bFading;//to indicate that we are fading 
	bool    m_bMusicFading;
	bool    m_bMusicFadedOut;

	bool    m_bFailedCullZoneTestPreviously;
	bool    m_FadeTargetIsSplashScreen;//used as hack for fading 
	bool    WorldViewerBeingUsed; // To indicate if the world viewer is being used.                                      

    
	uint8   m_uiTransitionJUSTStarted;  // This is the first frame of a transition.
	uint8   m_uiTransitionState;        // 0:one mode 1:transition
	uint8   ActiveCam;              // Which one at the moment (0 or 1)
                                        // Their is a fudge at the end when the renderware matrix will receive either
                                        // the active camera or the worldviewer camera
	uint32    m_uiCamShakeStart;          // When did the camera shake start.
	uint32    m_uiFirstPersonCamLastInputTime;
	uint32    m_uiLongestTimeInMill;
	uint32    m_uiNumberOfTrainCamNodes;
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


//	float   CarZoomIndicator;
//	float   CarZoomValue;
//	float   CarZoomValueSmooth;
//	float   m_fCarZoomValueScript;
//	float   PedZoomIndicator;
//	float   m_fPedZoomValue;
//	float   m_fPedZoomValueSmooth;
//	float   m_fPedZoomValueScript;
	int     m_nCarZoom;                 // store zoom index
	float   m_fCarZoomBase;             // store base zoom distance from index
	float   m_fCarZoomTotal;            // store total zoom after modded by camera modes
	float   m_fCarZoomSmoothed;         // buffered version of the var above
	float   m_fCarZoomValueScript;
	int     m_nPedZoom;                 // store zoom index
	float   m_fPedZoomBase;             // store base zoom distance from index
	float   m_fPedZoomTotal;            // store total zoom after modded by camera modes
	float   m_fPedZoomSmoothed;         // buffered version of the var above
	float   m_fPedZoomValueScript;


	float   CamFrontXNorm, CamFrontYNorm;
	float   DistanceToWater;
	float   HeightOfNearestWater;
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
	float   m_fFovForTrain;
	float   m_fFOV_Wide_Screen;

	float   m_fNearClipScript;
	float   m_fOldBetaDiff;         // Needed for interpolation between 2 modes
	float   m_fPositionAlongSpline;//Variable used to indicate how far along the spline we are 0-1 for started to completed respectively
	float   m_ScreenReductionPercentage;
	float   m_ScreenReductionSpeed;
	float   m_AlphaForPlayerAnim1rstPerson;
    
	float   Orientation;            // The orientation of the camera. Used for peds walking.
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
    
        // These ones are static so that they don't get cleared in CCamera::Init()
	static  float m_fMouseAccelHorzntl;// acceleration multiplier for 1st person controls
	static  float m_fMouseAccelVertical;// acceleration multiplier for 1st person controls
	static  float m_f3rdPersonCHairMultX;
	static  float m_f3rdPersonCHairMultY;

	CCam Cams[3];                // The actual cameras (usually only one of the two is active)
                            // And to complicate this we have a third camera, this camera is 
                            // used for debugging when we want to have a look at the world.
                            // We can't change the camera mode because other objects depend on their

	// #########################################//
	// DATA NOT UPDATED FOR SA  BELOW HERE!!!!! //
	// #########################################//

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


	CVector m_vecAttachedCamOffset; // for attaching the camera to a ped or vehicle (set by level designers for use in cutscenes)
	CVector m_vecAttachedCamLookAt; 
	float m_fAttachedCamAngle; // for giving the attached camera a tilt.

	// RenderWare camera pointer
	RwCamera* m_pRwCamera; // was RwCamera *
	///stuff for cut scenes
	CEntity *pTargetEntity;
	CEntity *pAttachedEntity;
	//CVector CutScene; 
	CCamPathSplines m_arrPathArray[MAX_NUM_OF_SPLINETYPES]; //These only get created when the script calls the load splines function
	// maybe this shouldn't be here depends if GTA_TRAIN is defined (its not)
	//CTrainCamNode     m_arrTrainCamNode[MAX_NUM_OF_NODES];

	bool m_bMirrorActive;
	bool m_bResetOldMatrix;

	CMatrix m_cameraMatrix;
	CMatrix m_cameraMatrixOld;
	CMatrix m_viewMatrix;
	CMatrix m_matInverse;
	CMatrix m_matMirrorInverse;
	CMatrix m_matMirror;

	CVector m_vecFrustumNormals[4];
	CVector m_vecFrustumWorldNormals[4];
	CVector m_vecFrustumWorldNormals_Mirror[4];

	float m_fFrustumPlaneOffsets[4];
	float m_fFrustumPlaneOffsets_Mirror[4];

	CVector m_vecRightFrustumNormal;
	CVector m_vecBottomFrustumNormal;
	CVector m_vecTopFrustumNormal;

	CVector m_vecOldSourceForInter;
	CVector m_vecOldFrontForInter;
	CVector m_vecOldUpForInter;
	float   m_vecOldFOVForInter;
	float   m_fFLOATingFade;//variable representing the FLOAT version of CDraw::Fade. Necessary to stop loss of precision
	float   m_fFLOATingFadeMusic;
	float   m_fTimeToFadeOut;
	float   m_fTimeToFadeMusic;
	float   m_fTimeToWaitToFadeMusic;
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

	DWORD   m_uiFadeTimeStarted;
	DWORD   m_uiFadeTimeStartedMusic;

	void InitialiseCameraForDebugMode(void);
	void CopyCameraMatrixToRWCam(bool);
};
static_assert(offsetof(CCamera, pToGarageWeAreIn) == 0x81C, "CCamera: error");
extern CCamera &TheCamera;

CVehicle *FindPlayerVehicle(int, bool);
CPlayerPed *FindPlayerPed(int);
