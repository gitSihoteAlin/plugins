//---------------------------------------------------------------------------

#ifndef RMNK_strH
#define RMNK_strH
const int Nraz = 10;
typedef struct TRmnkVar
{
  double A[Nraz]; // Вектор идентифицируемых параметров
  double Awn[Nraz]; //Вектор для внутренних нужд
  double X[Nraz]; // Вектор факторов
  double D[Nraz]; //Диагональная матрица весовых коэффициентов
  double P[Nraz][Nraz]; // Ковариационная матрица ошибок оценивания
  double e_id, b_z, Gamma,P_0,Otklik; //Невязка идентификации, фактор забывания, всп. переменные
  int n_p;  //Количество идентифицируемых параметров
} TRmnkVar;
int RMNK_s(TRmnkVar &);
int RMNK_sNU(TRmnkVar &);



//---------------------------------------------------------------------------
#endif
 