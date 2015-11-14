//---------------------------------------------------------------------------

#ifndef RMNK_strH
#define RMNK_strH
const int Nraz = 10;
typedef struct TRmnkVar
{
  double A[Nraz]; // ������ ���������������� ����������
  double Awn[Nraz]; //������ ��� ���������� ����
  double X[Nraz]; // ������ ��������
  double D[Nraz]; //������������ ������� ������� �������������
  double P[Nraz][Nraz]; // �������������� ������� ������ ����������
  double e_id, b_z, Gamma,P_0,Otklik; //������� �������������, ������ ���������, ���. ����������
  int n_p;  //���������� ���������������� ����������
} TRmnkVar;
int RMNK_s(TRmnkVar &);
int RMNK_sNU(TRmnkVar &);



//---------------------------------------------------------------------------
#endif
 