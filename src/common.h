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

struct CRect;
