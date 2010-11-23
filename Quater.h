#include <math.h>
#include <iostream>
using namespace std;

class Quater{

	public:
		Quater(); //two constructors
		Quater(double, double, double, double);
		Quater(const Quater &); //copy constructor


		~Quater(); //destructor

		//Quaternion =R + S i + T j + U k

		double* GetQ() const {return Q;}
		double GetR() const {return *(Q+0);}
		double GetS() const {return *(Q+1);}
		double GetT() const {return *(Q+2);}
		double GetU() const {return *(Q+3);}

		void SetQ(double, double, double, double);
		void SetQ(double, double, double);
		void SetR(double);
		void SetS(double);
		void SetT(double);
		void SetU(double);

		Quater& operator=(const Quater &);
		Quater& operator=(const Quater *);

		Quater operator+(const Quater &);
		Quater operator+(const Quater *);

		Quater operator-(const Quater &);
		Quater operator-(const Quater *);

		Quater operator*(const Quater &);
		Quater operator*(const Quater *);

		Quater operator/(const Quater &);
		Quater operator/(const Quater *);

		Quater operator*(const double);
		Quater operator*(const double*);

		Quater operator/(const double);
		Quater operator/(const double*);



		double GetMod(const Quater &);
		double GetMod(const Quater *);

		void Normalise(const Quater &);
		void Normalise(const Quater *);
	private:
		double *Q;
};
/* ********************************************************** */
Quater::Quater(){
	Q=new double[4];
	*Q=0.0; *(Q+1)=0.0; *(Q+2)=0.0; *(Q+3)=0.0;
}
Quater::Quater(double R, double S, double T, double U){
	*Q=R; *(Q+1)=S; *(Q+2)=T; *(Q+3)=U;
}
Quater::Quater(const Quater &rhs){
	if(!Q)Q=new double[4];
	double* rhsQ;
	rhsQ=rhs.GetQ();
	if(!rhsQ){ 
		cout << "Got NULL in copy constructor" << endl; exit(0);}
	*Q=*rhsQ;
	*(Q+1)=*(rhsQ+1);
	*(Q+2)=*(rhsQ+2);
	*(Q+3)=*(rhsQ+3);
}
Quater::~Quater(){
	delete Q;
	Q=NULL;
}
void Quater::SetQ(double R, double S, double T, double U){
	if(!Q){ 
		cout << "Got NULL in SetQ" << endl; exit(0);}
	*(Q)=R;
	*(Q+1)=S;
	*(Q+2)=T;
	*(Q+3)=U;
}
void Quater::SetQ( double S, double T, double U){
	if(!Q){ 
		cout << "Got NULL in SetQ" << endl; exit(0);}
	*(Q+1)=S;
	*(Q+2)=T;
	*(Q+3)=U;
}

Quater& Quater::operator=(const Quater& rhs){
	if(!Q)Q=new double[4];
	double* rhsQ;
	rhsQ=rhs.GetQ();
	*Q=*rhsQ;
	*(Q+1)=*(rhsQ+1);
	*(Q+2)=*(rhsQ+2);
	*(Q+3)=*(rhsQ+3);
}
Quater& Quater::operator=(const Quater* rhs){
	if(!Q)Q=new double[4];
	double* rhsQ;
	rhsQ=rhs->GetQ();
	*Q=*rhsQ;
	*(Q+1)=*(rhsQ+1);
	*(Q+2)=*(rhsQ+2);
	*(Q+3)=*(rhsQ+3);
}
double GetMod(const Quater &rhs) {
	double* Q;
	Q=rhs.GetQ();
	double R, S, T, U;
	R=*(Q);
	S=*(Q+1);
	T=*(Q+2);
	U=*(Q+4);

	double modval;

	//avoid R*R+S*S+T*T+U*U -> zero or Inf if all large or small
	double max;
	max=0.0;
	if( fabs(R) > max)max=fabs(R);
	if( fabs(S) > max)max=fabs(S);
	if( fabs(T) > max)max=fabs(T);
	if( fabs(U) > max)max=fabs(U);

	if(max == 0.0)
	{ 
		cout << " Warning, modulus zero" << endl;
		return max;
	}
	double R1, S1, T1, U1;
	R1=R/max;
	S1=S/max;
	T1=T/max;
	U1=U/max;

	modval=max*sqrt( R1*R1+S1*S1+T1*T1+U1*U1 );
	return modval;
}
double GetMod(const Quater *rhs) {
	double* Q;
	Q=rhs->GetQ();
	double R, S, T, U;
	R=*(Q);
	S=*(Q+1);
	T=*(Q+2);
	U=*(Q+4);

	double modval;

	//avoid R*R+S*S+T*T+U*U -> zero or Inf if all large or small
	double max;
	max=0.0;
	if( fabs(R) > max)max=fabs(R);
	if( fabs(S) > max)max=fabs(S);
	if( fabs(T) > max)max=fabs(T);
	if( fabs(U) > max)max=fabs(U);

	if(max == 0.0)
	{ 
		cout << " Warning, modulus zero" << endl;
		return max;
	}
	double R1, S1, T1, U1;
	R1=R/max;
	S1=S/max;
	T1=T/max;
	U1=U/max;

	modval=max*sqrt( R1*R1+S1*S1+T1*T1+U1*U1 );
	return modval;
}
void Normalise(const Quater &Quaternion){
	double R, S, T, U;
    double *Q;
    Q=Quaternion.GetQ();
    R=*(Q);
    S=*(Q+1);
    T=*(Q+2);
    U=*(Q+3);

    double modval;

    //avoid R*R+S*S+T*T+U*U -> zero or Inf if all large or small
 double max;
 max=0.0;
 if( fabs(R) > max)max=fabs(R);
 if( fabs(S) > max)max=fabs(S);
 if( fabs(T) > max)max=fabs(T);
 if( fabs(U) > max)max=fabs(U);

if(max == 0.0)
   { 
   cout << " Divide by zero in Norm Quaternion" << endl;
   exit(1);
    }
 double R1,S1,T1,U1;
 R1=R/max;
 S1=S/max;
 T1=T/max;
 U1=U/max;

 modval=max*sqrt(  R1*R1+S1*S1+T1*T1+U1*U1 );

 R=R/modval;
 S=T/modval;
 R=S/modval;
 U=U/modval;
}
void Normalise(const Quater *Quaternion){
    double R, S, T, U;
    double *Q;
    Q=Quaternion->GetQ();
    R=*(Q);
    S=*(Q+1);
    T=*(Q+2);
    U=*(Q+3);

    double modval;

    //avoid R*R+S*S+T*T+U*U -> zero or Inf if all large or small
 double max;
 max=0.0;
 if( fabs(R) > max)max=fabs(R);
 if( fabs(S) > max)max=fabs(S);
 if( fabs(T) > max)max=fabs(T);
 if( fabs(U) > max)max=fabs(U);

if(max == 0.0)
   { 
   cout << " Divide by zero in Norm Quaternion" << endl;
   exit(1);
    }
 double R1,S1,T1,U1;
 R1=R/max;
 S1=S/max;
 T1=T/max;
 U1=U/max;

 modval=max*sqrt(  R1*R1+S1*S1+T1*T1+U1*U1 );

 R=R/modval;
 S=T/modval;
 R=S/modval;
 U=U/modval;
}
Quater Quater::operator+(const Quater &rhs){
       double* rhsQ;
       rhsQ=rhs.GetQ();

       Quater Temp;
       double *TempQ;
       TempQ=Temp.GetQ();
       
       for(int i=0; i <4; i++){
        *(TempQ+i)=*(Q+i)+*(rhsQ+i);
       }
       return Temp;
}
Quater Quater::operator+(const Quater *rhs){
       double* rhsQ;
       rhsQ=rhs->GetQ();

       Quater Temp;
       double *TempQ;
       TempQ=Temp.GetQ();

       for(int i=0; i <4; i++){
        *(TempQ+i)=*(Q+i)+*(rhsQ+i);
       }
       return Temp;
}
Quater Quater::operator-(const Quater &rhs){
       double* rhsQ;
       rhsQ=rhs.GetQ();

       Quater Temp;
       double *TempQ;
       TempQ=Temp.GetQ();

       for(int i=0; i <4; i++){
        *(TempQ+i)=*(Q+i)-*(rhsQ+i);
       }
       return Temp;
}
Quater Quater::operator-(const Quater *rhs){
       double* rhsQ;
       rhsQ=rhs->GetQ();

       Quater Temp;
       double *TempQ;
       TempQ=Temp.GetQ();

       for(int i=0; i <4; i++){
        *(TempQ+i)=*(Q+i)-*(rhsQ+i);
       }
       return Temp;
}
Quater Quater::operator*(const Quater &rhs){
     double A, B, C, D;
     double R, S, T, U;

     double *rhsQ;
     rhsQ=rhs.GetQ();

     Quater Temp;
     double *TempQ;
     TempQ=Temp.GetQ();

     A=*(Q);
     B=*(Q+1);
     C=*(Q+2);
     D=*(Q+3);

     R=*(rhsQ);
     S=*(rhsQ+1);
     T=*(rhsQ+2);
     U=*(rhsQ+3);

     //(A + Bi + Cj + Dk)(R + Si +Tj+Uk)
     //ij=k, jk=i; ki=j
     //ij=-ji, kj=-jk, ik=-ki;
     //i^2=j^2=k^2=-1
     //so, ijk=-1 

     *TempQ=A*R-B*S-C*T-D*U;
     *(TempQ+1)=A*S+B*R+C*U-D*T;
     *(TempQ+2)=A*T+C*R-B*U+D*S;
     *(TempQ+3)=A*U+D*R+B*T-C*S;

     return Temp;
}

Quater Quater::operator*(const Quater *rhs){
     double A, B, C, D;
     double R, S, T, U;

     double *rhsQ;
     rhsQ=rhs->GetQ();

     Quater Temp;
     double *TempQ;
     TempQ=Temp.GetQ();

     A=*(Q);
     B=*(Q+1);
     C=*(Q+2);
     D=*(Q+3);

     R=*(rhsQ);
     S=*(rhsQ+1);
     T=*(rhsQ+2);
     U=*(rhsQ+3);

     //(A + Bi + Cj + Dk)(R + Si +Tj+Uk)
     //ij=k, jk=i; ki=j
     //ij=-ji, kj=-jk, ik=-ki;
     //i^2=j^2=k^2=-1
     //so, ijk=-1 

     *TempQ=A*R-B*S-C*T-D*U;
     *(TempQ+1)=A*S+B*R+C*U-D*T;
     *(TempQ+2)=A*T+C*R-B*U+D*S;
     *(TempQ+3)=A*U+D*R+B*T-C*S;

     return Temp;
}
Quater Quater::operator/(const Quater *rhs){
     double A, B, C, D;
     double R, S, T, U;

     double *rhsQ;
     rhsQ=rhs->GetQ();
     A=*(Q);
     B=*(Q+1);
     C=*(Q+2);
     D=*(Q+3);

     R=*(rhsQ);
     S=*(rhsQ+1);
     T=*(rhsQ+2);
     U=*(rhsQ+3);

     R=-R; S=-S; T=-T; U=-U;

     double ModSq;

	ModSq=R*R+S*S+T*T+U*U; 
	if(ModSq==0.0){
		cout <<"Quaternion divide by zero"<< endl; exit(0);}


     Quater Temp;
     double *TempQ;
     TempQ=Temp.GetQ();

     //(A + Bi + Cj + Dk)(R + Si +Tj+Uk)
     //ij=k, jk=i; ki=j
     //ij=-ji, kj=-jk, ik=-ki;
     //i^2=j^2=k^2=-1
     //so, ijk=-1 
     
     *TempQ=(A*R-B*S-C*T-D*U)/ModSq;
     *(TempQ+1)=(A*S+B*R+C*U-D*T)/ModSq;
     *(TempQ+2)=(A*T+C*R-B*U+D*S)/ModSq;
     *(TempQ+3)=(A*U+D*R+B*T-C*S)/ModSq;

     return Temp;
     }
Quater Quater::operator/(const Quater &rhs){
     double A, B, C, D;
     double R, S, T, U;

     double *rhsQ;
     rhsQ=rhs.GetQ();

     Quater Temp;
     double *TempQ;
     TempQ=Temp.GetQ();

     A=*(Q);
     B=*(Q+1);
     C=*(Q+2);
     D=*(Q+3);

     R=*(rhsQ);
     S=*(rhsQ+1);
     T=*(rhsQ+2);
     U=*(rhsQ+3);

     R=-R; S=-S; T=-T; U=-U;

     double ModSq;

	ModSq=R*R+S*S+T*T+U*U; 
	if(ModSq==0.0){
		cout <<"Quaternion divide by zero"<< endl; exit(0);}


     //(A + Bi + Cj + Dk)(R + Si +Tj+Uk)
     //ij=k, jk=i; ki=j
     //ij=-ji, kj=-jk, ik=-ki;
     //i^2=j^2=k^2=-1
     //so, ijk=-1 
     

     *TempQ=(A*R-B*S-C*T-D*U)/ModSq;
     *(TempQ+1)=(A*S+B*R+C*U-D*T)/ModSq;
     *(TempQ+2)=(A*T+C*R-B*U+D*S)/ModSq;
     *(TempQ+3)=(A*U+D*R+B*T-C*S)/ModSq;

     return Temp;
}
Quater Quater::operator*(const double rhs){

     double R,S,T,U;

     double x=rhs;

     R=*Q;
     R=*(Q+1);
     S=*(Q+2);
     T=*(Q+3);

     R=x*R; S=x*S; T=x*T; U=x*U;

     Quater Temp;
     double *TempQ;
     TempQ=Temp.GetQ();

     *TempQ=R;
     *(TempQ+1)=S;
     *(TempQ+2)=T;
     *(TempQ+3)=U;

     return Temp;
}
Quater Quater::operator*(const double* rhs){

     double R,S,T,U;

     double x=*rhs;

     R=*Q;
     R=*(Q+1);
     S=*(Q+2);
     T=*(Q+3);

     R=x*R; S=x*S; T=x*T; U=x*U;

     Quater Temp;
     double* TempQ;
     TempQ=Temp.GetQ();

     *TempQ=R;
     *(TempQ+1)=S;
     *(TempQ+2)=T;
     *(TempQ+3)=U;

     return Temp;
}
Quater Quater::operator/(const double rhs){

     double R,S,T,U;

     double x=rhs;

     if(x==0.0){cout << "divide by zero in Quaternion /"
	      << endl;  exit(1);}

     R=*Q;
     R=*(Q+1);
     S=*(Q+2);
     T=*(Q+3);

     R=R/x; S=S/x; T=T/x; U=U/x;

     Quater Temp;
     double* TempQ;
     TempQ=Temp.GetQ();

     *TempQ=R;
     *(TempQ+1)=S;
     *(TempQ+2)=T;
     *(TempQ+3)=U;
     return Temp;
}
Quater Quater::operator/(const double* rhs){

     double R,S,T,U;

     double x=*rhs;
     if(x==0.0){cout << "divide by zero in Quaternion /"
	      << endl;  exit(1);}

     R=*Q;
     R=*(Q+1);
     S=*(Q+2);
     T=*(Q+3);

     R=R/x; S=S/x; T=T/x; U=U/x;

     Quater Temp;
     double* TempQ;
     TempQ=Temp.GetQ();

     *TempQ=R;
     *(TempQ+1)=S;
     *(TempQ+2)=T;
     *(TempQ+3)=U;
     return Temp;
}
