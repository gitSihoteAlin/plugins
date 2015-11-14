#pragma once

typedef struct _Tvect_sost
{
	float planeX;	//текущее положение
	float planeY;
	float planeZ;

	float wx;
	float wy;
	float wz;

	float nx;
	float ny;
	float nz;

	float alfa;
	float beta;

	float tang;
	float kren;
	float kurs;

	float kren_zad;

	float Hg_rv;
	float Hg;
	float Hgf;
	float Hb;
	float Hr;
	float Hzad;
	float Vy;
	float dVy;
	float Vpr;

	float time;
	float dt;

	float JOY_X;
	float JOY_Z;
	float JOY_Y;
	float JOY_H;

	float D_naklon;
	float D_naklon2;

} Tvect_sost;

typedef struct _Tvect_upr
{
	float dx;
	float dz;
	float osh;
	float rv;
	float DIR_X;
	float DIR_Z;
	float DIR_H;
	float angle_vis;
	float angle_azimuth;

	//TEST
	float Vymax;
	float VyRV;
	float VyRLS;
} Tvect_upr;

// typedef struct _Tvector3 
// {
// 	float x;
// 	float y;
// 	float z;
// }Tvector3;
// 
// typedef struct _TColl
// {
// 	Tvector3 coord;
// 	float Dist;
// } TColl;

typedef struct _Tpoint
{
	float epsil;	//угол возвышения (визирования) i-й вершины относительно строительной оси ЛА – измерен БРЛК
	float alfa;		//угол возвышения (визирования) i-й вершины относительно горизонта
	float L;		//наклонная дальность до i-й вершины – измерена БРЛК
	float S;		//горизонтальная дальность до i-й вершины
	float t;		//ожидаемое время полета ЛА до i-й вершины 
	float Htr;		//требуемая относительная  высота пролёта i-й вершины
	float Vytr;		//требуемая вертикальная скорость 

}Tpoint;
