
/*
	plugin_9_Directors_and_Rangefinder - Andrey Ch 2014
	Compiled 10/05/2014

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
	Shift-F1 - Show directors
	Shift-F5 - Rangefinder
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

#include "typedefs.h"
#include "RMNK_str.h"

std::deque<float> the_queue_Y;

XPLMHotKeyID	gHotKeyF1 = NULL;
XPLMHotKeyID	gHotKeyF2 = NULL;
XPLMHotKeyID	gHotKeyF5 = NULL;
XPLMHotKeyID	gHotKeyF6 = NULL;
XPLMHotKeyID	gHotKeyF7 = NULL;
XPLMHotKeyID	gHotKeyF8 = NULL;
void	MyHotKeyCallbackF1(void *               inRefcon); 
void	MyHotKeyCallbackF2(void *               inRefcon); 
void	MyHotKeyCallbackF5(void *               inRefcon); 
void	MyHotKeyCallbackF6(void *               inRefcon); 
void	MyHotKeyCallbackF7(void *               inRefcon); 
void	MyHotKeyCallbackF8(void *               inRefcon); 
char F1_on = 0;
char F2_on = 0;
char F5_on = 0;
char F6_on = 0;
char F7_on = 1;
char F8_on = 0;


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
static XPLMDataRef gPlane_Hg;
static XPLMDataRef gPlane_Hb;
static XPLMDataRef gPlane_Vy;
static XPLMDataRef gPlane_dVy;
static XPLMDataRef gHWND;

static float Dir_kren;
static float Dir_tang;
static float Dir_rosh;

static float Hg;
static float Vy;
static float Vpr;

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
//XPLMDataRef gControl_yoke_pitch_ratio;

int AutopilotDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void * inRefcon);

TRmnkVar RmnkVar;

FILE *file_data;

PLUGIN_API int XPluginStart(
						char *		outName,
						char *		outSig,
						char *		outDesc)
{
	RmnkVar.P_0 = 1000;
	RmnkVar.n_p = 6;
	RMNK_sNU(RmnkVar);
	RmnkVar.n_p = 6;

	strcpy(outName, "Directors_and_Radar_From_Cha");
	strcpy(outSig, "xpsdk.experimental.directors");
	strcpy(outDesc, "A plug-in by Andrey Ch for displaying directors and rangefinder.");


	XPLMRegisterDrawCallback(
		AutopilotDrawCallback,
		xplm_Phase_Window,//xplm_Phase_FirstCockpit, /* Draw when sim is doing objects */
		0, /* After objects */
		NULL); /* No refcon needed */

	gControl_ail1_def = XPLMFindDataRef("sim/flightmodel/controls/ail1_def");
	gControl_ail2_def = XPLMFindDataRef("sim/flightmodel/controls/ail2_def");
	gControl_splr2_def = XPLMFindDataRef("sim/flightmodel/controls/splr2_def");

	gPlane_Vpr	= XPLMFindDataRef("sim/cockpit2/gauges/indicators/airspeed_kts_pilot");
	gPlane_Hg	= XPLMFindDataRef("sim/cockpit2/gauges/indicators/radio_altimeter_height_ft_pilot");
	gPlane_Hb	= XPLMFindDataRef("sim/cockpit2/gauges/indicators/altitude_ft_pilot");
	
	//gPlane_Vy	= XPLMFindDataRef("sim/cockpit2/gauges/indicators/vvi_fpm_pilot");
	//gPlane_Vy	= XPLMFindDataRef("sim/flightmodel/position/vh_ind");
	gPlane_Vy	= XPLMFindDataRef("sim/flightmodel/position/local_vy");
	gPlane_dVy	= XPLMFindDataRef("sim/flightmodel/position/local_ay");

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
	//gControl_yoke_pitch_ratio	= XPLMFindDataRef("sim/joystick/yoke_pitch_ratio");

	gHotKeyF1 = XPLMRegisterHotKey(XPLM_VK_F1, xplm_ShiftFlag | xplm_DownFlag, 
		"Show Directors",
		MyHotKeyCallbackF1,
		NULL);
	gHotKeyF2 = XPLMRegisterHotKey(XPLM_VK_F2, xplm_ShiftFlag | xplm_DownFlag, 
		"Autopilot",
		MyHotKeyCallbackF2,
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


	XPLMSetDatai(gOverride_joystick,1);
	//////////////////////////////////////////////////////////////////////////

	file_data = fopen("data_cha.txt","wt");
	if (file_data)
	{
		fprintf(file_data,"time\t");
		fprintf(file_data,"kren\t");
		fprintf(file_data,"tang\t");
		fprintf(file_data,"Vy\t");
		fprintf(file_data,"Hb\t");
		fprintf(file_data,"Hzad\t");
		fprintf(file_data,"osh\t");
		fprintf(file_data,"e\t");
		fprintf(file_data,"dVy\t");
		fprintf(file_data,"A[0]\t");
		fprintf(file_data,"A[2]\t");
		fprintf(file_data,"A[3]\t");
		fprintf(file_data,"A[5]\t");
		fprintf(file_data,"\n");
	}
	return 1;
}

PLUGIN_API void	XPluginStop(void)
{
	XPLMUnregisterHotKey(gHotKeyF1);
	XPLMUnregisterHotKey(gHotKeyF2);
	XPLMUnregisterHotKey(gHotKeyF5);
	XPLMUnregisterHotKey(gHotKeyF6);
	XPLMUnregisterHotKey(gHotKeyF7);
	XPLMUnregisterHotKey(gHotKeyF8);
	XPLMUnregisterDrawCallback(AutopilotDrawCallback,xplm_Phase_FirstCockpit, 0, NULL);

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

Tvect_sost vect_sost;
Tvect_upr vect_upr;

void Limit(float & f, float lim)
{
	if (f>lim) f = lim;
	if (f<-lim) f = -lim;
}

int	AutopilotDrawCallback(
	XPLMDrawingPhase     inPhase,
	int                  inIsBefore,
	void *               inRefcon)
{
	static char flag_sit = 1;
	if (flag_sit)
	{
		flag_sit = 0;
		XPLMLoadDataFile(xplm_DataFile_Situation, "Output\\situations\\10_500.sit");
		F2_on = 1;
	}
	if (F2_on)
	{
		//Get data
		vect_sost.wx = XPLMGetDataf(gPlane_wx);
		vect_sost.wy = -XPLMGetDataf(gPlane_wz); //faire attention sur la différence aux axes de coordonnées! 
		vect_sost.wz = XPLMGetDataf(gPlane_wy);

		vect_sost.nx = -XPLMGetDataf(gPlane_nx);
		vect_sost.ny = XPLMGetDataf(gPlane_ny);
		vect_sost.nz = XPLMGetDataf(gPlane_nz);

		vect_sost.alfa = XPLMGetDataf(gPlane_alfa);
		vect_sost.beta = XPLMGetDataf(gPlane_beta);

		vect_sost.tang = XPLMGetDataf(gPlane_tang);
		vect_sost.kren = XPLMGetDataf(gPlane_kren);
		vect_sost.kurs = XPLMGetDataf(gPlane_kurs);

		vect_sost.Hg	 = XPLMGetDataf(gPlane_Hg)*0.30479f;//футы в метры
		vect_sost.Hb	 = XPLMGetDataf(gPlane_Hb)*0.30479f;//футы в метры

		vect_sost.Hr = vect_sost.Hb - vect_sost.Hg;

		vect_sost.Vy	 = XPLMGetDataf(gPlane_Vy);
		vect_sost.dVy	 = XPLMGetDataf(gPlane_dVy);
		vect_sost.Vpr	 = XPLMGetDataf(gPlane_Vpr)*1.852f;//узлы в км/ч

		vect_sost.time	 = XPLMGetDataf(gPlane_time);

		static float old_time = vect_sost.time;
		vect_sost.dt	= vect_sost.time-old_time;
		old_time = vect_sost.time;


// 		float fff[8];
// 		XPLMGetDatavf(gPOINT_pitch_deg, fff, 0, 8);
// 		float f = fff[0];

		if (F8_on)
			Paint_Hg();
		else
			Paint_all();

		static char step = 0;
		static char flag_start = 0;
		static float start_time = 0;
		if (!flag_start && vect_sost.time>20)
		{
			flag_start = 1;
			start_time = vect_sost.time;
		}

		static char flag_stop = 0;			
		if (!flag_stop && (vect_sost.time-start_time)>40) 
		{
			flag_stop = 1;
		}

		if (flag_start)
		{
			if ((vect_sost.time-start_time)>40)
				step = 5;
			else if ((vect_sost.time-start_time)>30)
				step = 4;
			else if ((vect_sost.time-start_time)>20)
				step = 3;
			else if ((vect_sost.time-start_time)>10)
				step = 2;
			else 
				step = 1;
		}

		if (1)//крен
		{

			//static bool bbb = false;
			//static float old_time= vect_sost.time;
			//float dt_ = vect_sost.time-old_time;
			//if (dt_>15)
			//{
			//	old_time= vect_sost.time;
			//	bbb = !bbb;
			//}


			float kren_zad = 0;
			switch (step)
			{
			case 1: kren_zad = 0; break;
			case 2: kren_zad = 40; break;
			case 3: kren_zad = -40; break;
			case 4: kren_zad = 0; break;
			}

			//апериодический фильтр
			if (1)
			{
				float T = 2.;
				static float y2;
				float x = kren_zad;
				y2+= (1/T*(x-y2))*vect_sost.dt;
				kren_zad = y2;				
			}

	
			float dx = 1.5*(kren_zad - vect_sost.kren) - 0.5*vect_sost.wx;

			dx = dx/30.f;

			Limit(dx,1);
			XPLMSetDataf(gControl_yoke_roll_ratio,dx);	
		

			if (1)//танг
			{
	
				float Vzad = 220;

				switch (step)
				{
				case 1: Vzad = 220; break;
				case 2: Vzad = 100; break;
				case 3: Vzad = 180; break;
				case 4: Vzad = 250; break;
				case 5: Vzad = 0; break;
				}

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

				float dz = 1.4*(tang_zad - vect_sost.tang) - 1.0*vect_sost.wz;// + 0.2*izod_wz;

				dz = dz/30.f;
				Limit(dz,1);
				XPLMSetDataf(gControl_yoke_pitch_ratio,dz);	
			}
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
			float Hzad = 300;
			//float Htek = vect_sost.Hg;
			float Htek = vect_sost.Hb;

			//апериодический фильтр
			if (0)
			{
				float T = 2.;
				static float y2;
				float x = vect_sost.Hg;
				y2+= (1/T*(x-y2))*vect_sost.dt;
				Htek = y2;				
			}


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

			static char on_H_auto = 0;
			if (!on_H_auto && Htek>Hzad)
				on_H_auto = 1;


			//ПИД регулятор
			float Xosh = -5;
			if (on_H_auto)
			{
				float raznH = Hzad-Htek; 
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

				//Limit(x1,50);
				//Limit(x2,50);
				//Limit(y1,50);
				if (fabs(raznH)>100)
				{
					x1 = 0;
					x2 = 0;
					y1 = 0;
				}

				float dt = vect_sost.dt;

				x1+=x*dt;
				x2+=x1*dt;
				y1+=y*dt;
				y = 1/T*(k1*x+k2*x1+k3*x2-y1);

				Xosh = y;
			}

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

			
			if (file_data && flag_start && !flag_stop)
			{
				fprintf(file_data,"%f\t", vect_sost.time-start_time);
				fprintf(file_data,"%f\t", vect_sost.kren);
				fprintf(file_data,"%f\t", vect_sost.tang);
				fprintf(file_data,"%f\t", vect_sost.Vy);
				fprintf(file_data,"%f\t", vect_sost.Hb);
				fprintf(file_data,"%f\t", Hzad);
				fprintf(file_data,"%f\t", vect_upr.osh);
				fprintf(file_data,"%f\t", e_him/2.0*100);//max dVy = 2.0 //100%
				fprintf(file_data,"%f\t", vect_sost.dVy);
				fprintf(file_data,"%f\t", RmnkVar.A[0]);
				fprintf(file_data,"%f\t", RmnkVar.A[2]);
				fprintf(file_data,"%f\t", RmnkVar.A[3]);
				fprintf(file_data,"%f\t", RmnkVar.A[5]);
				fprintf(file_data,"\n");
			}
	}
	return 1;

}


void	MyHotKeyCallbackF1(void *               inRefcon)
{
	F1_on = !F1_on;
}

void	MyHotKeyCallbackF2(void *               inRefcon)
{
	F2_on = !F2_on;
}

void	MyHotKeyCallbackF5(void *               inRefcon)
{
	F5_on = !F5_on;
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


GLvoid BuildFont(GLvoid)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

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
	bool bbb = wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
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