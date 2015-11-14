#pragma once

typedef struct _Tvect_sost
{
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

	float Hg;
	float Hb;
	float Hr;
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
	float epsil;	//угол возвышени€ (визировани€) i-й вершины относительно строительной оси Ћј Ц измерен Ѕ–Ћ 
	float alfa;		//угол возвышени€ (визировани€) i-й вершины относительно горизонта
	float L;		//наклонна€ дальность до i-й вершины Ц измерена Ѕ–Ћ 
	float S;		//горизонтальна€ дальность до i-й вершины
	float t;		//ожидаемое врем€ полета Ћј до i-й вершины 
	float Htr;		//требуема€ относительна€  высота пролЄта i-й вершины
	float Vytr;		//требуема€ вертикальна€ скорость 

}Tpoint;

const int MAX_MATR = 3335;

typedef struct _TMatrix_CHA
{
	int mem_size_all;	//размер всей структуры в байтах
	int mem_size_mat;	//размер матрицы в байтах
	float X0;			//координаты левого нижнего угла	
	float Z0;			//координаты левого нижнего угла
	float X1;			//координаты правого верхнего угла	
	float Z1;			//координаты правого верхнего угла
	float sizeX;		//размеры	
	float sizeZ;		//размеры
	float step;			//шаг	
	int quantityX;		//количество точек	
	int quantityZ;		//количество точек
	int quantity_all;	//количество точек всех
	float alt[MAX_MATR][MAX_MATR];	//собственно матрица
}TMatrix_CHA;
