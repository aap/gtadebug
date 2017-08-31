#include "debug.h"

struct DebugLine
{
	CVector v1;
	CVector v2;
	uint col1;
	uint col2;
};
#define MAXDEBUGLINES 20000
DebugLine debugLines[MAXDEBUGLINES];
int numDebuglines;

static int doRenderPedCol;
static int doRenderBuildingCol;
static int doRenderVehicleCol;
static int doHitColModel;
static int doDrawBBoxes = 1;
static int doDrawBoxes = 1;
static int doDrawSpheres = 1;
static int doDrawTriangles = 1;
static float colDrawDist = 100.0f;

WRAPPER void CLines__RenderLineWithClipping(float x1, float y1, float z1, float x2, float y2, float z2, uint c1, uint c2) { EAXJMP(0x50A3B0); }

void
addDebugLine(float x1, float y1, float z1, float x2, float y2, float z2, uint c1, uint c2)
{
	int n;
	if(numDebuglines >= MAXDEBUGLINES)
		return;
	n = numDebuglines++;
	debugLines[n].v1.x = x1;
	debugLines[n].v1.y = y1;
	debugLines[n].v1.z = z1;
	debugLines[n].v2.x = x2;
	debugLines[n].v2.y = y2;
	debugLines[n].v2.z = z2;
	debugLines[n].col1 = c1;
	debugLines[n].col2 = c2;
}

void (*RenderDebugShit_orig)(void);
void
renderDebugLines(void)
{
	int i;
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)1);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
	for(i = 0; i < numDebuglines; i++)
		CLines__RenderLineWithClipping(debugLines[i].v1.x,
			debugLines[i].v1.y,
			debugLines[i].v1.z,
			debugLines[i].v2.x,
			debugLines[i].v2.y,
			debugLines[i].v2.z,
			debugLines[i].col1, debugLines[i].col2);
	numDebuglines = 0;
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, 0);

	RenderDebugShit_orig();
}

//WRAPPER void CTheScripts__ScriptDebugLine3D(float x1, float y1, float z1, float x2, float y2, float z2, uint c1, uint c2) { EAXJMP(0x4534E0); }
//void CTheScripts__ScriptDebugLine3D(RwV3d v1, RwV3d v2, uint c1, uint c2) { CTheScripts__ScriptDebugLine3D(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, c1, c2); }
void CTheScripts__ScriptDebugLine3D(float x1, float y1, float z1, float x2, float y2, float z2, uint c1, uint c2) { addDebugLine(x1, y1, z1, x2, y2, z2, c1, c2); }
void CTheScripts__ScriptDebugLine3D(RwV3d v1, RwV3d v2, uint c1, uint c2) { CTheScripts__ScriptDebugLine3D(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, c1, c2); }

void
renderBox(CColBox *box, uint col, RwMatrix *mat)
{
	RwV3d verts[8];
	verts[0].x = box->min.x;
	verts[0].y = box->min.y;
	verts[0].z = box->min.z;
	verts[1].x = box->max.x;
	verts[1].y = box->min.y;
	verts[1].z = box->min.z;
	verts[2].x = box->min.x;
	verts[2].y = box->max.y;
	verts[2].z = box->min.z;
	verts[3].x = box->max.x;
	verts[3].y = box->max.y;
	verts[3].z = box->min.z;
	verts[4].x = box->min.x;
	verts[4].y = box->min.y;
	verts[4].z = box->max.z;
	verts[5].x = box->max.x;
	verts[5].y = box->min.y;
	verts[5].z = box->max.z;
	verts[6].x = box->min.x;
	verts[6].y = box->max.y;
	verts[6].z = box->max.z;
	verts[7].x = box->max.x;
	verts[7].y = box->max.y;
	verts[7].z = box->max.z;
	RwV3dTransformPoints(verts, verts, 8, mat);

	CTheScripts__ScriptDebugLine3D(verts[0], verts[1], col, col);
	CTheScripts__ScriptDebugLine3D(verts[1], verts[3], col, col);
	CTheScripts__ScriptDebugLine3D(verts[3], verts[2], col, col);
	CTheScripts__ScriptDebugLine3D(verts[2], verts[0], col, col);

	CTheScripts__ScriptDebugLine3D(verts[0+4], verts[1+4], col, col);
	CTheScripts__ScriptDebugLine3D(verts[1+4], verts[3+4], col, col);
	CTheScripts__ScriptDebugLine3D(verts[3+4], verts[2+4], col, col);
	CTheScripts__ScriptDebugLine3D(verts[2+4], verts[0+4], col, col);

	CTheScripts__ScriptDebugLine3D(verts[0], verts[4], col, col);
	CTheScripts__ScriptDebugLine3D(verts[1], verts[5], col, col);
	CTheScripts__ScriptDebugLine3D(verts[2], verts[6], col, col);
	CTheScripts__ScriptDebugLine3D(verts[3], verts[7], col, col);
}

void
renderSphere(CColSphere *sphere, uint col, RwMatrix *matrix)
{
	RwV3d c;
	RwV3d verts[6];
	RwV3dTransformPoints(&c, (RwV3d*)&sphere->center, 1, matrix);
	verts[0] = verts[1] = verts[2] = verts[3] = verts[4] = verts[5] = c;
	verts[0].z += sphere->radius;	// top
	verts[1].z -= sphere->radius;	// bottom
/*
	verts[2].x += sphere->radius;
	verts[2].y += sphere->radius;
	verts[3].x += sphere->radius;
	verts[3].y -= sphere->radius;
	verts[4].x -= sphere->radius;
	verts[4].y += sphere->radius;
	verts[5].x -= sphere->radius;
	verts[5].y -= sphere->radius;
*/
	verts[2].x += sphere->radius;
	verts[3].x -= sphere->radius;
	verts[4].y += sphere->radius;
	verts[5].y -= sphere->radius;

	CTheScripts__ScriptDebugLine3D(verts[0], verts[2], col, col);
	CTheScripts__ScriptDebugLine3D(verts[0], verts[3], col, col);
	CTheScripts__ScriptDebugLine3D(verts[0], verts[4], col, col);
	CTheScripts__ScriptDebugLine3D(verts[0], verts[5], col, col);
	CTheScripts__ScriptDebugLine3D(verts[1], verts[2], col, col);
	CTheScripts__ScriptDebugLine3D(verts[1], verts[3], col, col);
	CTheScripts__ScriptDebugLine3D(verts[1], verts[4], col, col);
//	CTheScripts__ScriptDebugLine3D(verts[1], verts[5], col, col);
//	CTheScripts__ScriptDebugLine3D(verts[2], verts[3], col, col);
//	CTheScripts__ScriptDebugLine3D(verts[3], verts[5], col, col);
//	CTheScripts__ScriptDebugLine3D(verts[5], verts[4], col, col);
}

void
renderTriangle(CColTriangle *tri, CVector *v, uint col, RwMatrix *matrix)
{
	RwV3d verts[3];
	verts[0] = *(RwV3d*)&v[tri->a];
	verts[1] = *(RwV3d*)&v[tri->b];
	verts[2] = *(RwV3d*)&v[tri->c];
	RwV3dTransformPoints(verts, verts, 3, matrix);
	CTheScripts__ScriptDebugLine3D(verts[0], verts[1], col, col);
	CTheScripts__ScriptDebugLine3D(verts[1], verts[2], col, col);
	CTheScripts__ScriptDebugLine3D(verts[2], verts[0], col, col);
}

void
renderCol(CColModel *col, CMatrix *matrix)
{
	int i;
	if(doDrawBBoxes)
		renderBox(&col->boundingBox, 0xFF0000FF, &matrix->matrix);
	if(doDrawBoxes)
		for(i = 0; i < col->numBoxes; i++)
			renderBox(&col->boxes[i], 0xFFFFFFFF, &matrix->matrix);
	if(doDrawSpheres)
		for(i = 0; i < col->numSpheres; i++)
			renderSphere(&col->spheres[i], 0xFF00FFFF, &matrix->matrix);
	if(doDrawTriangles)
		for(i = 0; i < col->numTriangles; i++)
			renderTriangle(&col->triangles[i], col->vertices, 0x00FF00FF, &matrix->matrix);
}

void
renderEntityCol(CEntity *e)
{
	RwV3d v, campos;
	v = e->matrix.matrix.pos;
	campos = RwFrameGetLTM(RwCameraGetFrame((RwCamera*)((RwGlobals*)RwEngineInstance)->curCamera))->pos;
	RwV3dSub(&v, &v, &campos);
	float dist = RwV3dDotProduct(&v, &v);
	CBaseModelInfo *mi = (CBaseModelInfo*)CModelInfo::ms_modelInfoPtrs[e->nModelIndex];
	if(mi->type == 6 && doHitColModel){	// Ped{
		CPedModelInfo *pmi = (CPedModelInfo*)mi;
		renderCol(pmi->hitColModel, &e->matrix);
	}else
	if(sqrt(dist) < colDrawDist)
		renderCol(mi->colModel, &e->matrix);
}

static void (__thiscall *pedRender_)(CEntity *e);
static void (__thiscall *civilianpedRender_)(CEntity *e);
static void (__thiscall *coppedRender_)(CEntity *e);
static void (__thiscall *emergencypedRender_)(CEntity *e);
static void (__thiscall *playerpedRender_)(CEntity *e);

static void (__thiscall *buildingRender_)(CEntity *e);
static void (__thiscall *treadableRender_)(CEntity *e);

static void (__thiscall *vehicleRender_)(CEntity *e);
static void (__thiscall *automobileRender_)(CEntity *e);
static void (__thiscall *boatRender_)(CEntity *e);
static void (__thiscall *heliRender_)(CEntity *e);
static void (__thiscall *planeRender_)(CEntity *e);
static void (__thiscall *trainRender_)(CEntity *e);


struct EntityRender : CEntity
{
	void pedRender(void) {
		pedRender_(this);
		if(doRenderPedCol)
			renderEntityCol(this);
	}
	void civilianpedRender(void) {
		civilianpedRender_(this);
		if(doRenderPedCol)
			renderEntityCol(this);
	}
	void coppedRender(void) {
		coppedRender_(this); 
		if(doRenderPedCol)
			renderEntityCol(this);
	}
	void emergencypedRender(void) {
		emergencypedRender_(this);
		if(doRenderPedCol)
			renderEntityCol(this);
	}
	void playerpedRender(void) {
		playerpedRender_(this);
		if(doRenderPedCol)
			renderEntityCol(this);
	}

	void buildingRender(void) {
		buildingRender_(this);
		if(doRenderBuildingCol)
			renderEntityCol(this);
	}
	void treadableRender(void) {
		treadableRender_(this);
		if(doRenderBuildingCol)
			renderEntityCol(this);
	}

	void vehicleRender(void) {
		vehicleRender_(this);
		if(doRenderVehicleCol)
			renderEntityCol(this);
	}
	void automobileRender(void) {
		automobileRender_(this);
		if(doRenderVehicleCol)
			renderEntityCol(this);
	}
	void boatRender(void) {
		boatRender_(this);
		if(doRenderVehicleCol)
			renderEntityCol(this);
	}
	void heliRender(void) {
		heliRender_(this);
		if(doRenderVehicleCol)
			renderEntityCol(this);
	}
	void planeRender(void) {
		planeRender_(this);
		if(doRenderVehicleCol)
			renderEntityCol(this);
	}
	void trainRender(void) {
		trainRender_(this);
		if(doRenderVehicleCol)
			renderEntityCol(this);
	}
};

void
installColDebug(void)
{
	InterceptVmethod(&pedRender_, &EntityRender::pedRender, 0x5F8C60);
	InterceptVmethod(&civilianpedRender_, &EntityRender::civilianpedRender, 0x5F81D0);
	InterceptVmethod(&coppedRender_, &EntityRender::coppedRender, 0x5F82D8);
	InterceptVmethod(&emergencypedRender_, &EntityRender::emergencypedRender, 0x5F83A8);
	InterceptVmethod(&playerpedRender_, &EntityRender::playerpedRender, 0x5FA534);

	InterceptVmethod(&buildingRender_, &EntityRender::buildingRender, 0x5EBF78);
	InterceptVmethod(&treadableRender_, &EntityRender::treadableRender, 0x5EBFE8);

	InterceptVmethod(&vehicleRender_, &EntityRender::vehicleRender, 0x6028DC);
	InterceptVmethod(&automobileRender_, &EntityRender::automobileRender, 0x600C50);
	InterceptVmethod(&boatRender_, &EntityRender::boatRender, 0x600ED8);
	InterceptVmethod(&heliRender_, &EntityRender::heliRender, 0x601EE4);
	InterceptVmethod(&planeRender_, &EntityRender::planeRender, 0x602210);
	InterceptVmethod(&trainRender_, &EntityRender::trainRender, 0x602450);

	InterceptCall(&RenderDebugShit_orig, renderDebugLines, 0x48E5FE);

	DebugMenuAddVarBool32("Col Renderer", "Render building cols", &doRenderBuildingCol, nil);
	DebugMenuAddVarBool32("Col Renderer", "Render vehicle cols", &doRenderVehicleCol, nil);
	DebugMenuAddVarBool32("Col Renderer", "Render ped cols", &doRenderPedCol, nil);
	DebugMenuAddVarBool32("Col Renderer", "Render ped hit colmodel", &doHitColModel, nil);
	DebugMenuAddVarBool32("Col Renderer", "Render bounding boxes", &doDrawBBoxes, nil);
	DebugMenuAddVarBool32("Col Renderer", "Render col boxes", &doDrawBoxes, nil);
	DebugMenuAddVarBool32("Col Renderer", "Render col spheres", &doDrawSpheres, nil);
	DebugMenuAddVarBool32("Col Renderer", "Render col triangles", &doDrawTriangles, nil);

	DebugMenuAddVar("Col Renderer", "Draw distance", &colDrawDist, nil, 10.0f, 10.0f, 1000.0f);

}
