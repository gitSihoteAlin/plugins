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
	float epsil;	//���� ���������� (�����������) i-� ������� ������������ ������������ ��� �� � ������� ����
	float alfa;		//���� ���������� (�����������) i-� ������� ������������ ���������
	float L;		//��������� ��������� �� i-� ������� � �������� ����
	float S;		//�������������� ��������� �� i-� �������
	float t;		//��������� ����� ������ �� �� i-� ������� 
	float Htr;		//��������� �������������  ������ ������ i-� �������
	float Vytr;		//��������� ������������ �������� 

}Tpoint;

const int MAX_MATR = 3335;

typedef struct _TMatrix_CHA
{
	int mem_size_all;	//������ ���� ��������� � ������
	int mem_size_mat;	//������ ������� � ������
	float X0;			//���������� ������ ������� ����	
	float Z0;			//���������� ������ ������� ����
	float X1;			//���������� ������� �������� ����	
	float Z1;			//���������� ������� �������� ����
	float sizeX;		//�������	
	float sizeZ;		//�������
	float step;			//���	
	int quantityX;		//���������� �����	
	int quantityZ;		//���������� �����
	int quantity_all;	//���������� ����� ����
	float alt[MAX_MATR][MAX_MATR];	//���������� �������
}TMatrix_CHA;
