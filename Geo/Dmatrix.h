#include <stdio.h>
#include <iostream>
#include "Dvector.h"
class Dmatrix{

    public:

        Dmatrix();                     //constructor
        Dmatrix( unsigned int &, unsigned int &);         //constructor
        Dmatrix(const Dmatrix &);      //copy constructor

        ~Dmatrix();                    // destructor

        void SetRows(unsigned int &);
        void SetCols(unsigned int &);
        void SetDims(unsigned int &, unsigned  int &);
        void SetMem(unsigned int &, unsigned int &);

        int* GetRows() const;
        int* GetCols() const;
        double** GetM() const;

        double& operator() ( unsigned int &, unsigned int &);
        double operator() (unsigned int &, unsigned int &) const;

        Dmatrix& operator=(const Dmatrix&);

        Dmatrix operator+(const Dmatrix&);
        void operator+=(const Dmatrix&);

        Dmatrix operator-(const Dmatrix&);
        void operator-=(const Dmatrix&);

        Dmatrix operator*(const double &);
        void operator*=(const double &);

        Dmatrix operator/(const double &);
        void operator/=(const double &);

        Dmatrix operator*(const Dmatrix&);

        Dvector operator*(const Dvector&);

        Dvector GetRow(unsigned int &);
        Dvector GetCol(unsigned int &);
        Dvector GetDiag();

        void SetRow(const Dvector &, unsigned int &);
        void SetCol(const Dvector &, unsigned int &);
        void SetDiag(const Dvector &);

        void SwapRows(unsigned int &, unsigned int &);
        void SwapCols(unsigned int &, unsigned int &);


        Dmatrix Transpose(const Dmatrix&);

        
    private:
        int *rows;
        int *cols;
        double** M;
};

Dmatrix::Dmatrix(){

       M=NULL;
       rows=new int;
       cols=new int;
       *rows=0;
       *cols=0;
}

Dmatrix::Dmatrix(unsigned  int &i, unsigned  int &j){
     
       rows=new int;
       cols=new int;
       *rows=i;
       *cols=j;
       M=new double*[i];

       for(int k=0; k< i; k++){
           M[k]=new double[j];
           for(int l=0;l<j;l++)
                *(*(M+k)+l)=0.0;
            }
       
}
Dmatrix::Dmatrix(const Dmatrix& rhs)
{
   rows=new int;
   cols=new int;

   int *rhsrows, *rhscols;
   double** rhsM;

   rhsM=rhs.GetM();
   rhsrows=rhs.GetRows();
   rhscols=rhs.GetCols();

   *rows=*rhsrows;
   *cols=*rhscols;

   M=new double*[*rows];
   for(int k=0; k< *rows ; k++){
        M[k]=new double[*cols];
           for(int l=0;l<*cols;l++)
                *(*(M+k)+l)=0.0;
          }


   for(int i=0; i< *rows; i++){
       for(int j=0; j< *cols; j++)
         *(*(M+i)+j)=*(*(rhsM+i)+j);
      }
}
Dmatrix::~Dmatrix(){

     if(M){
         delete[] M;
         M=NULL;
     }
     // rows and cols are garbage as soon as destructor called
     rows=NULL;
     cols=NULL;
}

void Dmatrix::SetRows( unsigned int &i){
   if(*rows != 0){ printf("SetRows: Array Rows Already Set!\n "); exit(1);}
        *rows=i;
        if(!M){
           M=new double*[i];
           for(int l=0;l<i;l++)
                M[l]=NULL;
           }
             else
                {  printf("SetRows: M  already set!\n"); exit(1);}
             }
void Dmatrix::SetCols( unsigned int &j){
   if(*cols != 0){printf( "SetCols: Array Cols Already Set!\n) ");  exit(1);}
        *cols=j;
        int nrows= *rows;
        int ncols= *cols;
        if(!M){printf( "SetCols: Set rows first!\n"); exit(1);}
        if(!(*M)){
           for(unsigned int i=0;i<nrows;i++){
                 M[i]=new double[ncols];
                 for(int l=0;l<ncols;l++)
                     *(*(M+i)+l)=0.0;
                  }
              }
          else
            {printf("SetCols: Columns already set \n"); exit(1);}
      }

int* Dmatrix::GetRows() const {
     return rows;
}

int* Dmatrix::GetCols() const{
     return cols;
}
double** Dmatrix::GetM() const{
     return M;
}
void  Dmatrix::SetMem(unsigned int &i, unsigned int &j) {

   if(!M){ printf("SetMem: M already set!\n"); exit(1);}
   *rows=i;
   *cols=j;
   
     M=new double*[i];

     for(int k=0; k< j; k++){
         M[k]=new double[j];
          for(int l=0;l<j;l++)
                *(*(M+k)+l)=0.0;
         }
}

void Dmatrix::SetDims( unsigned int &i, unsigned  int &j){
     *rows=i;
     *cols=j;
     if(!M){
     M=new double*[i];

     for(int k=0; k< i; k++){
        M[k]=new double[j];
          for(int l=0;l<j;l++)
                *(*(M+k)+l)=0.0;
         }
     }
     else
      { printf("SetDims: M already set\n");exit(1); }

             }
Dmatrix& Dmatrix::operator=(const Dmatrix& rhs){

      //assumes there are  no matrices with just one row or column

      int* rhsrows=rhs.GetRows();
      int* rhscols=rhs.GetCols();
      if(*rows==0)*rows=*rhsrows;
      if(*cols==0)*cols=*rhscols;
      double** rhsM;
      rhsM=rhs.GetM();

     if(*rows != *rhsrows || *cols != *rhscols){
          printf("Operator =:Rows and Columns do not match!\n");
          exit(1);
      }

     //if memory not yet set
     if(!M){
      M=new double*[*rows];
      for(int k=0; k<*rows; k++)
           M[k]=new double[*cols];
      }

      
      for(int i=0; i< *rows; i++){
        for(int j=0; j < *cols; j++)
           *(*(M+i)+j)=*(*(rhsM+i)+j);
       }
}

double& Dmatrix::operator() (unsigned int &i, unsigned  int &j){
   if(!M){printf("double & operator:(): Mmemory not set!\n");exit(0);}
   if(i >= *rows || j >= *cols){printf( "double& Operator(): Array bound error\n " ); exit(1);}
   return *(*(M+i)+j);
}

double Dmatrix::operator() ( unsigned int &i, unsigned  int &j) const {
   if(i >= *rows || j >= *cols){printf( "double operator: Array bound error\n ");  exit(1);}
   return *(*(M+i)+j);
}

Dmatrix Dmatrix::operator+(const Dmatrix& rhs){

      int* rhsrows=rhs.GetRows();
      int* rhscols=rhs.GetCols();

      double** rhsM;
      rhsM=rhs.GetM();


     if(*rows != *rhsrows || *cols != *rhscols){
          printf("Operator +:Rows and Columns do not match!\n");
          exit(1);
      }
     unsigned int nrows=*rows;
     unsigned int ncols=*cols;

     Dmatrix Temp(nrows, ncols);

     double** TempM;
     TempM=Temp.GetM();
  
      for(unsigned int i=0; i< *rows; i++){
        for(unsigned int j=0; j < *cols; j++)
           *(*(TempM+i)+j)=*(*(M+i)+j) + *(*(rhsM+i)+j);
       } 
  
      return Temp;
}
void Dmatrix::operator+=(const Dmatrix& rhs){


      int* rhsrows=rhs.GetRows();
      int* rhscols=rhs.GetCols();
      double** rhsM;
      rhsM=rhs.GetM();

     if(*rows != *rhsrows || *cols != *rhscols){
          printf("Operator +=: Rows and Columns do not match!\n");
          exit(1);
      }
  
      for(unsigned int i=0; i< *rows; i++){
        for(unsigned int j=0; j < *cols; j++)
           *(*(M+i)+j)=*(*(M+i)+j) + *(*(rhsM+i)+j);
       } 
}
Dmatrix Dmatrix::operator-(const Dmatrix& rhs){


      int* rhsrows=rhs.GetRows();
      int* rhscols=rhs.GetCols();
      double** rhsM;
      rhsM=rhs.GetM();

     if(*rows != *rhsrows || *cols != *rhscols){
          printf("Operator -: Rows and Columns do not match!\n");
          exit(1);
      }
     unsigned int nrows=*rows;
     unsigned int ncols=*cols;
      Dmatrix Temp(nrows,ncols);
      double** TempM;
      TempM=Temp.GetM();
  
      for(unsigned int i=0; i< *rows; i++){
        for(unsigned int j=0; j < *cols; j++)
           *(*(TempM+i)+j)=*(*(M+i)+j) - *(*(rhsM+i)+j);
       } 
  
      return Temp;
}
void Dmatrix::operator-=(const Dmatrix& rhs){

      int* rhsrows=rhs.GetRows();
      int* rhscols=rhs.GetCols();
      double** rhsM;
      rhsM=rhs.GetM();

     if(*rows != *rhsrows || *cols != *rhscols){
          printf("Operator -=: Rows and Columns do not match!\n");
          exit(1);
      }
  
      for(unsigned int i=0; i< *rows; i++){
        for(unsigned int j=0; j < *cols; j++)
           *(*(M+i)+j)=*(*(M+i)+j) - *(*(rhsM+i)+j);
       } 
}

Dmatrix Dmatrix::operator*(const double& rhs){

      unsigned int nrows=*rows;
      unsigned int ncols=*cols;

      Dmatrix Temp(nrows,ncols);
      double** TempM;
      TempM=Temp.GetM();

  
      for(unsigned int i=0; i< *rows; i++){
        for(unsigned int j=0; j < *cols; j++)
           *(*(TempM+i)+j)=*(*(M+i)+j)*rhs;
       } 
    return Temp;
}

void Dmatrix::operator*=(const double& rhs){

  
      for(unsigned int i=0; i< *rows; i++){
        for(unsigned int j=0; j < *cols; j++)
           *(*(M+i)+j)=*(*(M+i)+j)*rhs;
       } 
}
Dmatrix Dmatrix::operator/(const double& rhs){

      unsigned int nrows=*rows;
      unsigned int ncols=*cols;

      Dmatrix Temp(nrows,ncols);
      double** TempM;
      TempM=Temp.GetM();

  
      for(unsigned int i=0; i< *rows; i++){
        for(unsigned int j=0; j < *cols; j++)
           *(*(TempM+i)+j)=*(*(M+i)+j)/rhs;
       } 
    return Temp;
}

void Dmatrix::operator/=(const double& rhs){
  
      for(unsigned int i=0; i< *rows; i++){
        for(unsigned int j=0; j < *cols; j++)
           *(*(M+i)+j)=*(*(M+i)+j)/rhs;
       } 
}

Dmatrix Dmatrix::operator*(const Dmatrix& rhs){

       unsigned int rhsrows;
       unsigned int rhscols;
       unsigned int nrows=*rows;
       unsigned int ncols=*cols;

       rhsrows=*rhs.GetRows();
       rhscols=*rhs.GetCols();

       double** rhsM;
       rhsM=rhs.GetM();

       if(nrows != rhscols || ncols != rhsrows)
           {printf("Operator *(Dmatrix): Mismatch for rows and columns!\n"); exit(1);}

       Dmatrix Temp(nrows,rhscols);
       double** TempM=Temp.GetM();

       double lhsval,rhsval,Tempval;

       for(unsigned int i=0; i< nrows; i++)
            for(unsigned int j=0; j< rhscols; j++){
               Tempval=0.0;
               for(unsigned int k=0; k< rhscols; k++){
                   lhsval=*(*(M+i)+k);
                   rhsval=*(*(rhsM+k)+j);
                   Tempval=Tempval+lhsval*rhsval;
                   }
                   *(*(TempM+i)+j)=Tempval;
            }
      return Temp;

}
Dvector Dmatrix::operator*(const Dvector& rhs){

       unsigned int rhslength;
       unsigned int nrows=*rows;
       unsigned int ncols=*cols;

       rhslength=*rhs.GetLength();

       double* rhsV;
       rhsV=rhs.GetV();

       if( ncols != rhslength)
           {printf(" Vector operator: Mismatch!\n"); exit(1);}

       Dvector Temp(nrows);
       double* TempV=Temp.GetV();

       double lhsval,rhsval,Tempval;

       for(unsigned int i=0; i< nrows; i++){
            Tempval=0.0;
            for(unsigned int j=0; j< ncols; j++){
                   lhsval=*(*(M+i)+j);
                   rhsval=*(rhsV+j);
                   Tempval=Tempval+lhsval*rhsval;
                   }
                   *(TempV+i)=Tempval;
            }
      return Temp;

}
Dvector Dmatrix::GetRow(unsigned int &irow){

    unsigned int nrows, ncols;

    nrows=*rows;
    ncols=*cols;

    if(irow > nrows){printf("GetRow: row does not exist\n"),  exit(1);}

    Dvector Temp(ncols);

    for(unsigned int i=0; i< ncols; i++)
       Temp(i)=*(*(M+irow)+i);

    return Temp;

}
Dvector Dmatrix::GetCol(unsigned int &icol){

    unsigned int nrows, ncols;

    nrows=*rows;
    ncols=*cols;

    if(icol > ncols){printf("column  does not exist\n"),  exit(1);}

    Dvector Temp(nrows);

    for(unsigned int i=0; i< nrows; i++)
       Temp(i)=*(*(M+i)+icol);

    return Temp;

}
Dvector Dmatrix::GetDiag(){

    unsigned int nrows, ncols;

    nrows=*rows;
    ncols=*cols;

    // get M(i,i) and store in vector even if M is not diagonal

    unsigned int ivec=nrows;
    if(ivec > ncols)ivec=ncols;

    Dvector Temp(ivec);

    for(unsigned int i=0; i< ivec; i++)
       Temp(i)=*(*(M+i)+i);

    return Temp;

}

void Dmatrix::SetRow(const Dvector& Set, unsigned int &irow){

    unsigned int nrows, ncols;

    nrows=*rows;
    ncols=*cols;

    if(irow > nrows){printf("SetRow: row does not exist\n"),  exit(1);}


    for(unsigned int i=0; i< ncols; i++)
       *(*(M+irow)+i)=Set(i);
}

void Dmatrix::SetCol(const Dvector& Set, unsigned int &icol){

    unsigned int nrows, ncols;

    nrows=*rows;
    ncols=*cols;

    if(icol > ncols){printf("SetCol: column  does not exist\n"),  exit(1);}


    for(unsigned int i=0; i< nrows; i++)
       *(*(M+i)+icol)=Set(i);

}
void Dmatrix::SetDiag(const Dvector& Set){

    unsigned int nrows, ncols;

    nrows=*rows;
    ncols=*cols;

    int ivec=nrows;
    if(ivec > ncols)ivec=ncols;

    int setlength=*Set.GetLength();

    if(setlength != ivec){printf("SetDiag:  Mismatch!\n"); exit(1);}

    for(unsigned int i=0; i< nrows; i++)
       *(*(M+i)+i)=Set(i);

}

void Dmatrix::SwapRows(unsigned int &i, unsigned int &j){

     Dvector Row1, Row2;

     Row1=this->GetRow(i);
     Row2=this->GetRow(j);

     this->SetRow(Row2, i);
     this->SetRow(Row1, j);

}

void Dmatrix::SwapCols(unsigned int &i, unsigned int &j){

     Dvector Col1, Col2;

     Col1=this->GetCol(i);
     Col2=this->GetCol(j);

     this->SetCol(Col2, i);
     this->SetCol(Col1, j);

}

Dmatrix Dmatrix::Transpose(const Dmatrix &A){


     unsigned int nrows;
     unsigned int ncols;
 
     nrows=*(A.GetRows());
     ncols=*(A.GetCols());

     Dmatrix Temp(ncols,nrows);

     for(unsigned int i=0;i<nrows;i++)
        for(unsigned int j=0;j<nrows;j++)
          Temp(j,i)=A(i,j);

     return Temp;
}










  


  


  


  
