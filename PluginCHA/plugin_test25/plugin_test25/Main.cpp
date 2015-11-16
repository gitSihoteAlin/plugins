
#if IBM
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include <vector>
// #include <queue>

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

XPLMDataRef		gPlaneX = NULL;
XPLMDataRef		gPlaneY = NULL;
XPLMDataRef		gPlaneZ = NULL;
XPLMDataRef		gPlaneTheta = NULL;
XPLMDataRef		gPlanePhi = NULL;
XPLMDataRef		gPlanePsi = NULL;
XPLMDataRef		gOverRidePlanePosition = NULL;
XPLMDataRef		gAGL = NULL;
XPLMDataRef		gLatRef = NULL;
XPLMDataRef		gLongRef = NULL;

XPLMDataRef		gPlane_time = NULL;

int AutopilotDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void * inRefcon);

#include "SU25_test.h"
SU25_test su25;
SU25_test::Out_data outSU25;
SU25_test::Ini_data iniSU25;
SU25_test::Inp_data inpSU25;

PLUGIN_API int XPluginStart(
						char *		outName,
						char *		outSig,
						char *		outDesc)
{
	strcpy(outName, "test_25");
	strcpy(outSig, "xpsdk.experimental.25");
	strcpy(outDesc, "A plug-in by Andrey Ch for ...");

	/* Prefetch the sim variables we will use. */
	gPlaneX = XPLMFindDataRef("sim/flightmodel/position/local_x");
	gPlaneY = XPLMFindDataRef("sim/flightmodel/position/local_y");
	gPlaneZ = XPLMFindDataRef("sim/flightmodel/position/local_z");
	gPlaneTheta = XPLMFindDataRef("sim/flightmodel/position/theta");
	gPlanePhi = XPLMFindDataRef("sim/flightmodel/position/phi");
	gPlanePsi = XPLMFindDataRef("sim/flightmodel/position/psi");
	gOverRidePlanePosition = XPLMFindDataRef("sim/operation/override/override_planepath");
	gAGL = XPLMFindDataRef("sim/flightmodel/position/y_agl");
	gLatRef = XPLMFindDataRef("sim/flightmodel/position/lat_ref");
	gLongRef = XPLMFindDataRef("sim/flightmodel/position/lon_ref");

	gPlane_time	= XPLMFindDataRef("sim/time/total_running_time_sec");


	XPLMRegisterDrawCallback(
		AutopilotDrawCallback,
		xplm_Phase_Window,//xplm_Phase_FirstCockpit, /* Draw when sim is doing objects */
		0, /* After objects */
		NULL); /* No refcon needed */

	return 1;
}

PLUGIN_API void	XPluginStop(void)
{

}

PLUGIN_API void XPluginDisable(void)
{
	int Enable[10];
	Enable[0] = 0;

	XPLMSetDatavi(gOverRidePlanePosition,&Enable[0],0,1); // disable physics
}

PLUGIN_API int XPluginEnable(void)
{
	int Enable[10];
	Enable[0] = 1;

	XPLMSetDatavi(gOverRidePlanePosition,&Enable[0],0,1); // disable physics

// 	float X = -12302;
// 	float Y = 605;
// 	float Z = 34271;
	//iniSU25.inp6DOF.Xe = vector3(-34271,12302,-3605);
	iniSU25.inp6DOF.Xe = vector3(-33000,12000,-3605);
	iniSU25.inp6DOF.Vb = vector3(100, 0, 0);
	iniSU25.inp6DOF.Rot;
	iniSU25.inp6DOF.wb;
	iniSU25.inp6DOF.mass = 15000;
	iniSU25.inp6DOF.Jx = 15000.0;
	iniSU25.inp6DOF.Jy = 67000.0;
	iniSU25.inp6DOF.Jz = 72500.0;
	iniSU25.inp6DOF.Jxz;

	su25.Init(iniSU25);
	
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(
					XPLMPluginID	inFromWho,
					int				inMessage,
					void *			inParam)
{
}

int	AutopilotDrawCallback(
	XPLMDrawingPhase     inPhase,
	int                  inIsBefore,
	void *               inRefcon)
{
	float time	 = XPLMGetDataf(gPlane_time);

	static float old_time = time;
	float dt	= time-old_time;
	old_time = time;

	if (0)
	{
		inpSU25.Elevator = 10;
		inpSU25.Rudder = 0;
		inpSU25.Ailerons = 0;
		inpSU25.gARUD_1 = 68;
		inpSU25.gARUD_2 = 68;
		inpSU25.Flaps = 0;
		su25.Calc(inpSU25, dt);
	}
	else
	{
 		su25.Autopilot(dt);
 		su25.Calc(dt);
	}

	outSU25 = su25.Get_Data();
	if (-outSU25.n.z>4)
		__asm nop

	float X = outSU25.outC6DOF.Xe.y;
	float Y = -outSU25.outC6DOF.Xe.z;
	float Z = -outSU25.outC6DOF.Xe.x;
	float pitch = outSU25.outC6DOF.Rot.y * 180/M_PI;
	float roll  = outSU25.outC6DOF.Rot.x * 180/M_PI;
	float yaw	= outSU25.outC6DOF.Rot.z * 180/M_PI;

	//X= -12302;
	//Y= 1605;
	//Z= 34271;
	//pitch = 0;
	//roll  =  0;
	//yaw	=  0;

	XPLMSetDataf(gPlaneX, X);
	XPLMSetDataf(gPlaneY, Y);
	XPLMSetDataf(gPlaneZ, Z);
	//float zg = XPLMGetDataf(gPlaneX);


	XPLMSetDataf(gPlaneTheta, pitch);
	XPLMSetDataf(gPlanePhi, roll);
	XPLMSetDataf(gPlanePsi, yaw);

	return 1;

}

