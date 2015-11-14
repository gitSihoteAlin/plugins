
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
#include <math.h>

#include <vector>
#include <queue>

#include "XPLMDefs.h"
#include "XPLMDisplay.h"
#include "XPLMDataAccess.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"

#define XPLM200
#include "XPLMScenery.h"

#include <gl\gl.h>
#include <gl\glu.h>

#define _USE_MATH_DEFINES
#include <math.h>

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

XPLMHotKeyID	gHotKeyF1 = NULL;
XPLMHotKeyID	gHotKeyF5 = NULL;
XPLMHotKeyID	gHotKeyF6 = NULL;
XPLMHotKeyID	gHotKeyF7 = NULL;
XPLMHotKeyID	gHotKeyF8 = NULL;
void	MyHotKeyCallbackF1(void *               inRefcon); 
void	MyHotKeyCallbackF5(void *               inRefcon); 
void	MyHotKeyCallbackF6(void *               inRefcon); 
void	MyHotKeyCallbackF7(void *               inRefcon); 
void	MyHotKeyCallbackF8(void *               inRefcon); 
char F1_on = 1;
char F5_on = 1;
char F6_on = 0;
char F7_on = 0;
char F8_on = 0;

//XPLMObjectRef frigate;
//XPLMDrawInfo_t *frigatestruct = new XPLMDrawInfo_t();

XPLMProbeRef Probe;
XPLMProbeInfo_t * yprobestruct = new XPLMProbeInfo_t();

//void drawSphere(double r, int lats, int longs);

struct  
{
	float D;
	float angle;
}range_data[181];

float angle_vis_ekr;
float D_ekr;
float angle_beam_ekr;

float angle_vis;	//угол места
float angle_azimuth;//азимут

//font
GLvoid BuildFont(GLvoid);
GLvoid KillFont(GLvoid);
GLvoid glPrint(const char *fmt, ...);					// Custom GL "Print" Routine

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

GLuint	base;				// Base Display List For The Font Set
GLfloat	cnt1;				// 1st Counter Used To Move Text & For Coloring
GLfloat	cnt2;				// 2nd Counter Used To Move Text & For Coloring

//static XPLMDataRef	gControl_ail1_def = NULL;
static XPLMDataRef	gControl_ail2_def = NULL;
static XPLMDataRef	gControl_splr2_def = NULL;
static XPLMHotKeyID gExampleGaugeHotKey = NULL;

static XPLMDataRef gPlane_Vpr;
static XPLMDataRef gPlane_Hg;
static XPLMDataRef gPlane_Vy;
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

XPLMDataRef gPlane_alfa;
XPLMDataRef gPlane_beta;

static int DirectorsDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void * inRefcon);
static int DistanceDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void * inRefcon);


PLUGIN_API int XPluginStart(
						char *		outName,
						char *		outSig,
						char *		outDesc)
{

	strcpy(outName, "Directors_and_Radar_From_Cha");
	strcpy(outSig, "xpsdk.experimental.directors");
	strcpy(outDesc, "A plug-in by Andrey Ch for displaying directors and rangefinder.");


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

	//gControl_ail1_def = XPLMFindDataRef("sim/flightmodel/controls/ail1_def");
	gControl_ail2_def = XPLMFindDataRef("sim/flightmodel/controls/ail2_def");
	gControl_splr2_def = XPLMFindDataRef("sim/flightmodel/controls/splr2_def");

	gPlane_Vpr	= XPLMFindDataRef("sim/cockpit2/gauges/indicators/airspeed_kts_pilot");
	gPlane_Hg	= XPLMFindDataRef("sim/cockpit2/gauges/indicators/radio_altimeter_height_ft_pilot");
	//gPlane_Vy	= XPLMFindDataRef("sim/cockpit2/gauges/indicators/vvi_fpm_pilot");
	//gPlane_Vy	= XPLMFindDataRef("sim/flightmodel/position/vh_ind");
	gPlane_Vy	= XPLMFindDataRef("sim/flightmodel/position/local_vy");

	gHWND	= XPLMFindDataRef("sim/operation/windows/system_window");

	gPlaneX = XPLMFindDataRef("sim/flightmodel/position/local_x");
	gPlaneY = XPLMFindDataRef("sim/flightmodel/position/local_y");
	gPlaneZ = XPLMFindDataRef("sim/flightmodel/position/local_z");

	gPlane_tang = XPLMFindDataRef("sim/flightmodel/position/theta");
	gPlane_kren = XPLMFindDataRef("sim/flightmodel/position/phi");
	gPlane_kurs = XPLMFindDataRef("sim/flightmodel/position/psi");

	//gPlane_alfa	= XPLMFindDataRef("sim/flightmodel/position/alpha");
	//gPlane_beta	= XPLMFindDataRef("sim/flightmodel/position/beta");

	gHotKeyF1 = XPLMRegisterHotKey(XPLM_VK_F1, xplm_ShiftFlag | xplm_DownFlag, 
		"Show Directors",
		MyHotKeyCallbackF1,
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

	//Для определитель высоты земной поверхности
	Probe = XPLMCreateProbe(xplm_ProbeY);
	yprobestruct->structSize = sizeof(XPLMProbeInfo_t);

	return 1;
}

PLUGIN_API void	XPluginStop(void)
{
	XPLMUnregisterHotKey(gHotKeyF1);
	XPLMUnregisterHotKey(gHotKeyF5);
	XPLMUnregisterHotKey(gHotKeyF6);
	XPLMUnregisterHotKey(gHotKeyF7);
	XPLMUnregisterHotKey(gHotKeyF8);
	XPLMUnregisterDrawCallback(DirectorsDrawCallback,xplm_Phase_FirstCockpit,0,NULL);
	XPLMUnregisterDrawCallback(DistanceDrawCallback,xplm_Phase_Objects, 0, NULL);
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
	if (F1_on)
	{

		Hg	 = XPLMGetDataf(gPlane_Hg)*0.30479f;//футы в метры
		//Vy	 = XPLMGetDataf(gPlane_Vy)*0.00508f;//футы/мин в метры/сек
		Vy	 = XPLMGetDataf(gPlane_Vy);
		Vpr	 = XPLMGetDataf(gPlane_Vpr)*1.852f;//узлы в км/ч

		/* Fetch the plane's location at this instant in OGL coordinates. */ 
		float planeX, planeY, planeZ;//текущее положение
		planeX = XPLMGetDataf(gPlaneX);
		planeY = XPLMGetDataf(gPlaneY);
		planeZ = XPLMGetDataf(gPlaneZ);

		float tang, kren, kurs;//углы эйлера
		//float alfa, beta;//аэродинамические

		tang = XPLMGetDataf(gPlane_tang);
		kren = XPLMGetDataf(gPlane_kren);
		kurs = XPLMGetDataf(gPlane_kurs);

		//alfa = XPLMGetDataf(gPlane_alfa);
		//beta = XPLMGetDataf(gPlane_beta);
	
	
		/// Do the actual drawing, but only if our window is active
		float FloatVals[56];
		//XPLMGetDatavf(gControl_ail1_def, FloatVals, 0, 56);
		XPLMGetDatavf(gControl_splr2_def, FloatVals, 0, 56);
	
		Dir_kren = FloatVals[8]*100;
		Dir_tang = FloatVals[9]*100;
		Dir_rosh = FloatVals[10]*100;

		angle_vis		= FloatVals[11];
		angle_azimuth	= FloatVals[12];
	
	
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
			glPrint("Vpr = %3.0f km/h", Vpr);
			//glRasterPos2f(280, 0);
			glRasterPos2f(170, 0);
			//glPrint("Vy = %2.1f m/s", Vy);
			glPrint("Vy = %2.2f m/s", Vy);
			glRasterPos2f(170, 100);
			glPrint("Hg = %3.0f m", Hg);

			if (F5_on)
			{
				glRasterPos2f(300, 200);
				glPrint("D_naklon = %5.0f", D_ekr);
			}

			if (F6_on)
			{
				glRasterPos2f(300, 300);
				glPrint("angle_vis = %3.1f", angle_vis);	
				glRasterPos2f(300, 250);
				glPrint("azimuth = %3.1f", angle_azimuth);
			}
	
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glLineWidth(1);
		glPopMatrix();
	
		glFlush();
	}
	return 1;
}

void	MyHotKeyCallbackF1(void *               inRefcon)
{
	F1_on = !F1_on;
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
		"Arial");					// Font Name

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

//Функция для определения дальности и отрисовки измерителя
int DistanceDrawCallback(
XPLMDrawingPhase inPhase, 
int inIsBefore, 
void * inRefcon)
{
		float planeX, planeY, planeZ;//текущее положение
		float tang, kren, kurs;//углы эйлера
		float alfa, beta;//аэродинамические
		float Hg;//

	/* If any data refs are missing, do not draw. */
	if (!gPlaneX || !gPlaneY || !gPlaneZ)
	return 1;

	/* Fetch the plane's location at this instant in OGL coordinates. */ 
	planeX = XPLMGetDataf(gPlaneX);
	planeY = XPLMGetDataf(gPlaneY);
	planeZ = XPLMGetDataf(gPlaneZ);

	tang = XPLMGetDataf(gPlane_tang);
	kren = XPLMGetDataf(gPlane_kren);
	kurs = XPLMGetDataf(gPlane_kurs);

	float FloatVals[56];
	//XPLMGetDatavf(gControl_ail1_def, FloatVals, 0, 56);
	XPLMGetDatavf(gControl_splr2_def, FloatVals, 0, 56);

	//учет угла крена
	float sin_kr = sin(kren*M_PI/180.f);
	float cos_kr = cos(kren*M_PI/180.f);
	angle_vis		=  FloatVals[11]*cos_kr+FloatVals[12]*sin_kr;
	angle_azimuth	=  -FloatVals[11]*sin_kr+FloatVals[12]*cos_kr;

	//alfa = XPLMGetDataf(gPlane_alfa);
	//beta = XPLMGetDataf(gPlane_beta);

	//Hg	 = XPLMGetDataf(gPlane_Hg)*0.30479f;//футы в метры


	//static float planeX_0	=	planeX;//начальное положение
	//static float planeY_0	=	planeY;
	//static float planeZ_0	=	planeZ;
	 

	/* Reset the graphics state. This turns off fog, texturing, lighting,
	* alpha blending or testing and depth reading and writing, which
	* guarantees that our axes will be seen no matter what. */
	XPLMSetGraphicsState(0, 0, 1, 1, 1, 0, 0);

	
	if (F5_on)
	{
		//максимальная дальность измерителя
		float Distance_to_ground = 100000;
		//угол места
		float fi_mesta = (angle_vis-tang)*M_PI/180.f;;

		//scaner
		float shag = 100;//meter
		const int DOTS = 50;//5000
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

			float kurs_rad = (kurs+angle_azimuth)*M_PI/180.f;
			x_beam = sin(kurs_rad)*D_xz;
			z_beam = -cos(kurs_rad)*D_xz;

			koord_beam[i].x = planeX + x_beam;
			koord_beam[i].y = planeY + y_beam;
			koord_beam[i].z = planeZ + z_beam;

			XPLMProbeTerrainXYZ(Probe,planeX+x_beam, 0 , planeZ+z_beam, yprobestruct);

			koord_ground[i].x = yprobestruct->locationX;
			koord_ground[i].y = yprobestruct->locationY;
			koord_ground[i].z = yprobestruct->locationZ;


			if (planeY + y_beam - koord_ground[i].y<0 && i>0)
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

					Distance_to_ground = CalcDist3D(planeX, planeY, planeZ, our_point.x, our_point.y, our_point.z);

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


		D_ekr = Distance_to_ground;

		if (F8_on)
		{
			glBegin(GL_LINES);	 	
			for (int i = 0; i<i_100; i++)
			{
				glColor3f(1.0, 0.5, 1.0);
				glVertex3f(planeX, planeY, planeZ );
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
				glVertex3f(planeX, planeY, planeZ );
				glVertex3f(our_point.x, our_point.y, our_point.z);
				glEnd();
			}

			else
			{
				glBegin(GL_LINES);	 	
				glColor3f(1.0, 0.8, 1.0);
				glVertex3f(planeX, planeY, planeZ );
				glVertex3f(koord_beam[i_100-1].x, koord_beam[i_100-1].y, koord_beam[i_100-1].z);
				glEnd();
			}
		}

		//Отправка данных
		float FloatVals[56];
		memset(FloatVals,0,sizeof(FloatVals));
		FloatVals[8] = Distance_to_ground;
		XPLMSetDatavf(gControl_ail2_def, FloatVals, 0, 56);

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
	}
	return 1;
} 