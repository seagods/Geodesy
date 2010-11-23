#include "Quater.h"

int main(){

	double a, b, c, d;
	double e, f, g, h;

	double x;

	Quater ONE;
	a=1.0; b=0.0; c=0.0; d=0.0;
	ONE.SetQ(a, b, c, d);

	Quater A, B, C;

	a=1.0; b=2.0, c=3.0; d=4.0;

	e=5.0; f=6.0; g=7.0; h=8.0;

	A.SetQ(a, b, c, d);
	B.SetQ(e, f, g, h);

	C=A+B;

	cout << C.GetR() 
		<<  "   " <<
		C.GetS()
		<<  "   " <<
                C.GetT()
		<<  "   " <<
                C.GetU()  << endl;

	return 0;

}
