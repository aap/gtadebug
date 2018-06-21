
#define PI M_PI

struct CVector
{
	float x, y, z;
	CVector(void) {}
	CVector(float x, float y, float z) : x(x), y(y), z(z) {}
	CVector(RwV3d const &v) : x(v.x), y(v.y), z(v.z) {}
	float Magnitude(void) const { return sqrtf(x*x + y*y + z*z); }
	float MagnitudeSqr(void) const { return x*x + y*y + z*z; }
	float Magnitude2D(void) const { return sqrtf(x*x + y*y); }
	void Normalise(void){
		float sq = MagnitudeSqr();
		if(sq > 0.0f){
			float invsqrt = 1.0f/sqrtf(sq);
			x *= invsqrt;
			y *= invsqrt;
			z *= invsqrt;
		}else
			x = 0.0f;
	}
	void operator=(RwV3d const &rhs){
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
	}
	CVector operator-(const CVector &rhs) const {
		return CVector(x-rhs.x, y-rhs.y, z-rhs.z);
	}
	CVector operator+(const CVector &rhs) const {
		return CVector(x+rhs.x, y+rhs.y, z+rhs.z);
	}
	CVector operator*(float t) const {
		return CVector(x*t, y*t, z*t);
	}
	CVector operator/(float t) const {
		return CVector(x/t, y/t, z/t);
	}
	CVector &operator-=(const CVector &rhs) {
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		return *this;
	}
	CVector &operator+=(const CVector &rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		return *this;
	}
	CVector &operator*=(float t) {
		this->x *= t;
		this->y *= t;
		this->z *= t;
		return *this;
	}
	CVector &operator/=(float t) {
		this->x /= t;
		this->y /= t;
		this->z /= t;
		return *this;
	}};
inline float
DotProduct(const CVector &v1, const CVector &v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

inline CVector
CrossProduct(const CVector &v1, const CVector &v2)
{
	return CVector(v1.y*v2.z - v1.z*v2.y,
		v1.z*v2.x - v1.x*v2.z,
		v1.x*v2.y - v1.y*v2.x);
}


struct CVector2D
{
	float x, y;
};

struct CMatrix
{
	RwMatrix matrix;
	RwMatrix *matrixPtr;
	int haveRwMatrix;

	void SetRotate(float x, float y, float z);
	CVector &GetForward(void) { return *(CVector*)&matrix.up; }
	CVector &GetUp(void) { return *(CVector*)&matrix.at; }
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
