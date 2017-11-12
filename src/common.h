struct CVector
{
	float x, y, z;

	CVector(void) {}
	CVector(float x, float y, float z) : x(x), y(y), z(z) {}
	CVector operator-(const CVector &rhs) const {
		return CVector(x-rhs.x, y-rhs.y, z-rhs.z);
	}
	CVector operator+(const CVector &rhs) const {
		return CVector(x+rhs.x, y+rhs.y, z+rhs.z);
	}
	CVector operator*(float f) const {
		return CVector(x*f, y*f, z*f);
	}

	static void CrossProduct(CVector *out, CVector *a, CVector *b);
	void Normalise(void);
};

struct CMatrix
{
	RwMatrix matrix;
	RwMatrix *matrixPtr;
	int haveRwMatrix;

	void SetRotate(float x, float y, float z);
};

struct CCam;

struct CameraSetting
{
	CVector Source;
	float Alpha, Beta;
	int hour, minute;
	int oldweather, newweather;
};
extern CameraSetting savedCameras[100];
extern int numSavedCameras;
extern int currentSavedCam;
void LoadSavedCams(void);
void DeleteSavedCams(void);
void SaveCam(CCam *cam);
void LoadCam(CCam *cam);
void NextSavedCam(CCam *cam);
void PrevSavedCam(CCam *cam);

struct CRect;
