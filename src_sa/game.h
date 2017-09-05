
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
