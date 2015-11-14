/*
	plugin_9_MAP - Andrey Ch 2015
	Compiled 10/05/2015

	This plugin ...

	Hot keys:
	Shift-F11 - get map
	Shift-F12 - show grid
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

void Limit(float & f, float lim)
{
	if (f>lim) f = lim;
	if (f<-lim) f = -lim;
}

std::deque<TColl> the_queue;

XPLMProbeRef Probe;
XPLMProbeInfo_t * yprobestruct = new XPLMProbeInfo_t();


//////////////////////////////////////////////////////////////////////////


#include "typedefs.h"

static TMatrix_CHA M;


XPLMHotKeyID	gHotKeyF11 = NULL;
XPLMHotKeyID	gHotKeyF12 = NULL;

void	MyHotKeyCallbackF11(void *               inRefcon); 
void	MyHotKeyCallbackF12(void *               inRefcon); 

char F11_on = 0;
char F12_on = 0;


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

static XPLMDataRef gHWND;

static XPLMDataRef gPlaneX;
static XPLMDataRef gPlaneY;
static XPLMDataRef gPlaneZ;

static XPLMDataRef gPause;

int funCallBack1(XPLMDrawingPhase inPhase, int inIsBefore, void * inRefcon);
int funCallBack2(XPLMDrawingPhase inPhase, int inIsBefore, void * inRefcon);


FILE *file_data_matrix;
FILE * pFile;

PLUGIN_API int XPluginStart(
						char *		outName,
						char *		outSig,
						char *		outDesc)
{
	strcpy(outName, "maps_cha");
	strcpy(outSig, "xpsdk.experimental.maps");
	strcpy(outDesc, "A plug-in by Andrey Ch for maps.");

	gHWND	= XPLMFindDataRef("sim/operation/windows/system_window");

	gPlaneX = XPLMFindDataRef("sim/flightmodel/position/local_x");
	gPlaneY = XPLMFindDataRef("sim/flightmodel/position/local_y");
	gPlaneZ = XPLMFindDataRef("sim/flightmodel/position/local_z");

	gPause = XPLMFindDataRef("sim/time/paused");	

	/// Register 
	XPLMRegisterDrawCallback(
					funCallBack1,
					//xplm_Phase_Gauges,
					xplm_Phase_Objects,
					0,
					NULL);

	/* Next register teh drawing callback. We want to be drawn 
	* after X-Plane draws its 3-d objects. */
	XPLMRegisterDrawCallback(
		funCallBack2,
		xplm_Phase_FirstCockpit, /* Draw when sim is doing objects */
		0, /* After objects */
		NULL); /* No refcon needed */


	gHotKeyF11 = XPLMRegisterHotKey(XPLM_VK_F11, xplm_ShiftFlag | xplm_DownFlag, 
		"Start getting Map",
		MyHotKeyCallbackF11,
		NULL);
	gHotKeyF12 = XPLMRegisterHotKey(XPLM_VK_F12, xplm_ShiftFlag | xplm_DownFlag, 
		"Show Map",
		MyHotKeyCallbackF12,
		NULL);

	//Для шрифта
	int ihWnd	 = XPLMGetDatai(gHWND);
	ihWnd = ihWnd;
	hWnd = (HWND) ihWnd;
	hDC = GetDC(hWnd);
	BuildFont();										// Build The Font


	//Для определителя высоты земной поверхности
	Probe = XPLMCreateProbe(xplm_ProbeY);
	yprobestruct->structSize = sizeof(XPLMProbeInfo_t);

	return 1;
}

PLUGIN_API void	XPluginStop(void)
{
	XPLMUnregisterHotKey(gHotKeyF11);
	XPLMUnregisterHotKey(gHotKeyF12);
	XPLMUnregisterDrawCallback(funCallBack1,xplm_Phase_FirstCockpit,0,NULL);
	XPLMUnregisterDrawCallback(funCallBack2,xplm_Phase_Objects, 0, NULL);
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

void	MyHotKeyCallbackF11(void *               inRefcon)
{

	F11_on = !F11_on;	
	//F12_on = 0;
}
void	MyHotKeyCallbackF12(void *               inRefcon)
{
	F12_on = !F12_on;
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

static char start_process = 0;//флаг процесса получения данных

//функция отрисовки сетки
int	funCallBack1(
                                   XPLMDrawingPhase     inPhase,
                                   int                  inIsBefore,
                                   void *               inRefcon)
{

	/* Reset the graphics state. This turns off fog, texturing, lighting,
	* alpha blending or testing and depth reading and writing, which
	* guarantees that our axes will be seen no matter what. */
	XPLMSetGraphicsState(0, 0, 1, 1, 1, 0, 0);

	static char first = 0;

	static char old_ffff = F12_on && !start_process;
	char ffff = F12_on && !start_process;
	if (!old_ffff && ffff )
		first = 1;
	else
		first = 0;

	old_ffff = F12_on && !start_process;

	if (F12_on && !start_process)
	{
		static TMatrix_CHA ma;
		//static char first = 1;
		//read file
		
		if (first)
		{
			//first = 0;
			pFile = fopen ( "matrix1.bin" , "rb" );
			if (pFile)
			{
				int result = fread (&ma,1,sizeof(TMatrix_CHA),pFile);	
				fclose(pFile);
			}
		}

		glColor3f(1, 0, 0);			 
		for (int j = 0; j<ma.quantityZ-1; j++)
		{			
			for (int i = 0; i<ma.quantityX-1; i++)
			{
				glBegin(GL_LINE_LOOP);
					glVertex3f(ma.X0 + ma.step*i, ma.alt[i][j], ma.Z0 + ma.step*j );
					glVertex3f(ma.X0 + ma.step*(i+1), ma.alt[i+1][j], ma.Z0 + ma.step*j );
					//glVertex3f(ma.X0 + ma.step*(i+1), ma.alt[i+1][i+1], ma.Z0 + ma.step*(i+1) );
					glVertex3f(ma.X0 + ma.step*i, ma.alt[i][j+1], ma.Z0 + ma.step*(j+1) );
					//glVertex3f(ma.X0 + ma.step*i, ma.alt[i][j], ma.Z0 + ma.step*j );
				glEnd();
			}
		}
	}
	return 1;
}

//Функция получения ЦКРМ
int funCallBack2(
XPLMDrawingPhase inPhase, 
int inIsBefore, 
void * inRefcon)
{

	float planeX, planeY, planeZ;//текущее положение

	int status_Pause;

	const float size_side = 10000;	//размер карты в метрах
	const float step = 300;		//шаг

	static float process_done = 0;

	/* If any data refs are missing, do not draw. */
	if (!gPlaneX || !gPlaneY || !gPlaneZ)
		return 1;

	/* Fetch the plane's location at this instant in OGL coordinates. */ 
	planeX = XPLMGetDataf(gPlaneX);
	planeY = XPLMGetDataf(gPlaneY);
	planeZ = XPLMGetDataf(gPlaneZ);	

	status_Pause = XPLMGetDatai(gPause);	

	XPLMSetGraphicsState(0, 0, 1, 1, 1, 0, 0);

	static float planeX0 = 0;
	static float planeY0 = 0;
	static float planeZ0 = 0;

	static char first_step = 1;

	static int row_m = 0;
	static int col_m = 0;

	static float deltaX = -size_side*0.5;//-100*shag;
	static float deltaZ = -size_side*0.5;//-100*shag;

	static char flag_ = 1;

	static char old_F11_on = F11_on;
	if (!old_F11_on && F11_on)
	{
		
		if (!status_Pause)
		{
			XPLMCommandKeyStroke(xplm_key_pause);
		}

		
		start_process = 1;

		//planeX0 = (int)planeX/100*100;
		//planeY0 = planeY;
		//planeZ0 = (int)planeZ/100*100;
		planeX0 = planeX;
		planeY0 = planeY;
		planeZ0 = planeZ;

		process_done = 0;

		first_step = 1;

		row_m = 0;
		col_m = 0;

		deltaX = -size_side*0.5;
		deltaZ = -size_side*0.5;

		flag_ = 1;
	}
	old_F11_on = F11_on;

	if (start_process)
	{

		M.mem_size_all = sizeof(TMatrix_CHA);
		//M.mem_size_mat = sizeof(TMatrix_CHA.alt);

		M.step = step;		
		
		if (first_step)
		{
			first_step = 0;
			//координаты левого нижнего угла
			M.X0 = planeX0+deltaX;
			M.Z0 = planeZ0+deltaZ;
		}


		//static float matrix_alt[1000][1000];

		if (deltaZ<=size_side*0.5)
		{
			if (deltaX>size_side*0.5) 
			{
				deltaX = -size_side*0.5;
				row_m = 0;
				M.Z1 = planeZ0+deltaZ;
				M.sizeZ = M.Z1-M.Z0;
				deltaZ+=step;					
				col_m++;		
				M.quantityZ = col_m;
			}
			else
			{
				if (row_m<MAX_MATR && col_m<MAX_MATR)
				{
					XPLMProbeTerrainXYZ(Probe,planeX0+deltaX, 0 , planeZ0+deltaZ, yprobestruct);

					Tvector3 temp;
					temp.x = yprobestruct->locationX;
					temp.y = yprobestruct->locationY;
					temp.z = yprobestruct->locationZ;

					M.alt[row_m][col_m] = yprobestruct->locationY;
				}
				else
					__asm nop;

				M.X1 = planeX0+deltaX;
				M.sizeX = M.X1-M.X0;
				deltaX+=step;

				row_m++;
				M.quantityX = row_m;
				M.quantity_all++;
			}
			process_done = deltaZ/size_side*100+50;
		}
		else
		{			
			if (flag_)
			{
				flag_ = 0;
				file_data_matrix = fopen("matrix1.bin","wb");
				if (file_data_matrix)
				{
					fwrite (&M , sizeof(char), M.mem_size_all, file_data_matrix);
					fclose(file_data_matrix);
					process_done = 200;
					start_process = 0;
				}
			}
		}
	}


	Tvector3 koord_ground;
	koord_ground.x = yprobestruct->locationX;
	koord_ground.y = yprobestruct->locationY;
	koord_ground.z = yprobestruct->locationZ;

	//Вывод на экран
	if (F11_on)
	{	
		int ourWidth, ourHight;
		XPLMGetScreenSize(&ourWidth, &ourHight);

		// Turn on Alpha Blending and turn off Depth Testing
		XPLMSetGraphicsState(0/*Fog*/, 1/*TexUnits*/, 0/*Lighting*/, 0/*AlphaTesting*/, 1/*AlphaBlending*/, 0/*DepthTesting*/, 0/*DepthWriting*/);
		glBlendFunc(GL_ONE, GL_ONE);

		//непонятная команда, влияющая на прозрачность директоров
		XPLMBindTexture2d(0, 0);

		glPushMatrix();
		glTranslatef(ourWidth/2, ourHight/2, 0); //центрируем				
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor3d(1,1,0);     // желтый цвет

		// Position The Text On The Screen
		float Xstr = 0;
		float Ystr = 300;
		float dYstr = 30;

		glRasterPos2f(Xstr, Ystr-dYstr*0);
		glPrint("planeX0 = %3.1f", planeX);	
		glRasterPos2f(Xstr, Ystr-dYstr*1);
		glPrint("planeZ0 = %3.1f", planeZ);
		glRasterPos2f(Xstr, Ystr-dYstr*2);
		glPrint("planeH0 = %3.1f", planeY);


		Xstr+=200;
		int nl = 0;
		glRasterPos2f(Xstr, Ystr-dYstr*nl++);
		glPrint("koord_ground.x = %3.1f", koord_ground.x);	
		glRasterPos2f(Xstr, Ystr-dYstr*nl++);
		glPrint("koord_ground.z = %3.1f", koord_ground.z);
		glRasterPos2f(Xstr, Ystr-dYstr*nl++);
		glPrint("koord_ground.h = %3.1f", koord_ground.y);
		glRasterPos2f(Xstr, Ystr-dYstr*nl++);
		if (process_done<150.f)
			glPrint("process = %3.1f%%", process_done);	
		else
			glPrint("process = done!");	

		//glRasterPos2f(Xstr, Ystr-dYstr*nl++);
		//glPrint("pause = %d", status_Pause);	


		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth(1);
		glPopMatrix();

		glFlush();
	}

	return 1;
} 