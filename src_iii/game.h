// III

class CGeneral
{
public:
	static double GetATanOfXY(float x, float y);
};


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
	uint8 surfaceA;
	uint8 pieceA;
	uint8 surfaceB;
	uint8 pieceB;
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
	static bool IsBoatModel(int index);
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
	int32 uAudioEntityId;
	float unk1;
	void *carTreadable;
	void *pedTreadable;
	uint32 m_nLastTimeCollided;
	CVector m_vecMoveSpeed;
	CVector m_vecTurnSpeed;
	CVector m_vecMoveFriction;
	CVector m_vecTurnFriction;
	CVector m_vecMoveSpeedAvg;
	CVector m_vecTurnSpeedAvg;
	float m_fMass;
	float m_fTurnMass;
	float fForceMultiplier;
	float m_fAirResistance;
	float fElasticity;
	float fPercentSubmerged;
	CVector m_vecCentreOfMass;
	void *entryInfoListNext;
	void *pMovingListNode;
	uint8 field_EC;
	uint8 m_nStaticFrames;
	uint8 m_nCollisionRecords;
	uint8 m_unk2;
	CEntity *m_aCollisionRecords[6];
	float m_fDistanceTravelled;
	float fCollisionPower;
	float pPhysColliding;
	CVector vecCollisionPower;
	int16 wComponentCol;
	uint8 byteMoveFlags;
	uint8 byteCollFlags;
	uint8 byteLastCollType;
	uint8 byteZoneLevel;
	int16 __padding;

	void ProcessControl(void);
	CVector GetSpeed(CVector &v) { return m_vecMoveSpeed + m_vecMoveFriction + CrossProduct(m_vecTurnFriction + m_vecTurnSpeed, v); }
};
static_assert(sizeof(CPhysical) == 0x128, "CPhysical: wrong size");

struct AnimBlendFrameData;
struct CPed;

struct LimbOrientation
{
	float phi;
	float theta;
};

struct CPedIK
{
	CPed *ped;
	LimbOrientation headOrient;
	LimbOrientation torsoOrient;
	LimbOrientation upperArmOrient;
	LimbOrientation lowerArmOrient;
	int flags;
};

struct CCollPoly
{
	CVector vecColPolyPoint1;
	CVector vecColPolyPoint2;
	CVector vecColPolyPoint3;
	char bIsValidCollision;
	char __fx0025[3];
};

struct CWeapon
{
	int nWeaponId;
	int nWeaponState;
	int nAmmoLoaded;
	int nAmmoTotal;
	int dwNextShotTime;
	char bAddRotOffset;
	char __f0015[3];
};

struct CPed : CPhysical
{
	CCollPoly polyColliding;
	int fCollisionSpeed;
	char bfFlagsA;
	char bfFlagsB;
	char bfFlagsC;
	char bfFlagsD;
	char bfFlagsE;
	char bfFlagsF;
	char bfFlagsG;
	char bfFlagsH;
	char someFlags;
	char pad_15D[3];
	char bytePedStatus;
	char field_161;
	char pad_162[2];
	int dwObjective;
	int dwPrevObjective;
	int field_16C;
	int field_170;
	int field_174;
	int field_178;
	int field_17C;
	int field_180;
	int dwPedFormation;
	int dwFearFlags;
	int pThreatEntity;
	int fEventOrThreatX;
	int fEventOrThreatY;
	int dwEventType;
	int pEventEntity;
	int fAngleToEvent;
	AnimBlendFrameData *pFrames[12];
	int dwAnimGroupId;
	int pVehicleAnim;
	CVector2D vecAnimMoveDelta;
	CVector vecOffsetSeek;
	CPedIK stPedIK;
	int fActionX;
	int fActionY;
	int dwActionTimer;
	int dwAction;
	int dwLastAction;
	int dwMoveState;
	int dwStoredActionState;
	int dwPrevActionState;
	int dwWaitState;
	int dwWaitTimer;
	int pPathNodesStates[8];
	char stPathNodeStates[80];
	__int16 wPathNodes;
	__int16 wCurPathNode;
	char bytePathState;
	char pad_2B5[3];
	int pNextPathNode;
	int pLastPathNode;
	int fHealth;
	int fArmour;
	__int16 wRouteLastPoint;
	__int16 wRoutePoints;
	__int16 wRoutePos;
	__int16 wRouteType;
	__int16 wRouteCurDir;
	__int16 field_2D2;
	int fMovedX;
	int fMovedY;
	int fRotationCur;
	int fRotationDest;
	float headingRate;
	__int16 wEnterType;
	__int16 wWalkAroundType;
	int pCurPhysSurface;
	CVector vecOffsetFromPhysSurface;
	int pCurSurface;
	CVector vecSeekVehicle;
	int pSeekTarget;
	int pVehicle;
	char byteIsInVehicle;
	char pad_315[3];
	int field_318;
	char field_31C;
	char field_31D;
	__int16 wPhoneId;
	int dwLookingForPhone;
	int dwPhoneTalkTimer;
	int pLastAccident;
	int dwPedType;
	float *pPedStats;
	int fFleeFromPosX;
	int fFleeFromPosY;
	int pFleeFrom;
	int dwFleeTimer;
	int field_344;
	int dwLastThreatTimer;
	int pVehicleColliding;
	char byteStateUnused;
	char pad_351[3];
	int dwTimerUnused;
	int pTargetUnused;
	CWeapon weapons[13];
	int field_494;
	char currentWeapon;
	char field_499;
	char byteWepSkills;
	char byteWepAccuracy;
	int pPointGunAt;
	CVector vecHitLastPos;
	int dwLastHitState;
	char byteFightFlags1;
	char byteFightFlags2;
	char pad_4B2[2];
	int pPedFire;
	int pPedFight;
	int fLookDirection;
	int dwWepModelID;
	int dwLeaveCarTimer;
	int dwGetUpTimer;
	int dwLookTimer;
	int dwStandardTimer;
	int dwAttackTimer;
	int dwLastHitTime;
	int dwHitRecoverTimer;
	int field_4E0;
	int field_4E4;
	int field_4E8;
	int field_4EC;
	char field_4F0;
	char field_4F1;
	char byteBodyPartBleeding;
	char field_4F3;
	int pNearPeds[10];
	__int16 nNearPeds;
	char byteLastDamWep;
	char pad_51F;
	char field_520;
	char pad_521[3];
	int dwTalkTimer;
	__int16 wTalkTypeLast;
	__int16 wTalkType;
	CVector vecSeekPosEx;
	int fSeekExAngle;
	int field_53C;
	bool IsPedInControl(void);
};
static_assert(offsetof(CPed, weapons) == 0x35C, "CPed: error");


struct tHandlingData;

struct CVehicle : CPhysical
{
  tHandlingData *handling;
  DWORD dword12C;
  DWORD dword130;
  DWORD dword134;
  DWORD dword138;
  DWORD dword13C;
  DWORD dword140;
  DWORD dword144;
  DWORD dword148;
  DWORD dword14C;
  DWORD dword150;
  BYTE f154[1];
  BYTE byte155;
  BYTE byte156;
  BYTE byte157;
  BYTE byte158;
  BYTE byte159;
  BYTE byte15A;
  BYTE m_bAnimationId;
  DWORD m_dwAnimationTime;
  DWORD dword160;
  BYTE byte164;
  BYTE byte165;
  BYTE f166[46];
  WORD word194;
  BYTE f196[2];
  DWORD dword198;
  char m_bCarColorId[2];
  char m_bExtra[2];
  WORD word1A0;
  BYTE f1A2[2];
  DWORD m_pDriver;
  int m_pPassenger[8];
  BYTE m_bNumPassengers;
  BYTE byte1C9;
  BYTE byte1CA;
  BYTE byte1CB;
  BYTE maxNumPassengers;
  BYTE f1CD[19];
  CEntity *m_pRoad;
  DWORD m_pCarFire;
  float m_fWheelAngle;
  float m_fGasPedal;
  float m_fBreakPedal;
  BYTE byte1F4;
  BYTE field_1F5;
  BYTE byte1F6;
  BYTE byte1F7;
  BYTE byte1F8;
  BYTE byte1F9;
  BYTE f1FA[1];
  BYTE byte1FB;
  BYTE f1FC[4];
  DWORD m_fHealth;
  BYTE byte204;
  BYTE f205[3];
  DWORD dword208;
  DWORD dword20C;
  DWORD dword210;
  WORD word214;
  WORD word216;
  DWORD dword218;
  float float21C;
  float float220;
  DWORD dword224;
  BYTE byte228;
  BYTE byte229;
  BYTE byte22A;
  BYTE byte22B;
  BYTE m_bCarHornTimer;
  BYTE byte22D;
  BYTE m_bSirenOrAlarm;
  BYTE byte22F;
  BYTE f230[36];
  BYTE byte254;
  BYTE f255[39];
  BYTE byte27C;
  BYTE f27D[3];
  DWORD m_fSteering;
  int m_dwVehicleSubType;
};
static_assert(sizeof(CVehicle) == 0x288, "CVehicle: wrong size");

struct CAutomobile : CVehicle
{
	int pad;

	CAutomobile *ctor(int id, uint8 type);
};

struct CBoat : CVehicle
{
	int pad;
	CBoat *ctor(int id, uint8 type);
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
float m_unknownZOffset;
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
	void Process_Kalvin(float*, float, float, float);
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
	uint32 m_uiFirstPersonCamLastInputTime;
// where are those?
//bool m_bVehicleSuspenHigh;
//bool m_bEnable1rstPersonCamCntrlsScript;
//bool m_bAllow1rstPersonWeaponsCamera;

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

	bool GetLookBehindForCar(void);
	bool GetLookBehindForPed(void);
	bool GetLookLeft(void);
	bool GetLookRight(void);

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
CVehicle *FindPlayerVehicle(void);
void FindPlayerCoors(CVector*);
