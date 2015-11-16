
/*
	plugin_5_SAU - Andrey Ch 2015
	Compiled 23/09/2015

	This plugin draws three directors for controlling helicopters. There are three channels there. They are roll, pitch and altitude.
	The directors are controlled via parameters of ailerons deflections! 
	In order to control the directors it needs to send a packet of data via UDP.
	The number of packet is 73.
	What is a header of the packet and how to send a packet see documentation about X-Plane or the site:http://www.nuclearprojects.com/xplane/xplaneref.html
	The first of three parameters in this packet are signals to control directors.
	The range of the signals are from -1 to 1. In this case the roll and pitch directors will be within square of mouse control or so.
	The zero signal lie on the center of the screen. 

	The second part of this plugin is a rangefinder.
	In order to control the angle of beam it needs to send a packet of data via UDP.
	The number of packet is 73.
	The fourth and the fifth parameters are two angles: the angle of elevation and azimuth.

	In order to get the data of Distance to ground it needs to receive a packet of data via UDP.
	The number of packet is 71.

	Hot keys:
	!Shift-F1 - Show directors
	!Shift-F5 - Rangefinder
	Shift-F6 - Show beam
	Shift-F7 - Show trace
	Shift-F8 - Show beam TEST
*/

#if IBM
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <queue>

#include "XPLMDefs.h"
#include "XPLMDisplay.h"
#include "XPLMDataAccess.h"
#include "XPLMGraphics.h"
#define XPLM200
#include "XPLMUtilities.h"
#include "XPLMScenery.h"

#include <gl\gl.h>
#include <gl\glu.h>

#define _USE_MATH_DEFINES
#include <math.h>

double BRLK(double angle, double TET, int Hrv, int N, double Hrel[], double dL);


//#define GRAF_B //для построения графиков для статьи


//////////////////////////////////////////////////////////////////////////

static	inline float sqr(float a) { return a * a; }

static	inline	float	CalcDist3D(float x1, float y1, float z1, float x2, float y2, float z2)
{
	return sqrt(sqr(x2-x1) + sqr(y2-y1) + sqr(z2-z1));
}
typedef struct _Tvector3 
{
	float x;
	float y;
	float z;
}Tvector3;

typedef struct _TColl
{
	Tvector3 coord;
	float Dist;
} TColl;

std::deque<TColl> the_queue;

XPLMProbeRef Probe;
XPLMProbeInfo_t * yprobestruct = new XPLMProbeInfo_t();

struct  
{
	float D;
	float angle;
}range_data[181];

//float angle_vis_ekr;
//float angle_beam_ekr;

float angle_vis_RLS;	//угол места
float angle_azimuth_RLS;//азимут

//////////////////////////////////////////////////////////////////////////


#include "typedefs.h"
#include "RMNK_str.h"

std::deque<float> the_queue_Y;

XPLMHotKeyID	gHotKeyF1 = NULL;
XPLMHotKeyID	gHotKeyF2 = NULL;
XPLMHotKeyID	gHotKeyF3 = NULL;
XPLMHotKeyID	gHotKeyF4 = NULL;
XPLMHotKeyID	gHotKeyF5 = NULL;
XPLMHotKeyID	gHotKeyF6 = NULL;
XPLMHotKeyID	gHotKeyF7 = NULL;
XPLMHotKeyID	gHotKeyF8 = NULL;
XPLMHotKeyID	gHotKeyF9 = NULL;
XPLMHotKeyID	gHotKeyF10 = NULL;
XPLMHotKeyID	gHotKeyF11 = NULL;
XPLMHotKeyID	gHotKeyF12 = NULL;

XPLMHotKeyID	gHotKeyUp = NULL;
XPLMHotKeyID	gHotKeyDown = NULL;
XPLMHotKeyID	gHotKey_Ctrl_F = NULL;
XPLMHotKeyID	gHotKey_Ctrl_D = NULL;

void	MyHotKeyCallbackF1(void *               inRefcon); 
void	MyHotKeyCallbackF2(void *               inRefcon); 
void	MyHotKeyCallbackF3(void *               inRefcon); 
void	MyHotKeyCallbackF4(void *               inRefcon); 
void	MyHotKeyCallbackF5(void *               inRefcon); 
void	MyHotKeyCallbackF6(void *               inRefcon); 
void	MyHotKeyCallbackF7(void *               inRefcon); 
void	MyHotKeyCallbackF8(void *               inRefcon); 
void	MyHotKeyCallbackF9(void *               inRefcon); 
void	MyHotKeyCallbackF10(void *               inRefcon); 
//void	MyHotKeyCallbackF11(void *               inRefcon); 
//void	MyHotKeyCallbackF12(void *               inRefcon); 

void	MyHotKeyCallbackUp(void *               inRefcon); 
void	MyHotKeyCallbackDown(void *               inRefcon); 

void	MyHotKeyCallback_Ctrl_F(void *               inRefcon); 
void	MyHotKeyCallback_Ctrl_D(void *               inRefcon); 

char F1_on = 1;
char F2_on = 0;
char F3_on = 0;
char F4_on = 0;
char F5_on = 1;
char F6_on = 0;
char F7_on = 0;
char F8_on = 0;
char F9_on = 0;
char F10_on = 1;
char F11_on = 0;
char F12_on = 0;


//font
GLvoid BuildFont(GLvoid);
GLvoid KillFont(GLvoid);
GLvoid glPrint(const char *fmt, ...);					// Custom GL "Print" Routine

void Paint_all();
void Paint_Hg();


HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

GLuint	base;				// Base Display List For The Font Set
GLfloat	cnt1;				// 1st Counter Used To Move Text & For Coloring
GLfloat	cnt2;				// 2nd Counter Used To Move Text & For Coloring

static XPLMDataRef	gControl_ail1_def = NULL;
static XPLMDataRef	gControl_ail2_def = NULL;
static XPLMDataRef	gControl_splr2_def = NULL;
static XPLMHotKeyID gExampleGaugeHotKey = NULL;

static XPLMDataRef gPlane_Vpr;
static XPLMDataRef gPlane_Hg_rv;
static XPLMDataRef gPlane_position_y_agl;
static XPLMDataRef gPlane_Hb;
static XPLMDataRef gPlane_local_vy;
static XPLMDataRef gPlane_dVy;
static XPLMDataRef gPlane_vh_ind;
static XPLMDataRef gPlane_vvi_fpm_pilot;

static XPLMDataRef gHWND;

static float Dir_kren;
static float Dir_tang;
static float Dir_rosh;

//static float Hg;
//static float Vy;
//static float Vpr;

static XPLMDataRef gPlaneX;
static XPLMDataRef gPlaneY;
static XPLMDataRef gPlaneZ;

XPLMDataRef gPlane_tang;
XPLMDataRef gPlane_kren;
XPLMDataRef gPlane_kurs;

XPLMDataRef gPlane_wx;
XPLMDataRef gPlane_wy;
XPLMDataRef gPlane_wz;

XPLMDataRef gPlane_nx;
XPLMDataRef gPlane_ny;
XPLMDataRef gPlane_nz;

XPLMDataRef gPlane_alfa;
XPLMDataRef gPlane_beta;

XPLMDataRef gPlane_time;
XPLMDataRef gPOINT_pitch_deg;

XPLMDataRef gOverride_control_surfaces;
XPLMDataRef gOverride_joystick_roll;
XPLMDataRef gOverride_joystick_pitch;
XPLMDataRef gOverride_joystick_heading	;
//XPLMDataRef gOverride_prop_pitch		;
XPLMDataRef gOverride_joystick		;

XPLMDataRef gControl_yoke_roll_ratio;
XPLMDataRef gControl_yoke_pitch_ratio;
XPLMDataRef gControl_yoke_heading_ratio;
XPLMDataRef gControl_joystick_axis_values;
XPLMDataRef gControl_joystick_axis_assignments;

XPLMDataRef gM_fuel_total;
XPLMDataRef gM_fuel1;
XPLMDataRef gM_fuel2;
XPLMDataRef gPOINT_pitch_deg_use;
XPLMDataRef gReplay_mode;

int AutopilotDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void * inRefcon);
int DirectorsDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void * inRefcon);
int DistanceDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void * inRefcon);
int DistanceDrawCallback2(XPLMDrawingPhase inPhase, int inIsBefore, void * inRefcon);

TRmnkVar RmnkVar;

FILE *file_data;

Tvect_sost vect_sost;
Tvect_upr vect_upr;
float MAX_Dist_RLS = 5000;


char from_matlab = 0;

PLUGIN_API int XPluginStart(
						char *		outName,
						char *		outSig,
						char *		outDesc)
{
	strcpy(outName, "Directors_and_Radar_From_Cha");
	strcpy(outSig, "xpsdk.experimental.directors");
	strcpy(outDesc, "A plug-in by Andrey Ch for displaying directors and rangefinder.");


	XPLMRegisterDrawCallback(
		AutopilotDrawCallback,
		xplm_Phase_Window,//xplm_Phase_FirstCockpit, /* Draw when sim is doing objects */
		0, /* After objects */
		NULL); /* No refcon needed */

		/// Register 
	XPLMRegisterDrawCallback(
					DirectorsDrawCallback,
					//xplm_Phase_Gauges,
					xplm_Phase_FirstCockpit,
					0,
					NULL);

	/* Next register teh drawing callback. We want to be drawn 
	* after X-Plane draws its 3-d objects. */
	XPLMRegisterDrawCallback(
		DistanceDrawCallback,
		xplm_Phase_Objects, /* Draw when sim is doing objects */
		0, /* After objects */
		NULL); /* No refcon needed */

	XPLMRegisterDrawCallback(
		DistanceDrawCallback2,
		xplm_Phase_Objects, /* Draw when sim is doing objects */
		0, /* After objects */
		NULL); /* No refcon needed */

	gControl_ail1_def = XPLMFindDataRef("sim/flightmodel/controls/ail1_def");
	gControl_ail2_def = XPLMFindDataRef("sim/flightmodel/controls/ail2_def");
	gControl_splr2_def = XPLMFindDataRef("sim/flightmodel/controls/splr2_def");

	gPlane_Vpr	= XPLMFindDataRef("sim/cockpit2/gauges/indicators/airspeed_kts_pilot");
	gPlane_Hg_rv	= XPLMFindDataRef("sim/cockpit2/gauges/indicators/radio_altimeter_height_ft_pilot");
	gPlane_position_y_agl	= XPLMFindDataRef("sim/flightmodel/position/y_agl");
	gPlane_Hb	= XPLMFindDataRef("sim/cockpit2/gauges/indicators/altitude_ft_pilot");
	
	//gPlane_Vy	= XPLMFindDataRef("sim/cockpit2/gauges/indicators/vvi_fpm_pilot");
	//gPlane_Vy	= XPLMFindDataRef("sim/flightmodel/position/vh_ind");
	gPlane_local_vy	= XPLMFindDataRef("sim/flightmodel/position/local_vy");
	gPlane_dVy	= XPLMFindDataRef("sim/flightmodel/position/local_ay");
	gPlane_vh_ind	= XPLMFindDataRef("sim/flightmodel/position/vh_ind");
	gPlane_vvi_fpm_pilot	= XPLMFindDataRef("sim/cockpit2/gauges/indicators/vvi_fpm_pilot");

	gHWND	= XPLMFindDataRef("sim/operation/windows/system_window");

	gPlaneX = XPLMFindDataRef("sim/flightmodel/position/local_x");
	gPlaneY = XPLMFindDataRef("sim/flightmodel/position/local_y");
	gPlaneZ = XPLMFindDataRef("sim/flightmodel/position/local_z");

	gPlane_tang = XPLMFindDataRef("sim/flightmodel/position/theta");
	gPlane_kren = XPLMFindDataRef("sim/flightmodel/position/phi");
	gPlane_kurs = XPLMFindDataRef("sim/flightmodel/position/psi");

	gPlane_wx = XPLMFindDataRef("sim/flightmodel/position/P");
	gPlane_wy = XPLMFindDataRef("sim/flightmodel/position/Q");	//faire attention sur la différence aux axes de coordonnées! 
	gPlane_wz = XPLMFindDataRef("sim/flightmodel/position/R");	//attention!

	gPlane_nx = XPLMFindDataRef("sim/flightmodel/forces/g_axil");
	gPlane_ny = XPLMFindDataRef("sim/flightmodel/forces/g_nrml");
	gPlane_nz = XPLMFindDataRef("sim/flightmodel/forces/g_side");	

	gPlane_alfa	= XPLMFindDataRef("sim/flightmodel/position/alpha");
	gPlane_beta	= XPLMFindDataRef("sim/flightmodel/position/beta");

	gPlane_time	= XPLMFindDataRef("sim/time/total_running_time_sec");

	gPOINT_pitch_deg	= XPLMFindDataRef("sim/flightmodel/engine/POINT_pitch_deg");


	gOverride_control_surfaces	= XPLMFindDataRef("sim/operation/override/override_control_surfaces");
	gOverride_joystick_roll		= XPLMFindDataRef("sim/operation/override/override_joystick_roll");
	gOverride_joystick_pitch	= XPLMFindDataRef("sim/operation/override/override_joystick_pitch");
	gOverride_joystick_heading	= XPLMFindDataRef("sim/operation/override/override_joystick_heading");
	//gOverride_prop_pitch		= XPLMFindDataRef("sim/operation/override/override_prop_pitch");
	gOverride_joystick		= XPLMFindDataRef("sim/operation/override/override_joystick");

	gControl_yoke_roll_ratio	= XPLMFindDataRef("sim/joystick/yoke_roll_ratio");
	gControl_yoke_pitch_ratio	= XPLMFindDataRef("sim/joystick/yoke_pitch_ratio");
	gControl_yoke_heading_ratio	= XPLMFindDataRef("sim/joystick/yoke_heading_ratio");

	gControl_joystick_axis_values= XPLMFindDataRef("sim/joystick/joystick_axis_values");
	gControl_joystick_axis_assignments= XPLMFindDataRef("sim/joystick/joystick_axis_assignments");
	//gControl_yoke_heading_ratio	= XPLMFindDataRef("sim/multiplayer/controls/yoke_pitch_ratio");
	//gControl_yoke_pitch_ratio	= XPLMFindDataRef("sim/joystick/yoke_pitch_ratio");

	gM_fuel_total	= XPLMFindDataRef("sim/flightmodel/weight/m_fuel_total");
	gM_fuel1	= XPLMFindDataRef("sim/flightmodel/weight/m_fuel1");
	gM_fuel2	= XPLMFindDataRef("sim/flightmodel/weight/m_fuel2");
	gPOINT_pitch_deg_use	= XPLMFindDataRef("sim/flightmodel/engine/POINT_pitch_deg_use");

	gReplay_mode	= XPLMFindDataRef("sim/operation/prefs/replay_mode");

	gHotKeyF1 = XPLMRegisterHotKey(XPLM_VK_F1, xplm_ShiftFlag | xplm_DownFlag, 
		"RLS",
		MyHotKeyCallbackF1,
		NULL);
	gHotKeyF2 = XPLMRegisterHotKey(XPLM_VK_F2, xplm_ShiftFlag | xplm_DownFlag, 
		"RV",
		MyHotKeyCallbackF2,
		NULL);
	gHotKeyF3 = XPLMRegisterHotKey(XPLM_VK_F3, xplm_ShiftFlag | xplm_DownFlag, 
		"RLS+RV",
		MyHotKeyCallbackF3,
		NULL);
	gHotKeyF4 = XPLMRegisterHotKey(XPLM_VK_F4, xplm_ShiftFlag | xplm_DownFlag, 
		"Control",
		MyHotKeyCallbackF4,
		NULL);
	gHotKeyF5 = XPLMRegisterHotKey(XPLM_VK_F5, xplm_ShiftFlag | xplm_DownFlag, 
		"Rangefinder",
		MyHotKeyCallbackF5,
		NULL);
	gHotKeyF6 = XPLMRegisterHotKey(XPLM_VK_F6, xplm_ShiftFlag | xplm_DownFlag, 
		"Show Beam",
		MyHotKeyCallbackF6,
		NULL);
	gHotKeyF7 = XPLMRegisterHotKey(XPLM_VK_F7, xplm_ShiftFlag | xplm_DownFlag, 
		"Show Trace",
		MyHotKeyCallbackF7,
		NULL);
	gHotKeyF8 = XPLMRegisterHotKey(XPLM_VK_F8, xplm_ShiftFlag | xplm_DownFlag, 
		"Show Beam TEST",
		MyHotKeyCallbackF8,
		NULL);
	gHotKeyF9 = XPLMRegisterHotKey(XPLM_VK_F9, xplm_ShiftFlag | xplm_DownFlag, 
		"Show DATA RMNK",
		MyHotKeyCallbackF9,
		NULL);
	gHotKeyF10 = XPLMRegisterHotKey(XPLM_VK_F10, xplm_ShiftFlag | xplm_DownFlag, 
		"Show DATA RMNK",
		MyHotKeyCallbackF10,
		NULL);
// 	gHotKeyF11 = XPLMRegisterHotKey(XPLM_VK_F11, xplm_ShiftFlag | xplm_DownFlag, 
// 		"Show DATA RMNK",
// 		MyHotKeyCallbackF11,
// 		NULL);
// 	gHotKeyF12 = XPLMRegisterHotKey(XPLM_VK_F12, xplm_ShiftFlag | xplm_DownFlag, 
// 		"Show DATA RMNK",
// 		MyHotKeyCallbackF12,
// 		NULL);

	gHotKeyUp = XPLMRegisterHotKey(XPLM_VK_UP, xplm_ControlFlag | xplm_DownFlag, 
		"Increase Hzad",
		MyHotKeyCallbackUp,
		NULL);

	gHotKeyDown = XPLMRegisterHotKey(XPLM_VK_DOWN, xplm_ControlFlag | xplm_DownFlag, 
		"Decrease Hzad",
		MyHotKeyCallbackDown,
		NULL);

	gHotKey_Ctrl_F = XPLMRegisterHotKey(XPLM_VK_F, xplm_ControlFlag | xplm_DownFlag, 
		"Refuel",
		MyHotKeyCallback_Ctrl_F,
		NULL);

	gHotKey_Ctrl_D = XPLMRegisterHotKey(XPLM_VK_D, xplm_ControlFlag | xplm_DownFlag, 
		"Change MaxDistRLS",
		MyHotKeyCallback_Ctrl_D,
		NULL);

	//Для шрифта
	int ihWnd	 = XPLMGetDatai(gHWND);
	ihWnd = ihWnd;
	hWnd = (HWND) ihWnd;
	hDC = GetDC(hWnd);
	BuildFont();										// Build The Font



	//

	/////////////выключение динамики X-Plane//////////////////////////////////
	//int i2[20];
	//long l;
	//l = XPLMGetDatavi(gOverRidePlanePosition,i2,0,20);
	//i2[0] = 1;
	//XPLMSetDatavi(gOverRidePlanePosition,i2,0,20);

	//XPLMSetDatai(gOverride_control_surfaces,1);
	//XPLMSetDatai(gOverride_joystick_roll,1);
	//XPLMSetDatai(gOverride_joystick_pitch,1);
	//XPLMSetDatai(gOverride_joystick_heading,1);


	//XPLMSetDatai(gOverride_joystick,0);
	//////////////////////////////////////////////////////////////////////////

	//file_data = fopen("data_cha.txt","wt");
	//шапка для файла данных
	//if (file_data)
	//{
	//	fprintf(file_data,"time\t");
	//	fprintf(file_data,"kren\t");
	//	fprintf(file_data,"tang\t");
	//	fprintf(file_data,"Vy\t");
	//	fprintf(file_data,"Hb\t");
	//	fprintf(file_data,"Hzad\t");
	//	fprintf(file_data,"osh\t");
	//	fprintf(file_data,"e\t");
	//	fprintf(file_data,"dVy\t");
	//	fprintf(file_data,"A[0]\t");
	//	fprintf(file_data,"A[2]\t");
	//	fprintf(file_data,"A[3]\t");
	//	fprintf(file_data,"A[5]\t");
	//	fprintf(file_data,"\n");
	//}

	//2
	//if (file_data)
	//{
	//	fprintf(file_data,"time\t");
	//	fprintf(file_data,"kren_zad\t");
	//	fprintf(file_data,"kren\t");		
	//	fprintf(file_data,"tang\t");		
	//	fprintf(file_data,"Vy\t");			
	//	fprintf(file_data,"Hb\t");			
	//	fprintf(file_data,"Hrel\t");		
	//	fprintf(file_data,"osh\t");			
	//	fprintf(file_data,"Vyzad\t");	
	//	fprintf(file_data,"Hg_rv\t");
	//	fprintf(file_data,"Hg\t");
	//	fprintf(file_data,"Hzad\t");
	//	fprintf(file_data,"Hg-Hzad\t");
	//	fprintf(file_data,"\n");
	//}

	//Для определителя высоты земной поверхности
	Probe = XPLMCreateProbe(xplm_ProbeY);
	yprobestruct->structSize = sizeof(XPLMProbeInfo_t);

	//Инициализация РМНК
	RmnkVar.P_0 = 1000;
	RmnkVar.n_p = 6;
	RMNK_sNU(RmnkVar);
	RmnkVar.n_p = 6;

	vect_sost.Hzad = 30;


	return 1;
}

PLUGIN_API void	XPluginStop(void)
{
	XPLMUnregisterHotKey(gHotKeyF1);
	XPLMUnregisterHotKey(gHotKeyF2);
	XPLMUnregisterHotKey(gHotKeyF3);
	XPLMUnregisterHotKey(gHotKeyF4);
	XPLMUnregisterHotKey(gHotKeyF5);
	XPLMUnregisterHotKey(gHotKeyF6);
	XPLMUnregisterHotKey(gHotKeyF7);
	XPLMUnregisterHotKey(gHotKeyF8);
	XPLMUnregisterHotKey(gHotKeyF9);
	XPLMUnregisterHotKey(gHotKeyF10);
	//XPLMUnregisterHotKey(gHotKeyF11);
	//XPLMUnregisterHotKey(gHotKeyF12);
	XPLMUnregisterDrawCallback(AutopilotDrawCallback,xplm_Phase_FirstCockpit, 0, NULL);
	XPLMUnregisterDrawCallback(DirectorsDrawCallback,xplm_Phase_FirstCockpit,0,NULL);
	XPLMUnregisterDrawCallback(DistanceDrawCallback,xplm_Phase_Objects, 0, NULL);
	XPLMUnregisterDrawCallback(DistanceDrawCallback2,xplm_Phase_Objects, 0, NULL);

	if (file_data!=NULL)
		fclose(file_data);
}

PLUGIN_API void XPluginDisable(void)
{
}

PLUGIN_API int XPluginEnable(void)
{
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(
					XPLMPluginID	inFromWho,
					int				inMessage,
					void *			inParam)
{
}

void Limit(float & f, float lim)
{
	if (f>lim) f = lim;
	if (f<-lim) f = -lim;
}

void Limit01(float & f)
{
	if (f>1.f) f = 1.f;
	if (f<0.f) f = 0.f;
}

void Limit11(float & f)
{
	if (f>1.f) f = 1.f;
	if (f<-1.f) f = -1.f;
}

//double UprRV(double dH, double dt)
//{
//	double T=0.85;
//	double k1 = -3.5;
//	double k2 = -2.5;
//	double k3 = -0.5;
//	double Vyzad_gran = 9.0;
//	Upr.dHf = Filtr1(dH, Upr.dHf, T, dt);
//	Upr.integraldHf += Upr.dHf*dt;
//	//Ограничение интеграла
//	if (Upr.integraldHf > 2.0) Upr.integraldHf = 2.0;
//	else if (Upr.integraldHf < -2.0) Upr.integraldHf = -2.0;
//	double dHdt = (dH - Upr.dHf)/T;
//	double Vyzad = k1*dHdt+k2*Upr.dHf+k3*Upr.integraldHf;
//	if (Vyzad < -Vyzad_gran) Vyzad = -Vyzad_gran;
//	if (Vyzad > Vyzad_gran) Vyzad = Vyzad_gran;
//	return Vyzad;
//}

char rejim_komplex = 1;//1 - RV, 2- RLS

void Control()
{
#ifdef GRAF_B

	//static char step = 0;
	static char flag_start = 0;
	static float start_time = 0;
	float t_begin = 25;
	float t_rec= 30;
	if (!flag_start && vect_sost.time>t_begin)
	{
		flag_start = 1;
		start_time = vect_sost.time;


		file_data = fopen("data_cha.txt","wt");
		//шапка для файла данных
		if (file_data)
		{
			fprintf(file_data,"time\t");
			fprintf(file_data,"kren_zad\t");
			fprintf(file_data,"kren\t");		
			fprintf(file_data,"tang\t");		
			fprintf(file_data,"Vy\t");			
			fprintf(file_data,"Hb\t");			
			fprintf(file_data,"Hrel\t");		
			fprintf(file_data,"osh\t");			
			fprintf(file_data,"Vyzad\t");	
			fprintf(file_data,"Hg_rv\t");
			fprintf(file_data,"Hg\t");
			fprintf(file_data,"Hzad\t");
			fprintf(file_data,"Hg-Hzad\t");
			fprintf(file_data,"\n");
		}

	}

	static char flag_stop = 0;			
	if (flag_start && !flag_stop && (vect_sost.time-start_time)>t_rec) 
	{
		flag_stop = 1;
		if (file_data!=NULL)
		{
			fclose(file_data);
			file_data=NULL;
		}
	}

#endif
	if (1)//крен
	{
	
		float kren_zad = 50*vect_sost.JOY_X;

#ifdef GRAF_B
		if (flag_start && !flag_stop)
		{
			//static char left_right = 0;
			static float t;
			t+=vect_sost.dt;

			kren_zad = 0;
			if (t>5)
				kren_zad = 30;
			if (t>15)
				kren_zad = -30;
			if (t>25)
				kren_zad = 0;
			
			
			kren_zad = 0;


			//if (t>10)
			//{
			//	t = 0;
			//	left_right=!left_right;
			//}
	
			//if (left_right)
			//	kren_zad = 30;	
			//else
			//	kren_zad = -30;	
	
			//апериодический фильтр kren_zad
			if (1)
			{
					float T = 0.5;//2.
				static float y2;
				float x = kren_zad;
				y2+= (1/T*(x-y2))*vect_sost.dt;
				kren_zad = y2;				
			}

			vect_sost.kren_zad = kren_zad;
		}
		else
			vect_sost.kren_zad = 0;
#endif




		float dx = 1.5*(vect_sost.kren_zad - vect_sost.kren) - 0.5*vect_sost.wx;

		vect_upr.DIR_X = dx;

		dx = dx/30.f;

		Limit(dx,1);
		XPLMSetDataf(gControl_yoke_roll_ratio,dx);	
	}

	if (1)//танг
	{

		float Vzad = 220;

		//switch (step)
		//{
		//case 1: Vzad = 220; break;
		//case 2: Vzad = 100; break;
		//case 3: Vzad = 180; break;
		//case 4: Vzad = 250; break;
		//case 5: Vzad = 0; break;
		//}

		//апериодический фильтр
		if (1)
		{
			float T = 5.;
			static float y2;
			float x = Vzad;
			y2+= (1/T*(x-y2))*vect_sost.dt;
			Vzad = y2;				
		}

		float tang_zad = 0.5*(vect_sost.Vpr-Vzad);
		Limit(tang_zad,15);

		tang_zad = 50*vect_sost.JOY_Z;

		float dz = 1.4*(tang_zad - vect_sost.tang) - 1.0*vect_sost.wz;// + 0.2*izod_wz;

		vect_upr.DIR_Z = dz;

		dz = dz/30.f;
		//Limit(dz,1);
		Limit(dz,1);
		XPLMSetDataf(gControl_yoke_pitch_ratio,dz);	
	}
	
	if (1)//рыск
	{

		//изодромный фильтр
		{
			float T = 0.5;
			static float y2;
			float x = vect_sost.wy;
			y2+= (1/T*(x-y2))*vect_sost.dt;
			float y = x- y2;
			vect_sost.wy = y;				
		}


		float dy = -0.5*vect_sost.beta + 0.3*vect_sost.wy;//0.1*vect_sost.nz//0.2*vect_sost.wy
		XPLMSetDataf(gControl_yoke_heading_ratio,dy);
		vect_upr.rv = dy;
	}

	//if (1)//высота
	//{
	//vect_sost.Hzad = 30;
	float Hzad_RLS = vect_sost.Hzad;//(vect_sost.JOY_H+1)*50;
	float Hzad_RV = vect_sost.Hzad;//(vect_sost.JOY_H+1)*50;
	float Htek = vect_sost.Hgf;

	//RMNK
	RmnkVar.b_z = 0.99;		
	RmnkVar.X[0] = vect_sost.Vy;
	RmnkVar.X[1] = vect_sost.Vpr/3.6;
	RmnkVar.X[2] = cos(vect_sost.tang*M_PI/180.f);
	RmnkVar.X[3] = sin(vect_sost.kren*M_PI/180.f);
	RmnkVar.X[4] = 1;
	RmnkVar.X[5] = vect_upr.osh;

	RmnkVar.D[0] = 0.1;
	RmnkVar.D[1] = 0.010;
	RmnkVar.D[2] = 0.5;
	RmnkVar.D[3] = 0.5;
	RmnkVar.D[4] = 0.1;
	RmnkVar.D[5] = 0.01;

	RmnkVar.Otklik = vect_sost.dVy;

	RMNK_s(RmnkVar);

	//static char rejim_komplex = 1;//1 - RV, 2- RLS


	//ПИД регулятор стаб Hрв
    if (1)
	{
		float raznH = Hzad_RV-Htek; 
		Limit(raznH,20); 

		static float y = 0;
		static float y1 = 0;
		float x = raznH;
		static float x1 = 0;
		static float x2 = 0;
		const float k1 = -3.5;
		const float k2 = -2.5;
		const float k3 = -0.5;
		const float T = 0.85;

		//Limit(x1,150);
		//Limit(x2,150);
		//Limit(y1,150);
		//if (fabs(raznH)>100)
		//{
		//	x1 = 0;
		//	x2 = 0;
		//	y1 = 0;
		//}
		if (rejim_komplex == 2)
		{
			x1 = 0;
			x2 = 0;
			y1 = 0;
			y = 0;
			x = 0;
		}

		float dt = vect_sost.dt;

		if (y>7.f || y<-7.f) 
			dt = 0;

		x1+=x*dt;
		x2+=x1*dt;
		y1+=y*dt;
		y = 1/T*(k1*x+k2*x1+k3*x2-y1);

		vect_upr.VyRV = -y*0.2;
	}

	//RLS
	if (F1_on)
	{
		vect_upr.Vymax = vect_upr.VyRLS;
	}

	//RV
	if (F2_on)
	{
		vect_upr.Vymax = vect_upr.VyRV;
	}

	//комплексирование
	if (F3_on)
	{
		//if (vect_upr.VyRLS > 4)
		//	vect_upr.Vymax = vect_upr.VyRLS;
		//else vect_upr.Vymax = vect_upr.VyRV;

		if (vect_upr.VyRLS > 4)
		{
			rejim_komplex = 2;
			vect_upr.Vymax = vect_upr.VyRLS;
		}
		else 
		{
			rejim_komplex = 1;
			vect_upr.Vymax = vect_upr.VyRV;
		}

		////апериодический фильтр
		//if (0)
		//{
		//	float T = 0.2;
		//	static float y2;
		//	float x = vect_upr.Vymax;
		//	y2+= (1/T*(x-y2))*vect_sost.dt;
		//	vect_upr.Vymax = y2;				
		//}
	}
	//else
	//	vect_upr.Vymax = vect_upr.VyRV;

	//vect_upr.Vymax = vect_upr.VyRLS;

	//ограничение заданной вертикальной скорости снижения
	float Vy_MAX_descent = -4.f;
	if (vect_upr.Vymax<Vy_MAX_descent)
		vect_upr.Vymax=Vy_MAX_descent;

	float dVy_ = -1*(vect_upr.Vymax-vect_sost.Vy);

	vect_upr.DIR_H = dVy_*10;

	static float int_dVy_;
	int_dVy_+=dVy_*vect_sost.dt;
	Limit(int_dVy_,20);

	
	float Xosh2 = dVy_+int_dVy_;
	float Xosh;
	Xosh = Xosh2;

	Limit(Xosh,25);

	float vscob = 
		+(-1-RmnkVar.A[0])*vect_sost.Vy
		-1*Xosh
		-RmnkVar.A[1]*vect_sost.Vpr/3.6
		-RmnkVar.A[2]*cos(vect_sost.tang*M_PI/180.f)
		-RmnkVar.A[3]*sin(vect_sost.kren*M_PI/180.f)
		-RmnkVar.A[4]
	;

	//защита
	if (RmnkVar.A[5]<0.005)	
		RmnkVar.A[5] = 0.005;

	vect_upr.osh = 6.7*1
		+1./RmnkVar.A[5]*(vscob);	

	if (vect_upr.osh>15) vect_upr.osh = 15;
	if (vect_upr.osh<-4) vect_upr.osh = -4;

	//апериодический фильтр
	{
		float T = 0.2;
		static float y2;
		float x = vect_upr.osh;
		y2+= (1/T*(x-y2))*vect_sost.dt;
		vect_upr.osh = y2;				
	}

	//vect_upr.osh = 10;

	//send control
	float fff[8];
	fff[0] = vect_upr.osh;//vect_upr.osh;//dH2;
	XPLMSetDatavf(gPOINT_pitch_deg, fff, 0, 1);

	//апериодический фильтр
	float e_him = 0;
	if (1)
	{
		float T = 0.1;
		static float y2;
		float x = RmnkVar.e_id;
		y2+= (1/T*(x-y2))*vect_sost.dt;
		e_him = y2;				
	}

	//вывод данных в файл для построения графиков
// 	if (0)//(file_data && flag_start && !flag_stop)
// 	{
// 		fprintf(file_data,"%f\t", vect_sost.time-start_time);
// 		fprintf(file_data,"%f\t", vect_sost.kren);
// 		fprintf(file_data,"%f\t", vect_sost.tang);
// 		fprintf(file_data,"%f\t", vect_sost.Vy);
// 		fprintf(file_data,"%f\t", vect_sost.Hb);
// 		fprintf(file_data,"%f\t", Hzad);
// 		fprintf(file_data,"%f\t", vect_upr.osh);
// 		fprintf(file_data,"%f\t", e_him/2.0*100);//max dVy = 2.0 //100%
// 		fprintf(file_data,"%f\t", vect_sost.dVy);
// 		fprintf(file_data,"%f\t", RmnkVar.A[0]);
// 		fprintf(file_data,"%f\t", RmnkVar.A[2]);
// 		fprintf(file_data,"%f\t", RmnkVar.A[3]);
// 		fprintf(file_data,"%f\t", RmnkVar.A[5]);
// 		fprintf(file_data,"\n");
// 	}

#ifdef GRAF_B

	//вывод данных в файл для построения графиков2
	if (file_data && flag_start && !flag_stop)
	{
		fprintf(file_data,"%f\t", vect_sost.time-start_time);
		fprintf(file_data,"%f\t", vect_sost.kren_zad);
		fprintf(file_data,"%f\t", vect_sost.kren);
		fprintf(file_data,"%f\t", vect_sost.tang);
		fprintf(file_data,"%f\t", vect_sost.Vy);
		fprintf(file_data,"%f\t", vect_sost.Hb);
		fprintf(file_data,"%f\t", vect_sost.Hb-vect_sost.Hgf);
		fprintf(file_data,"%f\t", vect_upr.osh);
		fprintf(file_data,"%f\t", vect_upr.VyRV);
		fprintf(file_data,"%f\t", vect_sost.Hg_rv);
		fprintf(file_data,"%f\t", vect_sost.Hgf);
		fprintf(file_data,"%f\t", vect_sost.Hzad);
		fprintf(file_data,"%f\t", vect_sost.Hgf-vect_sost.Hzad);
		fprintf(file_data,"\n");
	}
#endif
	//Вертикальная скорость
	if (1)
	{
		static char flag_up_down_beam = 0;//1-Up 0-Down
		const int N_DATA_RLS = 5000;
		static Tpoint p[N_DATA_RLS];//массив данных РЛС

		static float VymaxRLS = -10;

		float sector_obz = 15;	//сектор обзора +-15

		//1.	Получить из БРЛК массивы наклонных дальностей 
		static int c = 0;//количество точек 

		//static char i = 0;
		//i++;
		//if (i>sector_obz*2) 
		//{
		//	i = 0;
		//	memset(&p[c], 0, sizeof(Tpoint)*(100-c));
		//	c = 0;
		//	vect_upr.VyRLS = VymaxRLS;
		//	VymaxRLS = -10.f;
		//	flag_up_down_beam = !flag_up_down_beam;
		//}	

		static float temp_angle_vis;
		//if (temp_angle_vis>sector_obz)
		//{
		//	flag_up_down_beam = 0;
		//	memset(&p[c], 0, sizeof(Tpoint)*(N_DATA_RLS));
		//	c = 0;
		//	vect_upr.VyRLS = VymaxRLS;
		//	VymaxRLS = -10.f;
		//}
		//if (temp_angle_vis<-sector_obz)
		//{
		//	flag_up_down_beam = 1;
		//	memset(&p[c], 0, sizeof(Tpoint)*(N_DATA_RLS));
		//	c = 0;
		//	vect_upr.VyRLS = VymaxRLS;
		//	VymaxRLS = -10.f;
		//}


		if (flag_up_down_beam)
		{
			temp_angle_vis += vect_sost.dt*sector_obz*0.6666666666;//скорость вертикальной развертки 10 градусов/ сек, т.е. сканирование от -15 гр до +15 гр это 3 сек.
			if (temp_angle_vis>sector_obz)
			{
				flag_up_down_beam = !flag_up_down_beam;
				memset(p, 0, sizeof(p));
				c = 0;
				vect_upr.VyRLS = VymaxRLS;
				VymaxRLS = -10.f;
			}
		}
		else
		{
			temp_angle_vis -= vect_sost.dt*sector_obz*0.6666666666;
			if (temp_angle_vis<-sector_obz)
			{
				flag_up_down_beam = !flag_up_down_beam;
				memset(p, 0, sizeof(p));
				c = 0;
				vect_upr.VyRLS = VymaxRLS;
				VymaxRLS = -10.f;
			}
		}

		vect_upr.angle_vis = temp_angle_vis;

		//2.	Если некоторые Lk > Lмах, то удалить эти наклонные дальности 
		if (vect_sost.D_naklon2<MAX_Dist_RLS && c<N_DATA_RLS)
		{
			
			p[c].L = vect_sost.D_naklon2;
			p[c].epsil = -vect_upr.angle_vis;			

			//4.	По формулам (2) вычислить массивы ожидаемых времен полета ti 
			p[c].alfa = (-p[c].epsil+vect_sost.tang)*M_PI/180;
			p[c].t = p[c].L*cos(p[c].alfa)/(vect_sost.Vpr/3.6);

			
			p[c].Htr = vect_sost.Hg+ p[c].L*sin(p[c].alfa)+Hzad_RLS;

			float T = 1.f;//Т=1 сек – постоянная времени, характеризующая динамику ЛА;
			p[c].Vytr = (p[c].Htr - vect_sost.Hg - vect_sost.Vy*T)/(p[c].t - T);

			if (p[c].Vytr>VymaxRLS) 
				VymaxRLS = p[c].Vytr;

			c++;
		}

		//if (c == 0)//Если нет ничего в секторе обзора, то снижаемся
		//	;//vect_upr.VyRLS = -10;


		vect_upr.angle_azimuth = 0;

		//if (flag_up_down_beam)
		//	vect_upr.angle_vis = -(i-sector_obz);
		//else
		//	vect_upr.angle_vis = (i-sector_obz);

		//vect_upr.angle_vis = -10;

		if (vect_upr.VyRLS>10.f) 
			vect_upr.VyRLS = 10;
	}
	else
		vect_upr.VyRLS = 0;
}

void GetVectorSost()
{
	vect_sost.planeX = XPLMGetDataf(gPlaneX);
	vect_sost.planeY = XPLMGetDataf(gPlaneY);
	vect_sost.planeZ = XPLMGetDataf(gPlaneZ);

	vect_sost.wx = XPLMGetDataf(gPlane_wx);
	vect_sost.wy = -XPLMGetDataf(gPlane_wz); //faire attention sur la difference aux axes de coordonnees! 
	vect_sost.wz = XPLMGetDataf(gPlane_wy);

	vect_sost.nx = -XPLMGetDataf(gPlane_nx);
	vect_sost.ny = XPLMGetDataf(gPlane_ny);
	vect_sost.nz = XPLMGetDataf(gPlane_nz);

	vect_sost.alfa = XPLMGetDataf(gPlane_alfa);
	vect_sost.beta = XPLMGetDataf(gPlane_beta);

	vect_sost.tang = XPLMGetDataf(gPlane_tang);
	vect_sost.kren = XPLMGetDataf(gPlane_kren);
	vect_sost.kurs = XPLMGetDataf(gPlane_kurs);

	vect_sost.Hg_rv	 = XPLMGetDataf(gPlane_Hg_rv)*0.30479f;//футы в метры
	vect_sost.Hg	 = XPLMGetDataf(gPlane_position_y_agl);//метры

	//апериодический фильтр Hgf
	if (1)
	{
		float T = 0.1;
		static float y2;
		float x = vect_sost.Hg;
		y2+= (1/T*(x-y2))*vect_sost.dt;
		vect_sost.Hgf = y2;				
	}


	vect_sost.Hb	 = XPLMGetDataf(gPlane_Hb)*0.30479f;//футы в метры

	vect_sost.Hr = vect_sost.Hb - vect_sost.Hg;

	vect_sost.Vy	 = XPLMGetDataf(gPlane_local_vy);
	//vect_sost.Vy	 = XPLMGetDataf(gPlane_vh_ind);
	//vect_sost.Vy	 = XPLMGetDataf(gPlane_vvi_fpm_pilot);
	vect_sost.dVy	 = XPLMGetDataf(gPlane_dVy);
	vect_sost.Vpr	 = XPLMGetDataf(gPlane_Vpr)*1.852f;//узлы в км/ч

	vect_sost.time	 = XPLMGetDataf(gPlane_time);

	static float old_time = vect_sost.time;
	vect_sost.dt	= vect_sost.time-old_time;
	old_time = vect_sost.time;


	//vect_sost.JOY_X = XPLMGetDataf(gControl_yoke_roll_ratio);
	//vect_sost.JOY_Z = XPLMGetDataf(gControl_yoke_pitch_ratio);
	//vect_sost.JOY_Y = XPLMGetDataf(gControl_yoke_heading_ratio);
	//vect_sost.JOY_H = XPLMGetDataf(gPOINT_pitch_deg);

	float fff[100];
	XPLMGetDatavf(gControl_joystick_axis_values, fff, 0, 100);
	vect_sost.JOY_X = fff[0]*0.001;
	vect_sost.JOY_Z = fff[1]*0.001;
	//vect_sost.JOY_Y = fff[2]*0.001;
	vect_sost.JOY_H = fff[2]*0.001;
	//int iii[100];
	//XPLMGetDatavi(gControl_joystick_axis_assignments, iii, 0, 100);
	//iii[0];
}

void IniSituation( const char* name_file)
{
	static char flag_sit = 1;
	//const char* name_file = "Output\\situations\\10_500.sit";
	if (flag_sit)
	{
		flag_sit = 0;
		XPLMLoadDataFile(xplm_DataFile_Situation, name_file);
		//F2_on = 1;
	}
}

int	AutopilotDrawCallback(
	XPLMDrawingPhase     inPhase,
	int                  inIsBefore,
	void *               inRefcon)
{
	//GetVectorSost();
	//return 1;

	//Загрузка начального положения вертолета
	//IniSituation("Output\\situations\\9.sit");
	//IniSituation("Output\\situations\\2cha.sit");
	IniSituation("Output\\situations\\3cha_sea.sit");

	if (1)//F4_on)
	{
		//Получение вектора состояния
		GetVectorSost();

		int Replay_mode = XPLMGetDatai(gReplay_mode);//Ловим режим воспроизведения полета REPLAY
		if (!Replay_mode)		
			Control();	//Управление

		//Визуализация данных на экране
		if (F9_on)
			Paint_all();
		//else
			//Paint_Hg();

	}
	return 1;

}


void	MyHotKeyCallbackF1(void *               inRefcon)
{
	F1_on = 1;//!F1_on;
	F2_on = 0;//!F1_on;
	F3_on = 0;//!F1_on;
}

void	MyHotKeyCallbackF2(void *               inRefcon)
{
	F1_on = 0;
	F2_on = 1;
	F3_on = 0;
}

void	MyHotKeyCallbackF3(void *               inRefcon)
{
	F1_on = 0;
	F2_on = 0;
	F3_on = 1;
}

void	MyHotKeyCallbackF4(void *               inRefcon)
{
	F4_on = !F4_on;
}

void	MyHotKeyCallbackF5(void *               inRefcon)
{
	//F5_on = !F5_on;
}

void	MyHotKeyCallbackF6(void *               inRefcon)
{
	F6_on = !F6_on;
	F8_on = 0;
}
void	MyHotKeyCallbackF7(void *               inRefcon)
{
	F7_on = !F7_on;
	//kurs_beam-=10;
}
void	MyHotKeyCallbackF8(void *               inRefcon)
{
	F8_on = !F8_on;
	//kurs_beam+=10;
	F6_on = 0;
}

void	MyHotKeyCallbackF9(void *               inRefcon)
{
	F9_on = !F9_on;	
}
void	MyHotKeyCallbackF10(void *               inRefcon)
{
	F10_on = 1;	
	F11_on = 0;	
	F12_on = 0;	
}
void	MyHotKeyCallbackF11(void *               inRefcon)
{
	F10_on = 0;	
	F11_on = 1;	
	F12_on = 0;
}
void	MyHotKeyCallbackF12(void *               inRefcon)
{
	F10_on = 0;	
	F11_on = 0;	
	F12_on = 1;
}

void	MyHotKeyCallbackUp(void *               inRefcon)
{
	vect_sost.Hzad+=5;
}

void	MyHotKeyCallbackDown(void *               inRefcon)
{
	vect_sost.Hzad-=5;
	if (vect_sost.Hzad<10)
		vect_sost.Hzad=10;
}

void	MyHotKeyCallback_Ctrl_F(void *               inRefcon)
{
	//Refueling
	XPLMSetDataf(gM_fuel1,500);
	XPLMSetDataf(gM_fuel2,500);
}

void	MyHotKeyCallback_Ctrl_D(void *               inRefcon)
{
	//Changing Maximal Locator Range
	if (MAX_Dist_RLS > 2501)  
		MAX_Dist_RLS = 2500;
	else
		MAX_Dist_RLS = 5000;
}

GLvoid BuildFont(GLvoid)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(300);								// Storage For 96 Characters

	font = CreateFont(	24,							// Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		FW_NORMAL,						// Font Weight
		FALSE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
		L"Arial");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	bool bbb = wglUseFontBitmaps(hDC, 32, 300, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

GLvoid KillFont(GLvoid)									// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}


GLvoid glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}

void Graphic_y(float y)
{


	if (F7_on)
	{
		float length_graf = 500.f;//dots
		float Y100 = 100.f;//dots



		static float dttt;
		dttt+=vect_sost.dt;
		if (dttt>0.1)
		{
			dttt = dttt-0.1;
			the_queue_Y.push_front(y);
			if (the_queue_Y.size()>length_graf)
				the_queue_Y.pop_back();
		}



		//float mashY = Y100/5.;	// 100dots / Vy
		float mashY = Y100/0.1;	// 100dots / Vy

		glPushMatrix();
		int ourWidth, ourHight;
		XPLMGetScreenSize(&ourWidth, &ourHight);
		glTranslatef(ourWidth/2, ourHight/2, 0); //центрируем	


		//central line
		glColor3f(1,0.5,0.5); 
		glLineWidth(1.f);
		glBegin(GL_LINES);
		glVertex3f(0,0, 0);
		glVertex3f(length_graf,0, 0);
		glVertex3f(0,Y100, 0);
		glVertex3f(length_graf,Y100, 0);
		glVertex3f(0,-Y100, 0);
		glVertex3f(length_graf,-Y100, 0);
		for (int i = 0; i<6; i++)
		{
			glVertex3f(i*Y100,0, 0);
			glVertex3f(i*Y100,5, 0);
		}
		glEnd();


		//graphic
		glColor3f(1,0.8,0.8);  
		glPointSize(1.0f);
		glBegin(GL_POINTS);	 	
		for (int i = 1; i<the_queue_Y.size(); i++)
		{
			glVertex3f(i*1, the_queue_Y[i]*mashY, 0);
		}
		glEnd();
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1);
	glPopMatrix();
}


void Paint_all()
{

	//Show data on screen
	XPLMSetGraphicsState(0/*Fog*/, 1/*TexUnits*/, 0/*Lighting*/, 0/*AlphaTesting*/, 1/*AlphaBlending*/, 0/*DepthTesting*/, 0/*DepthWriting*/);
	glBlendFunc(GL_ONE, GL_ONE);
	//непонятная команда, влияющая на прозрачность директоров
	XPLMBindTexture2d(0, 0);


	glPushMatrix();
	{
		int ourWidth, ourHight;
		XPLMGetScreenSize(&ourWidth, &ourHight);
		glTranslatef(ourWidth/2, ourHight/2, 0); //центрируем	



		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor3d(1,1,0);     // желтый цвет

		float X_str = 310;
		float H_str = 350;
		float d_Hstr = 40;
		char  i = 0;
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("wx = %2.1f", vect_sost.wx);
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("wy = %2.1f", vect_sost.wy);
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("wz = %-2.1f", vect_sost.wz);
		i++;
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("nx = %2.1f ", vect_sost.nx);
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("ny = %2.1f ", vect_sost.ny);
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("nz = %2.1f ", vect_sost.nz);
		i++;
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("alfa = %2.1f ", vect_sost.alfa);
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("beta = %2.1f ", vect_sost.beta);
		i++;
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("tang = %2.1f ", vect_sost.tang);
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("kren = %2.1f ", vect_sost.kren);
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("kurs = %3.0f ", vect_sost.kurs);
		i++;			
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("time = %2.1f ", vect_sost.time);
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("dt = %1.4f ", vect_sost.dt);

		X_str = 10;
		H_str = 350;
		i = 0;
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("Hg = %4.0f ", vect_sost.Hg);
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("Hb = %4.0f ", vect_sost.Hb);
		i++;
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("Vy = %2.3f", vect_sost.Vy);
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("dVy = %2.4f", vect_sost.dVy);
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("Vpr = %2.1f", vect_sost.Vpr);
		i++;
		//glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("osh = %2.1f", vect_upr.osh);
		//glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("rv?? = %2.1f", vect_upr.rv);

		//glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("fff1 = %2.1f", fff[0]);
		//glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("fff2 = %2.1f", fff[1]);
		i++;
		//glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("intH = %3.0f", intH);


		X_str = -500;
		H_str = 0;
		float dy_str = 80;
		i = 0;
		//glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("%03.1f|%03.1f|%03.1f|%03.1f|%03.1f|%03.1f", RmnkVar.P[0][0], RmnkVar.P[0][1], RmnkVar.P[0][2], RmnkVar.P[0][3], RmnkVar.P[0][4], RmnkVar.P[0][5]);
		glRasterPos2f(X_str+0*dy_str, H_str+d_Hstr);	glPrint("P");
		glRasterPos2f(X_str+6.5*dy_str, H_str+d_Hstr);	glPrint("X");
		glRasterPos2f(X_str+8*dy_str, H_str+d_Hstr);	glPrint("A");
		glRasterPos2f(X_str+0*dy_str, H_str+d_Hstr+100);	glPrint("e = %2.5f", RmnkVar.e_id);

		
		for (int l = 0; l<6;l++)
		{
			for (int k = 0; k<6; k++)
			{
				glRasterPos2f(X_str+k*dy_str, H_str-d_Hstr*l);	glPrint("%3.1f", RmnkVar.P[l][k]);
			}
			glRasterPos2f(X_str+6.5*dy_str, H_str-d_Hstr*l);	glPrint("%3.4f", RmnkVar.X[l]);
			glRasterPos2f(X_str+8*dy_str, H_str-d_Hstr*l);	glPrint("%3.4f", RmnkVar.A[l]);			
		}
		//-d_Hstr*i++;




		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth(1);
	}
	glPopMatrix();

	Graphic_y(RmnkVar.e_id);

	glFlush();
}


void Paint_Hg()
{

	//Show data on screen
	XPLMSetGraphicsState(0/*Fog*/, 1/*TexUnits*/, 0/*Lighting*/, 0/*AlphaTesting*/, 1/*AlphaBlending*/, 0/*DepthTesting*/, 0/*DepthWriting*/);
	glBlendFunc(GL_ONE, GL_ONE);
	//непонятная команда, влияющая на прозрачность директоров
	XPLMBindTexture2d(0, 0);


	glPushMatrix();
	{
		int ourWidth, ourHight;
		XPLMGetScreenSize(&ourWidth, &ourHight);
		glTranslatef(ourWidth/2, ourHight/2, 0); //центрируем	



		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor3d(1,1,0);     // желтый цвет

		float X_str = 310;
		float H_str = 350;
		float d_Hstr = 40;
		char  i = 0;

		X_str = 10;
		H_str = 350;
		i = 0;
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("Hg = %4.0f ", vect_sost.Hg);
		glRasterPos2f(X_str, H_str-d_Hstr*i++);	glPrint("Hb = %4.0f ", vect_sost.Hb);


		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth(1);
	}
	glPopMatrix();

	//Graphic_y(RmnkVar.e_id);

	glFlush();
}


/*
 * DirectorsDrawCallback
 *
 * This will draw our directors.
 *
 */
int	DirectorsDrawCallback(
                                   XPLMDrawingPhase     inPhase,
                                   int                  inIsBefore,
                                   void *               inRefcon)
{
	if (1)//F1_on)
	{		
		float	Mfuel = XPLMGetDataf(gM_fuel_total);
		float	POINT_pitch_deg_use[8];
		XPLMGetDatavf(gPOINT_pitch_deg_use, POINT_pitch_deg_use, 0, 8);

		if (from_matlab)
		{
			float FloatVals[56];
			XPLMGetDatavf(gControl_splr2_def, FloatVals, 0, 56);	
	
			Dir_kren = FloatVals[8]*100;
			Dir_tang = FloatVals[9]*100;
			Dir_rosh = FloatVals[10]*100;
	
			angle_vis_RLS		= FloatVals[11];
			angle_azimuth_RLS	= FloatVals[12];
		}
		else
		{
			Dir_kren = vect_upr.DIR_X;
			Dir_tang = vect_upr.DIR_Z;
			Dir_rosh = vect_upr.DIR_H;
			angle_vis_RLS = vect_upr.angle_vis;
			angle_azimuth_RLS = vect_upr.angle_azimuth;
		}
	
	
		int DirKrenLength = 100;
		//int DirKrenPosition = 0;
		int DirTangLength = 100;
		//int DirTangPosition = 0;
	
		int ourWidth, ourHight;
		XPLMGetScreenSize(&ourWidth, &ourHight);
	
		// Turn on Alpha Blending and turn off Depth Testing
		XPLMSetGraphicsState(0/*Fog*/, 1/*TexUnits*/, 0/*Lighting*/, 0/*AlphaTesting*/, 1/*AlphaBlending*/, 0/*DepthTesting*/, 0/*DepthWriting*/);
		glBlendFunc(GL_ONE, GL_ONE);
	
		//непонятная команда, влияющая на прозрачность директоров
		XPLMBindTexture2d(0, 0);
	
		glPushMatrix();
			glTranslatef(ourWidth/2, ourHight/2, 0); //центрируем				
			glLineWidth(3);       // ширину линии 
			// устанавливаем 1
			glBegin(GL_LINES);
				glColor3d(1,1,0);     // желтый цвет
				glVertex3d(Dir_kren,-DirKrenLength/2, 0); // первая линия (крен)
				glVertex3d(Dir_kren,+DirKrenLength/2, 0); 
				glColor3d(1,1,0);     // желтый цвет
				glVertex3d(-DirTangLength/2,Dir_tang,0); // вторая линия (тангаж)
				glVertex3d(+DirTangLength/2,Dir_tang,0); 				
			glEnd();
	
			//точки
			glBegin(GL_POINTS);
				glVertex3f(0,-DirKrenLength	*0.33, 0);
				glVertex3f(0,-DirKrenLength	*0.66, 0);
				glVertex3f(0,DirKrenLength	*0.33, 0); 
				glVertex3f(0,DirKrenLength	*0.66, 0); 
	
				glVertex3f(-DirTangLength	*0.33,0, 0);
				glVertex3f(-DirTangLength	*0.66,0, 0);
				glVertex3f(DirTangLength	*0.33, 0, 0);
				glVertex3f(DirTangLength	*0.66, 0, 0);
			glEnd();
	
			{//кольцо
	
				float radius = 10.f;
				float position = -120.f;
						
				glBegin(GL_LINES);
					glVertex3f(-radius + position, 0, -1); //централная линия директора ОШ
					glVertex3f(+radius + position, 0, -1);					
				glEnd();
	
				glBegin( GL_LINE_LOOP );
				for( int i = 0; i <= 16; i++ ) 
				{
					float a = (float)i / 16.0f * 3.1415f * 2.0f;
					glVertex2f(cos( a ) * radius + position, sin( a ) * radius + Dir_rosh);
				}
				glEnd();
			}
	
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
			// Position The Text On The Screen
			glRasterPos2f(-230, 100);
			glPrint("Vpr = %3.0f km/h", vect_sost.Vpr);
			glRasterPos2f(170, 0);
			glPrint("Vy = %2.2f m/s", vect_sost.Vy);
			glRasterPos2f(170, 30);
			glPrint("Hb = %2.0f m", vect_sost.Hb);

#ifdef GRAF_B
			if (file_data!=NULL)
			{
				glColor3d(1,0.0,0);     // оранжевый цвет
				glRasterPos2f(170, 60);
				glPrint("rec");
				glColor3d(1,1,0);     // желтый цвет
			}
#endif

			if (!from_matlab)
			{
				glColor3d(1,0.8,0);     // оранжевый цвет
				glRasterPos2f(170, 130);
				glPrint("Hzad = %3.0f m", vect_sost.Hzad);
			}

			glColor3d(1,1,0);     // желтый цвет
			glRasterPos2f(170, 100);
			glPrint("Hrv = %3.0f m", vect_sost.Hg);

			glRasterPos2f(170, -100);
			glPrint("Osh = %2.1f\260", POINT_pitch_deg_use[0]);//vect_upr.osh);
			glRasterPos2f(170, -130);
			glPrint("Rv = %2.1f\260", POINT_pitch_deg_use[1]);//vect_upr.osh);

			glRasterPos2f(370, -70);
			glPrint("kren = %2.1f\260", vect_sost.kren);// [\260 - символ градуса]

			glRasterPos2f(370, -100);
			glPrint("tang = %2.1f\260", vect_sost.tang);// [\260 - символ градуса]

			if (Mfuel<100.f)
			{
				glColor3d(1,0,0);     // красный цвет
				XPLMSetDataf(gM_fuel1,500);
				XPLMSetDataf(gM_fuel2,500);
			}
			else
				glColor3d(1,1,0);     // желтый цвет
			glRasterPos2f(370, -160);
			glPrint("Fuel = %3.0f kg", Mfuel);

			glColor3d(1,1,0);     // желтый цвет

			if (0)
			{
				glRasterPos2f(170, 300);
				glPrint("Vymax = %2.1f m/s", vect_upr.Vymax);
				glRasterPos2f(170, 320);
				glPrint("VyRLS = %2.1f m/s", vect_upr.VyRLS);
				glRasterPos2f(170, 340);
				glPrint("VyRV = %2.1f m/s", vect_upr.VyRV);
			}
			else
			{
				glRasterPos2f(0, 300);
				glPrint("Mode:");
				if (from_matlab)
				{
					glRasterPos2f(70, 300);
					glPrint("MATLAB");
				} 
				else if (F1_on)
				{
					glRasterPos2f(70, 300);
					if (MAX_Dist_RLS>2501)
						glPrint("RLS 5");
					else
						glPrint("RLS 2.5");
				}
				else if (F2_on)
				{
					glRasterPos2f(70, 300);
					glPrint("RV");
				}
				else if (F3_on)
				{
					glRasterPos2f(70, 300);
					if (rejim_komplex==1)//RV
						glPrint("RV!+RLS");
					if (rejim_komplex==2)//RLS
						glPrint("RV+RLS!");

					if (MAX_Dist_RLS>2501)
						glPrint(" 5");
					else
						glPrint(" 2.5");

					rejim_komplex;
				}
			}
			
			if (F6_on)
			{
				glRasterPos2f(300, 200);
				glPrint("D_naklon = %5.0f", vect_sost.D_naklon2);
				//glRasterPos2f(300, 170);
				//glPrint("D_naklon2 = %5.0f", vect_sost.D_naklon2);
			}

			if (F6_on)
			{
				glRasterPos2f(300, 300);
				glPrint("angle_vis = %3.1f", angle_vis_RLS);	
				glRasterPos2f(300, 250);
				glPrint("azimuth = %3.1f", angle_azimuth_RLS);
			}
	
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glLineWidth(1);
		glPopMatrix();
	
		glFlush();
	}
	return 1;
}

void rawSphere( float X, float Y, float Z )
{
	glPushMatrix();
	glTranslatef (X,  Y,  Z);
	glColor3f( 1, 1, 0);
	GLUquadricObj *quadric;
	quadric = gluNewQuadric();
	//gluQuadricDrawStyle(quadric, GLU_SMOOTH );
	gluSphere( quadric , 1. , 8 , 8 );
	gluDeleteQuadric(quadric); 
	glPopMatrix();
}

//Функция для определения дальности и отрисовки измерителя
int DistanceDrawCallback(
XPLMDrawingPhase inPhase, 
int inIsBefore, 
void * inRefcon)
{	
	return 1;

	float FloatVals[56];
	XPLMGetDatavf(gControl_splr2_def, FloatVals, 0, 56);

	if (!from_matlab && FloatVals[13]>0) //получение признака работы с матлабом
		from_matlab = 1;

	//учет угла крена
	float sin_kr = sin(vect_sost.kren*M_PI/180.f);
	float cos_kr = cos(vect_sost.kren*M_PI/180.f);

	if (!from_matlab)//управление лучем внутреннее
	{
		FloatVals[11] = vect_upr.angle_vis;
		FloatVals[12] = vect_upr.angle_azimuth;
	}

	FloatVals[11] = -FloatVals[11]; //коррекция по просьбе ВБ

	angle_vis_RLS		=  FloatVals[11]*cos_kr+FloatVals[12]*sin_kr;
	angle_azimuth_RLS	=  -FloatVals[11]*sin_kr+FloatVals[12]*cos_kr;

	/* Reset the graphics state. This turns off fog, texturing, lighting,
	* alpha blending or testing and depth reading and writing, which
	* guarantees that our axes will be seen no matter what. */
	XPLMSetGraphicsState(0, 0, 1, 1, 1, 0, 0);

	if (F5_on)
	{
		//максимальная дальность измерителя
		float Distance_to_ground = MAX_Dist_RLS;
		//угол места
		float fi_mesta = (angle_vis_RLS-vect_sost.tang)*M_PI/180.f;;

		//scaner
		const int DOTS = 10000;
		float shag = MAX_Dist_RLS/DOTS;//100;//meter

		float D_beam = 0;

		float x_beam = 0;
		float y_beam = 0;
		float z_beam = 0;

		Tvector3 koord_beam[DOTS];
		Tvector3 koord_ground[DOTS];
		Tvector3 dkoord_beam;
		Tvector3 dkoord_ground;
		Tvector3 ddd;
		Tvector3 our_point;

		const int MAX_COLLECTOR = 100;//2400;


		//TEST mistake	
		//Tvector3 test_our_point;

		int i_100 = 0;

		for (int i = 0; i<DOTS; i++)
		{
			float D_xz = D_beam*cos(fi_mesta);
			y_beam = -D_beam*sin(fi_mesta);
			D_beam+=shag;

			float kurs_rad = (vect_sost.kurs+angle_azimuth_RLS)*M_PI/180.f;
			x_beam = sin(kurs_rad)*D_xz;
			z_beam = -cos(kurs_rad)*D_xz;

			koord_beam[i].x = vect_sost.planeX + x_beam;
			koord_beam[i].y = vect_sost.planeY + y_beam;
			koord_beam[i].z = vect_sost.planeZ + z_beam;

			XPLMProbeTerrainXYZ(Probe,vect_sost.planeX+x_beam, 0 , vect_sost.planeZ+z_beam, yprobestruct);

			koord_ground[i].x = yprobestruct->locationX;
			koord_ground[i].y = yprobestruct->locationY;
			koord_ground[i].z = yprobestruct->locationZ;


			if (vect_sost.planeY + y_beam - koord_ground[i].y<0 && i>0)
			{
				dkoord_beam.x = koord_beam[i].x - koord_beam[i-1].x;
				dkoord_beam.y = koord_beam[i].y - koord_beam[i-1].y;
				dkoord_beam.z = koord_beam[i].z - koord_beam[i-1].z;				

				float k = 0;
				float d1 = koord_beam[i-1].y - koord_ground[i-1].y;
				float d2 = -koord_beam[i].y + koord_ground[i].y;
				if (d2>0)
				{
					k = d1/(d2+d1);
					ddd.x = dkoord_beam.x*k;
					ddd.y = dkoord_beam.y*k;
					ddd.z = dkoord_beam.z*k;

					our_point.x = koord_beam[i-1].x + ddd.x;
					our_point.y = koord_beam[i-1].y + ddd.y;
					our_point.z = koord_beam[i-1].z + ddd.z;
					
					//TEST mistake						
					//XPLMProbeTerrainXYZ(Probe,our_point.x, 0 , our_point.z, yprobestruct);
					//test_our_point.x = yprobestruct->locationX;
					//test_our_point.y = yprobestruct->locationY;
					//test_our_point.z = yprobestruct->locationZ;

					Distance_to_ground = CalcDist3D(vect_sost.planeX, vect_sost.planeY, vect_sost.planeZ, our_point.x, our_point.y, our_point.z);

					if (F7_on)
					{
						TColl Coll;
						Coll.coord = our_point;
						Coll.Dist = Distance_to_ground;
						the_queue.push_front(Coll);
						if (the_queue.size()>MAX_COLLECTOR)
							the_queue.pop_back();
					}
				}					
				break;
			}
			else
				i_100++;
		}


		if (F8_on)
		{
			glBegin(GL_LINES);	 	
			for (int i = 0; i<i_100; i++)
			{
				glColor3f(1.0, 0.5, 1.0);
				glVertex3f(vect_sost.planeX, vect_sost.planeY, vect_sost.planeZ );
				glVertex3f(koord_beam[i].x, koord_beam[i].y, koord_beam[i].z);
				glColor3f(1.0, 1.0, 0.0);
				glVertex3f(koord_beam[i].x, koord_beam[i].y, koord_beam[i].z);
				glVertex3f(koord_ground[i].x, koord_ground[i].y, koord_ground[i].z);
			}

			if (i_100>0 && i_100<DOTS)
			{
				glColor3f(0.0, 1.0, 0.0);
				glVertex3f(koord_beam[i_100-1].x, koord_beam[i_100-1].y, koord_beam[i_100-1].z);
				glVertex3f(our_point.x, our_point.y, our_point.z);

				//TEST mistake
				//glColor3f(1.0, 0.0, 0.0);
				//glVertex3f(our_point.x, our_point.y, our_point.z);
				//glVertex3f(test_our_point.x, test_our_point.y, test_our_point.z);
			}
			glEnd();
		}
		if (F6_on)
		{
			if (i_100>0 && i_100<DOTS)
			{
				glBegin(GL_LINES);	 	
				glColor3f(1.0, 0.5, 1.0);
				glVertex3f(vect_sost.planeX, vect_sost.planeY, vect_sost.planeZ );
				glVertex3f(our_point.x, our_point.y, our_point.z);
				glEnd();
			}

			else
			{
				glBegin(GL_LINES);	 	
				glColor3f(1.0, 0.8, 1.0);
				glVertex3f(vect_sost.planeX, vect_sost.planeY, vect_sost.planeZ );
				glVertex3f(koord_beam[i_100-1].x, koord_beam[i_100-1].y, koord_beam[i_100-1].z);
				glEnd();
			}
		}

		//Отправка данных
		//float FloatVals[56];
		//memset(FloatVals,0,sizeof(FloatVals));
		//FloatVals[8] = Distance_to_ground;
		//XPLMSetDatavf(gControl_ail2_def, FloatVals, 0, 56);

		//отрисовка следа
		if (F6_on && F7_on)
		{
			glBegin(GL_LINES);	 	
			for (int i = 1; i<the_queue.size(); i++)
			{
				float k_color1 = the_queue[i].Dist/1000.f;
				if (k_color1>1.f) k_color1 = 1.f;
				if (k_color1<0.f) k_color1 = 0.f;

				float k_color2 = 1-(the_queue[i].Dist-1000)/1000.f;
				if (k_color2>1.f) k_color2 = 1.f;
				if (k_color2<0.f) k_color2 = 0.f;

				glColor3f(k_color2, k_color1, 0.0);
				glVertex3f(the_queue[i-1].coord.x, the_queue[i-1].coord.y, the_queue[i-1].coord.z );
				glVertex3f(the_queue[i].coord.x, the_queue[i].coord.y, the_queue[i].coord.z);
			}
			glEnd();
		}

		vect_sost.D_naklon = Distance_to_ground;
	}
	return 1;
} 


//Функция для определения дальности и отрисовки измерителя
int DistanceDrawCallback2(
XPLMDrawingPhase inPhase, 
int inIsBefore, 
void * inRefcon)
{	
	//return 1;

	float FloatVals[56];
	XPLMGetDatavf(gControl_splr2_def, FloatVals, 0, 56);

	if (!from_matlab && FloatVals[13]>0) //получение признака работы с матлабом
		from_matlab = 1;


	//TEST_CHA
	//FloatVals[11] = vect_upr.angle_vis  =-15;

	//учет угла крена
	float sin_kr = sin(vect_sost.kren*M_PI/180.f);
	float cos_kr = cos(vect_sost.kren*M_PI/180.f);

	if (!from_matlab)//управление лучем внутреннее
	{
		FloatVals[11] = vect_upr.angle_vis;
		FloatVals[12] = vect_upr.angle_azimuth;
	}

	FloatVals[11] = -FloatVals[11]; //коррекция по просьбе ВБ

	angle_vis_RLS		=  FloatVals[11]*cos_kr+FloatVals[12]*sin_kr;
	angle_azimuth_RLS	=  -FloatVals[11]*sin_kr+FloatVals[12]*cos_kr;

	/* Reset the graphics state. This turns off fog, texturing, lighting,
	* alpha blending or testing and depth reading and writing, which
	* guarantees that our axes will be seen no matter what. */
	XPLMSetGraphicsState(0, 0, 1, 1, 1, 0, 0);

	//Новый имитатор РЛС
	if (F5_on)
	{

		double eps, teps,td,de,H,L,td1,td2,de1;
		const float Lmax = 100000;

		//новый алгоритм
		const int N_dots = 5000;	//количество точек рельефа
		Tvector3 Rel[N_dots];		//координаты точек рельефа
		double Hrel[N_dots];		//координаты точек рельефа
		//memset(Rel,0,sizeof(Rel));

		//double Tg_e[N_dots];		//тангенсы
		float dL = MAX_Dist_RLS/N_dots;	//шаг (расстояние между соседними точками)

		//int i2 = 0;

		//const double maxdbl=1e15;
		//td=-maxdbl;

		float kurs_rad = (vect_sost.kurs)*M_PI/180.f;
		float SN_kurs_rad = sin(kurs_rad);
		float CS_kurs_rad = cos(kurs_rad);

		for (int i2 = 0; i2<N_dots; i2++)
		{
			//формирование координат x и z рельефа (в которых будем искать высоту рельефа)
			//float kurs_rad = (vect_sost.kurs+angle_azimuth_RLS)*M_PI/180.f;
			Rel[i2].x = vect_sost.planeX+SN_kurs_rad*dL*i2;
			Rel[i2].z = vect_sost.planeZ-CS_kurs_rad*dL*i2;
			//получение высоты рельефа
			XPLMProbeTerrainXYZ(Probe,Rel[i2].x, 0 , Rel[i2].z, yprobestruct);
			Rel[i2].y = yprobestruct->locationY;
			Hrel[i2] = Rel[i2].y;





			//отображение точек вертикальными белыми линиями
// 			if (F6_on)
// 			{
// 				//rawSphere(Rel[i2].x, Rel[i2].y, Rel[i2].z);
// 				glBegin(GL_LINES);	 	
// 				glColor3f(1.0, 1.0, 1.0);
// 				glVertex3f(Rel[i2].x, Rel[i2].y, Rel[i2].z);
// 				glVertex3f(Rel[i2].x, Rel[i2].y+10, Rel[i2].z);
// 				glEnd();
// 			}
			//Tg_e[0]=-maxdbl;
			//if (i2==0)
			//	continue;

			//Tg_e[i2]=(Rel[i2].y-(Rel[0].y+vect_sost.Hg))/double(dL*i2); //tangensi uglov vozvyshenia
			//if(Tg_e[i2]<=td)
			//	Tg_e[i2] = td;     //bylo -maxdbl
			//else 
			//	td=Tg_e[i2];
		}

		double dist2 = BRLK(-angle_vis_RLS,vect_sost.tang,vect_sost.Hg_rv,N_dots,Hrel,dL);


		//double angle, da=40.1; //grad
		//bool OK=false; //  nashli ugol
		//int ti;
		//static float fff;
		//static char flag_ = 1;
		//fff+=0.01*flag_;
		//if (fff>30)
		//	flag_=-1;
		//if (fff<-30)
		//	flag_=+1;


		//eps=(-angle_vis_RLS+vect_sost.tang)*M_PI/180.f;//(angle_vis_RLS-vect_sost.tang)*M_PI/180.f;//-10*M_PI/180.f;//(TET+angle)/57.3; //rad
		
		//TEST_CHA
		//angle_vis_RLS = -15;
		//eps=(-angle_vis_RLS+vect_sost.tang*0)*M_PI/180.f;//(angle_vis_RLS-vect_sost.tang)*M_PI/180.f;//-10*M_PI/180.f;//(TET+angle)/57.3; //rad

		//отрисовка заданного луча
		//if (F6_on)
		//{
		//	Tvector3 line_beam;
		//	//формирование координат x и z рельефа (в которых будем искать высоту рельефа)
		//	float kurs_rad = (vect_sost.kurs+angle_azimuth_RLS)*M_PI/180.f;
		//	line_beam.x = vect_sost.planeX+sin(kurs_rad)*MAX_Dist_RLS;
		//	line_beam.z = vect_sost.planeZ-cos(kurs_rad)*MAX_Dist_RLS;
		//	line_beam.y = vect_sost.planeY+tan(eps)*MAX_Dist_RLS;
		// 
		//	glBegin(GL_LINES);	 	
		//	glColor3f(0.0, 1.0, 0.0);
		//	glVertex3f(vect_sost.planeX+1, vect_sost.planeY, vect_sost.planeZ+1 );
		//	glVertex3f(line_beam.x, line_beam.y, line_beam.z);
		//	glEnd();
		//}

		//teps=tan(eps);
		//// if(teps> Tg_e[N_dots-1]+da/57.3)continue;  //angle  vyshe rel'efa
		//de=maxdbl; // minimalnaya nev'iazka Tg
		//OK=false;
		//// /*
		//{ //polovin. delenie
		//	int a=1,b=N_dots-1,ab;
		//	while(b-a>1)
		//	{
		//		ab=(a+b)/2;  //seredina
		//		if ((teps-Tg_e[a])*(teps-Tg_e[ab])<0)
		//			b=ab;
		//		else 
		//			a=ab;
		//	}//while
		//	de=fabs(teps-Tg_e[a]);
		//	de1=fabs(teps-Tg_e[b]);
		//	if(de<de1)
		//		ti=a;
		//	else
		//	{
		//		ti=b; 
		//		de=de1;
		//	}			
		//	//ti=(a+b)/2;
		//	H=Rel[ti].y;
		//	if (de<=0.01)//de<=0.5*da/57.3)
		//		OK=true;
		//} //polovin. delenie

		//if (OK)
		//{
		//	td=(H-(Rel[0].y+vect_sost.Hg_rv));
		//	L=sqrt(td*td+ti*dL*ti*dL);  //m
		//	if (L>MAX_Dist_RLS)
		//		L = Lmax;    					
		//}//OK
		//else
		//{
		//	L = Lmax; 
		//	ti = N_dots-1;
		//}

		//if (ti == N_dots-1)//заглушка для логики
		//	L = Lmax; 

		vect_sost.D_naklon2 = dist2;//L;

		//отрисовка расчитанного луча
		//if (F6_on)
		//{
		//	glBegin(GL_LINES);	 	
		//	glColor3f(1.0, 0.5, 1.0);
		//	glVertex3f(vect_sost.planeX, vect_sost.planeY, vect_sost.planeZ );
		//	glVertex3f(Rel[ti].x, Rel[ti].y, Rel[ti].z);
		//	glEnd();
		//}

		//Отправка данных
		float FloatVals[56];
		memset(FloatVals,0,sizeof(FloatVals));
		FloatVals[8] = vect_sost.D_naklon2 ;
		FloatVals[9] = -angle_vis_RLS+vect_sost.tang;
		FloatVals[10] = angle_azimuth_RLS+vect_sost.kurs;
		XPLMSetDatavf(gControl_ail2_def, FloatVals, 0, 56);

		//вывод двнных в файл
		if (0)
		{
			static char flag_rec = 0;
			if (vect_sost.time>5 && !flag_rec)
			{
				flag_rec = 1;
				file_data = fopen("data_forVB.txt","wt");


				fprintf(file_data,"angle_vis_RLS\n");
				fprintf(file_data,"%f\n", angle_vis_RLS);
				fprintf(file_data,"vect_sost.tang\n");
				fprintf(file_data,"%f\n", vect_sost.tang);
				fprintf(file_data,"vect_sost.Hg_rv\n");
				fprintf(file_data,"%f\n", vect_sost.Hg_rv);
				fprintf(file_data,"vect_sost.D_naklon2\n");
				fprintf(file_data,"%f\n", vect_sost.D_naklon2);

				fprintf(file_data,"Rel\t");
				fprintf(file_data,"Tg_e\n");

				for (int i = 0; i<N_dots; i++)
				{
					fprintf(file_data,"%f\t", Rel[i].y);
					//fprintf(file_data,"%f\n", Tg_e[i]);
				}

				fclose(file_data);
			}
		}



		//отрисовка заданного луча
		if (F6_on)
		{
			Tvector3 line_beam;
			//формирование координат x и z рельефа (в которых будем искать высоту рельефа)
			float kurs_rad = (vect_sost.kurs+angle_azimuth_RLS)*M_PI/180.f;
			line_beam.x = vect_sost.planeX+sin(kurs_rad)*MAX_Dist_RLS;
			line_beam.z = vect_sost.planeZ-cos(kurs_rad)*MAX_Dist_RLS;
			line_beam.y = vect_sost.planeY+tan((-angle_vis_RLS+vect_sost.tang)*M_PI/180.f)*MAX_Dist_RLS;
		 
			glBegin(GL_LINES);	 	
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(vect_sost.planeX, vect_sost.planeY, vect_sost.planeZ );
			glVertex3f(line_beam.x, line_beam.y, line_beam.z);
			glEnd();
		}
	}	
	return 1;
} 



const int dL=1; //m
const int Lmax=5000; //m - dalnost BRLK
const int N=Lmax/dL +1; //tochek rel'efa
const int TET=0; //grad - tangazh
const int Hrv=100; //m - PB


double BRLK(double angle, double TET, int Hrv, int N, double Hrel[], double dL){

	const double maxdbl=1e15;
	double td,de,teps,de1,L,eps;
	L=100000;
	bool OK;
	int i,ti;
	double* Tg_e; //tangens
	Tg_e=new double [N];
	td=Tg_e[0]=-maxdbl; //tangensi uglov vozvyshenia
	for(i=1;i<N;i++){
		Tg_e[i]=(Hrel[i]-(Hrel[0]+Hrv))/double(i*dL); //tangensi uglov vozvyshenia

		if(Tg_e[i]<=td)Tg_e[i] = td;
		else td=Tg_e[i];
	}//i
	eps=(TET+angle)/57.3; //rad
	teps=tan(eps);
	if(teps> Tg_e[N-1]){  //angle  vyshe rel'efa
		delete[] Tg_e;
		return (L);
	}
	de=maxdbl; // minimalnaya nev'iazka Tg
	OK=false;
	{ //polovin. delenie
		int a=1,b=N-1,ab;
		while(b-a>1){
			ab=(a+b)/2;  //seredina
			if((teps-Tg_e[a])*(teps-Tg_e[ab])<0)b=ab;
			else a=ab;

		}//while
		de=fabs(teps-Tg_e[a]);
		de1=fabs(teps-Tg_e[b]);
		if(de<de1)ti=a;
		else{ti=b; de=de1;}
		if(de<=0.01)OK=true;
	} //polovin. delenie

	if(OK){
		L=ti*dL*sqrt(1+Tg_e[ti]*Tg_e[ti]);
		if(L>Lmax)L=100000;
	}//OK
	delete[] Tg_e;
	return (L);
}//BRLK