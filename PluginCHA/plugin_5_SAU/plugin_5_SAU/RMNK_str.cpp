//---------------------------------------------------------------------------


#pragma hdrstop

#include "RMNK_str.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//  ����pp������ �HK � �����p�� ���������}
//  a(i) = a(i-1) + R(i)e(i);
//  R(i) = P~(i-1)x(i)/(1+x(i)'P~(i-1)x(i));
//  P(i-1)~=P(i-1)/b_���;
//  P(i) = P~(i-1)-R(i)x(i)'P~(i-1)}

int RMNK_s(TRmnkVar & Rmnk)
{
   double  X[Nraz], MAS1[Nraz],MAS4[Nraz],MAS2[Nraz][Nraz],MAS3[Nraz][Nraz];
   int     i,j,l;
   double  var1 = 1.0;
   double b_z = (Rmnk.b_z > 0.0 && Rmnk.b_z <= 1.0) ? 1.0/Rmnk.b_z: 1.0;
   if (Rmnk.P_0  < 0.0) Rmnk.P_0 = 0.1;
   //���������� ������� �������� � ������ ������� �������������
   for (i=0; i <=Rmnk.n_p; i++) X[i] = Rmnk.D[i]*Rmnk.X[i];
   //���������� ������� �������������
   Rmnk.e_id = Rmnk.Otklik;
   for (i=0; i <=Rmnk.n_p; i++) Rmnk.e_id -= Rmnk.Awn[i]*X[i];
   // ---------------��p��p������ ���p��� P~(i-1)-------------
    /*��� ������� ������� �� ������ ��������� �������������� �������� ��
     ������������ ���������. ��� ���������� ������-���� ������������� ���-
     ����� �������� �_0 (���. �������� ����. ��������) ������ � �������,
     � ������� ���-�� ���� �������, �� ������� �� ������ ��������� �� ���
     ���, ���� ����. ������� �� ������ ������ �_0. ��� ������� ��� ��������������� �����������
     ����� ������� P*/
      for (i=0; i <=Rmnk.n_p; i++)  MAS1[i] =  (Rmnk.P[i][i] < Rmnk.P_0) ? 0.0 : 1.0;
      for (i=0; i <=Rmnk.n_p; i++)
      {
         for (j=0; j <=Rmnk.n_p; j++)
         {
            if (MAS1[i] == 0 && MAS1[j]== 0)  Rmnk.P[i][j] *= b_z;
         }
      }
    // --------------- ��p��p������ ���p��� R(i) ---------------- }
      for (j=0; j <=Rmnk.n_p; j++)
      {
        MAS1[j]=0.0;
        for (l=0; l <=Rmnk.n_p; l++)  MAS1[j]+= Rmnk.P[j][l]*X[l];  //P~(i-1)*Xi
      }
      for (j=0; j <=Rmnk.n_p; j++) var1 += X[j]*MAS1[j];           //Xi'*P~(i-1)*Xi+1
      for (i=0; i <=Rmnk.n_p; i++) MAS1[i] = MAS1[i]/var1;
      //�p���p�� �������: 0 < x(i)'P~(i-1)x(i)/(1+x(i)'P~(i-1)x(i)) < 1
      var1 = (var1-1)/var1;
      Rmnk.Gamma=var1;
      if (var1 < 0.0 || var1 >=1.0)   //��������� ������� �(i-1) ��� ��������� ������������� ��������������
      {
         for (i=0; i <=Rmnk.n_p; i++)
         {
           for (j=0; j <=Rmnk.n_p; j++) Rmnk.P[i][j] = (i == j) ? Rmnk.P_0 : 0.0; // ���� �������� �(i-1) �� ������������ �������, ����� ����� ��������� ����� �������� ���������
         }
      }
   //----------------��p��p������ ������ ��p����p��-----------------
      for (j=0; j <=Rmnk.n_p; j++) Rmnk.Awn[j] += MAS1[j]*Rmnk.e_id;
      //������ ����������
      for (j=0; j <=Rmnk.n_p; j++) Rmnk.A[j] = Rmnk.Awn[j]*Rmnk.D[j];
   //-----------------��p��p������ ���p��� P(i) ----------------------
      for (j=0; j <=Rmnk.n_p; j++)
      {
         for (l=0; l <=Rmnk.n_p; l++) MAS2[j][l] = MAS1[j]*X[l];  //R(i)*X(i)~
      }
      for (i=0; i <=Rmnk.n_p; i++)
      {
        for (j=0; j <=Rmnk.n_p; j++)
        {
          MAS3[i][j]=0.0;
          for (l=0; l <=Rmnk.n_p; l++) MAS3[i][j] +=  MAS2[i][l]*Rmnk.P[l][j];//R(i)*X(i)~*P(i-1)
        }
      }
      for (i=0; i <=Rmnk.n_p; i++)
      {
         for (j=0; j <=Rmnk.n_p; j++) Rmnk.P[i][j]-= MAS3[i][j]; //P(i)
      }
      for (i=0; i <=Rmnk.n_p; i++)  //�������������� ���� ��� ����������� ������� �������������� ������� P
      {
         if (Rmnk.P[i][i] < 0.0) Rmnk.P[i][i]  = -Rmnk.P[i][i];
         for (j = i+1; j <= Rmnk.n_p; j++)  Rmnk.P[j][i] = Rmnk.P[i][j];
      }
  return 1;
}


int RMNK_sNU(TRmnkVar & Rmnk)
{
 for (int i=0; i <=Rmnk.n_p; i++)
         {
           for (int j=0; j <=Rmnk.n_p; j++) Rmnk.P[i][j] = (i == j) ? Rmnk.P_0 : 0.0; // ���� �������� �(i-1) �� ������������ �������, ����� ����� ��������� ����� �������� ���������
           Rmnk.A[i] = Rmnk.Awn[i] = 0.0;
         }


 return 1;
}
