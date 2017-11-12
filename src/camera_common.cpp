#include <debug.h>

CameraSetting savedCameras[100];
int numSavedCameras;
int currentSavedCam;

void
LoadSavedCams(void)
{
	char line[256];
	CameraSetting *cs;
	FILE *f = fopen("campositions.txt", "r");
	if(f){
		cs = savedCameras;
		numSavedCameras = 0;
		while(fgets(line, 256, f)){
			sscanf(line, "%f %f %f  %f %f  %d %d  %d %d",
				&cs->Source.x, &cs->Source.y, &cs->Source.z,
				&cs->Alpha, &cs->Beta,
				&cs->hour, &cs->minute,
				&cs->oldweather, &cs->newweather);
			cs++;
			numSavedCameras++;
		}
		if(currentSavedCam >= numSavedCameras)
			currentSavedCam = 0;
		fclose(f);
	}
}

void
DeleteSavedCams(void)
{
	FILE *f = fopen("campositions.txt", "w");
	if(f)
		fclose(f);
}

void
SaveCam(CCam *cam)
{
	FILE *f = fopen("campositions.txt", "a");
	if(f){
		fprintf(f, "%f %f %f  %f %f  %d %d  %d %d\n",
			cam->Source.x, cam->Source.y, cam->Source.z,
			cam->Alpha, cam->Beta,
			CClock::ms_nGameClockHours, CClock::ms_nGameClockMinutes,
			CWeather::OldWeatherType, CWeather::NewWeatherType);
		fclose(f);
		LoadSavedCams();
		currentSavedCam = numSavedCameras-1;
	}
}

void
LoadCam(CCam *cam)
{
	if(currentSavedCam < 0 || currentSavedCam >= numSavedCameras)
		return;
	cam->Source = savedCameras[currentSavedCam].Source;
	cam->Alpha = savedCameras[currentSavedCam].Alpha;
	cam->Beta = savedCameras[currentSavedCam].Beta;

	CClock::ms_nGameClockHours = savedCameras[currentSavedCam].hour;
	CClock::ms_nGameClockMinutes = savedCameras[currentSavedCam].minute;
	CWeather::OldWeatherType = savedCameras[currentSavedCam].oldweather;
	CWeather::NewWeatherType = savedCameras[currentSavedCam].newweather;
	CWeather::InterpolationValue = CClock::ms_nGameClockMinutes/60.0f;
}

void
NextSavedCam(CCam *cam)
{
	currentSavedCam++;
	if(currentSavedCam >= numSavedCameras) currentSavedCam = 0;
	LoadCam(cam);
}

void
PrevSavedCam(CCam *cam)
{
	currentSavedCam--;
	if(currentSavedCam < 0) currentSavedCam = numSavedCameras-1;
	LoadCam(cam);
}

