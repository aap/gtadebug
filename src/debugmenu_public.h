
extern "C" {

typedef void (*TriggerFunc)(void);

typedef void (*DebugMenuAddInt8_TYPE)(const char *path, const char *name, int8_t *ptr, TriggerFunc triggerFunc, int8_t step, int8_t lowerBound, int8_t upperBound, const char **strings);
typedef void (*DebugMenuAddInt16_TYPE)(const char *path, const char *name, int16_t *ptr, TriggerFunc triggerFunc, int16_t step, int16_t lowerBound, int16_t upperBound, const char **strings);
typedef void (*DebugMenuAddInt32_TYPE)(const char *path, const char *name, int32_t *ptr, TriggerFunc triggerFunc, int32_t step, int32_t lowerBound, int32_t upperBound, const char **strings);
typedef void (*DebugMenuAddInt64_TYPE)(const char *path, const char *name, int64_t *ptr, TriggerFunc triggerFunc, int64_t step, int64_t lowerBound, int64_t upperBound, const char **strings);
typedef void (*DebugMenuAddUInt8_TYPE)(const char *path, const char *name, uint8_t *ptr, TriggerFunc triggerFunc, uint8_t step, uint8_t lowerBound, uint8_t upperBound, const char **strings);
typedef void (*DebugMenuAddUInt16_TYPE)(const char *path, const char *name, uint16_t *ptr, TriggerFunc triggerFunc, uint16_t step, uint16_t lowerBound, uint16_t upperBound, const char **strings);
typedef void (*DebugMenuAddUInt32_TYPE)(const char *path, const char *name, uint32_t *ptr, TriggerFunc triggerFunc, uint32_t step, uint32_t lowerBound, uint32_t upperBound, const char **strings);
typedef void (*DebugMenuAddUInt64_TYPE)(const char *path, const char *name, uint64_t *ptr, TriggerFunc triggerFunc, uint64_t step, uint64_t lowerBound, uint64_t upperBound, const char **strings);
typedef void (*DebugMenuAddFloat32_TYPE)(const char *path, const char *name, float *ptr, TriggerFunc triggerFunc, float step, float lowerBound, float upperBound);
typedef void (*DebugMenuAddFloat64_TYPE)(const char *path, const char *name, double *ptr, TriggerFunc triggerFunc, double step, double lowerBound, double upperBound);
typedef void (*DebugMenuAddCmd_TYPE)(const char *path, const char *name, TriggerFunc triggerFunc);

struct DebugMenuAPI
{
	DebugMenuAddInt8_TYPE addint8;
	DebugMenuAddInt16_TYPE addint16;
	DebugMenuAddInt32_TYPE addint32;
	DebugMenuAddInt64_TYPE addint64;
	DebugMenuAddUInt8_TYPE adduint8;
	DebugMenuAddUInt16_TYPE adduint16;
	DebugMenuAddUInt32_TYPE adduint32;
	DebugMenuAddUInt64_TYPE adduint64;
	DebugMenuAddFloat32_TYPE addfloat32;
	DebugMenuAddFloat64_TYPE addfloat64;
	DebugMenuAddCmd_TYPE addcmd;
};
extern DebugMenuAPI gDebugMenuAPI;

void DebugMenuAddInt8(const char *path, const char *name, int8_t *ptr, TriggerFunc triggerFunc, int8_t step, int8_t lowerBound, int8_t upperBound, const char **strings)
{ gDebugMenuAPI.addint8(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddInt16(const char *path, const char *name, int16_t *ptr, TriggerFunc triggerFunc, int16_t step, int16_t lowerBound, int16_t upperBound, const char **strings)
{ gDebugMenuAPI.addint16(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddInt32(const char *path, const char *name, int32_t *ptr, TriggerFunc triggerFunc, int32_t step, int32_t lowerBound, int32_t upperBound, const char **strings)
{ gDebugMenuAPI.addint32(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddInt64(const char *path, const char *name, int64_t *ptr, TriggerFunc triggerFunc, int64_t step, int64_t lowerBound, int64_t upperBound, const char **strings)
{ gDebugMenuAPI.addint64(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddUInt8(const char *path, const char *name, uint8_t *ptr, TriggerFunc triggerFunc, uint8_t step, uint8_t lowerBound, uint8_t upperBound, const char **strings)
{ gDebugMenuAPI.adduint8(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddUInt16(const char *path, const char *name, uint16_t *ptr, TriggerFunc triggerFunc, uint16_t step, uint16_t lowerBound, uint16_t upperBound, const char **strings)
{ gDebugMenuAPI.adduint16(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddUInt32(const char *path, const char *name, uint32_t *ptr, TriggerFunc triggerFunc, uint32_t step, uint32_t lowerBound, uint32_t upperBound, const char **strings)
{ gDebugMenuAPI.adduint32(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddUInt64(const char *path, const char *name, uint64_t *ptr, TriggerFunc triggerFunc, uint64_t step, uint64_t lowerBound, uint64_t upperBound, const char **strings)
{ gDebugMenuAPI.adduint64(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddFloat32(const char *path, const char *name, float *ptr, TriggerFunc triggerFunc, float step, float lowerBound, float upperBound)
{ gDebugMenuAPI.addfloat32(path, name, ptr, triggerFunc, step, lowerBound, upperBound); }
void DebugMenuAddFloat64(const char *path, const char *name, double *ptr, TriggerFunc triggerFunc, double step, double lowerBound, double upperBound)
{ gDebugMenuAPI.addfloat64(path, name, ptr, triggerFunc, step, lowerBound, upperBound); }
void DebugMenuAddCmd(const char *path, const char *name, TriggerFunc triggerFunc)
{ gDebugMenuAPI.addcmd(path, name, triggerFunc); }

inline bool DebugMenuLoad(void)
{
	HMODULE mod = LoadLibrary("debugmenu.dll");
	if(mod == 0){
		char modulePath[MAX_PATH];
		HMODULE dllModule;
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&gDebugMenuAPI, &dllModule);
		GetModuleFileName(dllModule, modulePath, MAX_PATH);
		char *p = strrchr(modulePath, '\\');
		if(p) p[1] = '\0';
		strcat(modulePath, "debugmenu.dll");
		mod = LoadLibrary(modulePath);
	}
	if(mod == 0)
		return false;
	gDebugMenuAPI.addint8 = (DebugMenuAddInt8_TYPE)GetProcAddress(mod, "DebugMenuAddInt8");
	gDebugMenuAPI.addint16 = (DebugMenuAddInt16_TYPE)GetProcAddress(mod, "DebugMenuAddInt16");
	gDebugMenuAPI.addint32 = (DebugMenuAddInt32_TYPE)GetProcAddress(mod, "DebugMenuAddInt32");
	gDebugMenuAPI.addint64 = (DebugMenuAddInt64_TYPE)GetProcAddress(mod, "DebugMenuAddInt64");
	gDebugMenuAPI.adduint8 = (DebugMenuAddUInt8_TYPE)GetProcAddress(mod, "DebugMenuAddUInt8");
	gDebugMenuAPI.adduint16 = (DebugMenuAddUInt16_TYPE)GetProcAddress(mod, "DebugMenuAddUInt16");
	gDebugMenuAPI.adduint32 = (DebugMenuAddUInt32_TYPE)GetProcAddress(mod, "DebugMenuAddUInt32");
	gDebugMenuAPI.adduint64 = (DebugMenuAddUInt64_TYPE)GetProcAddress(mod, "DebugMenuAddUInt64");
	gDebugMenuAPI.addfloat32 = (DebugMenuAddFloat32_TYPE)GetProcAddress(mod, "DebugMenuAddFloat32");
	gDebugMenuAPI.addfloat64 = (DebugMenuAddFloat64_TYPE)GetProcAddress(mod, "DebugMenuAddFloat64");
	gDebugMenuAPI.addcmd = (DebugMenuAddCmd_TYPE)GetProcAddress(mod, "DebugMenuAddCmd");
	return true;
}

}

// Also overload them for simplicity

void DebugMenuAddVar(const char *path, const char *name, int8_t *ptr, TriggerFunc triggerFunc, int8_t step, int8_t lowerBound, int8_t upperBound, const char **strings)
{ gDebugMenuAPI.addint8(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddVar(const char *path, const char *name, int16_t *ptr, TriggerFunc triggerFunc, int16_t step, int16_t lowerBound, int16_t upperBound, const char **strings)
{ gDebugMenuAPI.addint16(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddVar(const char *path, const char *name, int32_t *ptr, TriggerFunc triggerFunc, int32_t step, int32_t lowerBound, int32_t upperBound, const char **strings)
{ gDebugMenuAPI.addint32(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddVar(const char *path, const char *name, int64_t *ptr, TriggerFunc triggerFunc, int64_t step, int64_t lowerBound, int64_t upperBound, const char **strings)
{ gDebugMenuAPI.addint64(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddVar(const char *path, const char *name, uint8_t *ptr, TriggerFunc triggerFunc, uint8_t step, uint8_t lowerBound, uint8_t upperBound, const char **strings)
{ gDebugMenuAPI.adduint8(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddVar(const char *path, const char *name, uint16_t *ptr, TriggerFunc triggerFunc, uint16_t step, uint16_t lowerBound, uint16_t upperBound, const char **strings)
{ gDebugMenuAPI.adduint16(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddVar(const char *path, const char *name, uint32_t *ptr, TriggerFunc triggerFunc, uint32_t step, uint32_t lowerBound, uint32_t upperBound, const char **strings)
{ gDebugMenuAPI.adduint32(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddVar(const char *path, const char *name, uint64_t *ptr, TriggerFunc triggerFunc, uint64_t step, uint64_t lowerBound, uint64_t upperBound, const char **strings)
{ gDebugMenuAPI.adduint64(path, name, ptr, triggerFunc, step, lowerBound, upperBound, strings); }
void DebugMenuAddVar(const char *path, const char *name, float *ptr, TriggerFunc triggerFunc, float step, float lowerBound, float upperBound)
{ gDebugMenuAPI.addfloat32(path, name, ptr, triggerFunc, step, lowerBound, upperBound); }
void DebugMenuAddVar(const char *path, const char *name, double *ptr, TriggerFunc triggerFunc, double step, double lowerBound, double upperBound)
{ gDebugMenuAPI.addfloat64(path, name, ptr, triggerFunc, step, lowerBound, upperBound); }