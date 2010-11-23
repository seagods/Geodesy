#include <iostream>
#include "Dmatrix.h"

using namespace std;

int main(){


   int *ptrRows,*ptrCols;
   double** ptrM;

   unsigned int nrowsA, ncolsA;
   unsigned int nrowsB, ncolsB;
   nrowsA=3,ncolsA=4;
   nrowsB=4,ncolsB=3;

   Dmatrix B(nrowsB,ncolsB);  //creates 4 by 3 on the heap.
                              // nrows and cols MUST be unsigned int
   Dmatrix A;  //constructor with no arguments creates has M pointing to NULL
              // rows=ncols = 0;
   A.SetMem(nrowsA,ncolsA);  //Alocates Memory-only use if constructor
                             //had no arguments
                             // No data set
                       


   unsigned int i=0,j=0;
   for(unsigned int i=0; i<nrowsA; i++){
       for(unsigned int j=0; j<ncolsA; j++){
           A(i,j)=(double)(i*ncolsA+j);
           }
    }

   Dmatrix C;

   C=A;     //overloaded equals creates memory if not there

   for(unsigned int i=0; i<nrowsB; i++){
       for(unsigned int j=0; j<ncolsB; j++){
           B(i,j)=(double)(i*ncolsB+j);
           }
    }

   Dmatrix D(B);    // make matrix D a deep copy of B

   Dmatrix E(nrowsA,ncolsA);

   E=C+A;
   E=E+A;
   E+=A;
   E=E-A;
   E-=A;

   double mult=10;
   double div=5;

   E=A*mult;
   E*=mult;

   E=A/div;
   E/=div;

   for(unsigned int i=0; i<nrowsA; i++){
       for(unsigned int j=0; j<ncolsA; j++)
	   cout << "E_(i,j)=" << E(i,j) << endl;
    }

   for(unsigned int i=0; i<nrowsA; i++)
       for(unsigned int j=0; j<ncolsA; j++)
	   cout << "A " << i << "  " << j << " = "<< A(i,j) << endl;
   for(unsigned int i=0; i<nrowsB; i++)
       for(unsigned int j=0; j<ncolsB; j++)
	   cout << "B " << i << "  " << j << " = "<< B(i,j) << endl;

    Dmatrix AB(nrowsA,nrowsA);


    AB=A*B;

    for(unsigned int i=0; i<nrowsA; i++)
       for(unsigned int j=0; j<nrowsA; j++)
	   cout << "AB " << i << "  " << j << " = "<< AB(i,j) << endl;

    unsigned int lengthA=4;
    Dvector Avec(lengthA);

    for(unsigned int i=0;i<lengthA;i++)
         Avec(i)=7*(i+1);


   Dvector Bvec;
   unsigned int lengthB;
   lengthB=*A.GetRows();
   Bvec.SetLength(lengthB);


   Bvec=A*Avec;
   for(unsigned int i=0;i<lengthB;i++)
         cout << "Bvec i=" << i << "  Val=" << Bvec(i) << endl;


   Dvector Col;
   Dvector Row;
   Row.SetLength(ncolsA);
   Col.SetLength(nrowsA);

   for(unsigned int i=0; i< ncolsA; i++)
        Row(i)=-(double)(i+20);

   for(unsigned int i=0; i< nrowsA; i++)
        Col(i)=-(double)(i+120);

   unsigned int ir,ic;
   ir=1;
   ic=2;
  // A.SetRow(Row, ir);
  // A.SetCol(Col, ic);


   unsigned int ir1,ir2;
   unsigned int ic1,ic2;

   ir1=1; ir2=2;
   A.SwapCols(ir1,ir2);

   for(unsigned int i=0; i<nrowsA; i++)
      for(unsigned int j=0; j<ncolsA; j++)
        cout << "A " << i << "  " << j << " = "<< A(i,j) << endl;


   return 0;

}
