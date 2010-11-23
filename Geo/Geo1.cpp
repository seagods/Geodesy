#include "Geo.h"
#include "Dmatrix.h"
#include <fstream>
#define GeoPos 10000

//  We shall be using OpenGL's Quadric function
//  GLU_FILL means that it won't be wire frame
//  GLU_LINE means it will be wire frame
//  int g_RenderMode=GLU_FILL; // this is for quadrics only
//

int g_RenderMode=GLU_LINE;

int nv,npoly;

const double pi=acos(-1.0);

Triangle *Triangles;
D3Dvec *NodeV;

D3Dvec Geodesy[GeoPos];
int Geokount;

int ntri, ntrinew, nnodenew;
    int istart,istop, isplit;

double xmin,xmax,ymin,ymax,zmin,zmax;
bool   xallpos,xallneg;
bool   yallpos,yallneg;
bool   zallpos,zallneg;

int iplot;

double xrange,yrange,zrange;
double xrangenorm, yrangenorm,zrangenorm;
int   normx,normy,normz;


char *cstrval=(char*)malloc(50);;
//cstrval=(char*)malloc(50);

const int MAX_TEXTURES=300;
unsigned int gl_Texture[MAX_TEXTURES];

/*  FONTS IN
HAVE TRUETYPE FONTS IN
1.  /usr/share/tuxpaint/fonts
2,  /usr/lib/SunJava2-1.4.2/jre/lib/fonts
3.  /usr/X11R6/lib/X11/fonts/truetype
4   plus a few others from games and povray
*/

int width=1024;
int height=768;
const double speed=50;
const double acceleration=1;
const double convert=180.0/pi;
const double twopi=2.0*pi;
// instantaneous rotation angles
double angx=0.0, angy=0.0;
// cumulative versions
double angxcum=0.0, angycum=0.0;

bool upPressed=false;
bool downPressed=false;
bool leftPressed=false;
bool ctrl_leftPressed=false;
bool shift_leftPressed=false;
bool rightPressed=false;
bool ctrl_rightPressed=false;
bool shift_rightPressed=false;
bool p_Pressed=false;
bool MouseOn=true;
bool togglez=false;



GLUquadric *QuadSphere;

void Init(){
   InitialiseGL(SCREEN_WIDTH, SCREEN_HEIGHT);
  // CreateTexture(gl_Texture,"Earth2.bmp",0);

   //lighting  r,b,g,alpha (alpha is for blending)
   glClearColor(1.,1.,1.,1.); //background colour white
//   glClearColor(0.,0.,0.,1.); //background colour black

     std::cout << " Max Number of Lights=" << GL_MAX_LIGHTS << endl;

     //To specifiy a light we need to name them as follows
     // from GL_LIGHT0, GL_LIGHT1,GL_LIGHT2,... GL_LIGHTN.

     /* Second Argument can be
       GL_AMBIENT, GL_DIFFUSE,GL_SPECULAR,GL_POSITION,
    GL_SPOT_CUTTOFF, GL_SPOT_DIRECTION, GL_SPOT_EXPONENT,
    GL_CONSTANT_ATTENIATION, GL_LINEAR_ATTENUATION,
    GL_QUADRATIC_ATTENUATION   */
    //
    //colour of the ambient and diffuse light
     float ambient[4]={0.7, 0.7, 0.7, 0.2}; // light from all over
     float diffuse[4]={1.0, 1.0, 1.0, 1.0}; // light component reflecting diffusely
     float specular[4]={1.0, 1.0, 1.0, 1.0}; //light reflecting specularly

     glLightfv(GL_LIGHT0,GL_AMBIENT, ambient);
     glLightfv(GL_LIGHT0,GL_DIFFUSE, diffuse);
     glLightfv(GL_LIGHT0,GL_SPECULAR, specular);

     float gLightPosition[4]={-1000.,-1000.,-1000.,0.0};
  //   float gLightPosition[4]={10000.,0.,0., 1.0};
     //The last number  tells OpenGL that it's
     // a light position if it is non zero. 
     // The first three numbers are the coordinates. If this last entry is zero
     // OpenGL will treat it as a direction, and will work out
     // direction cosines from the first three numbers. 

     glLightfv(GL_LIGHT0,GL_POSITION,gLightPosition);

     glEnable( GL_LIGHT0 );

     glEnable( GL_LIGHTING);

      QuadSphere=gluNewQuadric();

}
void EventLoop(CCam & Camera1)
{
 bool quitit=false;
 SDL_Event event;
 SDL_MouseMotionEvent event2;
 SDL_keysym  *whatkey;
 bool mousedown=false;

int n_nodes;
bool old1,old2,old3;
bool case0, case1, case2, case3, case4, case5, case6, case7;

//  Go to while(!quitit) to see what happens once data set up

// Go to Render Scene for graphics bit
//


    bool exitnow=false;

    extern double X,Y,Z;
    X=-1e32;Y=-1e-32;Z=-1e32;
    xmax=X; ymax=Y, zmax=Z;
    xmin=-X; ymin=-Y, zmin=-Z;

    double xunit,yunit,zunit;
    xunit=1.0;yunit=1.0;zunit=1.0;

    // (On ,my machine) very slow  ndiv>9, pages memory
    // On Earth, ndiv=6 is the last stage where unsigned shorts
    // could be used for triangle and node numbers, and the length scale is ~200km.
    int ndiv=4;

    int Ttotal,Ntotal;

    Ttotal=8*(int)(pow(4,ndiv+1)-1)/3; 
    Ntotal=4*(int)(pow(4,ndiv)-1)+6;  

    int trinumbers[ndiv+1];
    int nodenumbers[ndiv+1];

    cout << "There will be " << endl;
    cout <<  Ttotal << "  Triangles in total " << endl;
    cout << 3*pow(4,ndiv)  << "  new nodes  in last subdivision " << endl;
    cout <<  Ntotal << " Nodes  in total "  << endl;

    trinumbers[0]=8;   //initial octahedron
    nodenumbers[0]=6;

    int trinum=8;


    Triangles=(Triangle*) calloc( Ttotal ,sizeof(Triangle));
    NodeV=(D3Dvec*)calloc( Ntotal, sizeof(D3Dvec));

    for(int i=0; i<Ntotal; i++){
	    NodeV[i]=D3Dvec(); } //initialise or bust!
    for(int i=0; i<Ttotal; i++){
	    Triangles[i]=Triangle(); } //initialise


    cout << " Starting Subdivision " << endl;


 //   if_stream opens for input
 //   of_stream opens for output
 //   f_stream opens for both
 //
    double x,y,z;
 // position vectors for basic octahedron 
    x=0.0; y=-1.0; z=0.0;
    NodeV[0].SetVec(x,y,z);
    x=1.0; y=0.0; z=0.0;
    NodeV[1].SetVec(x,y,z);
    x=0.0; y=1.0; z=0.0;
    NodeV[2].SetVec(x,y,z);
    x=-1.0; y=0.0; z=0.0;
    NodeV[3].SetVec(x,y,z);
    x=0.0; y=0.0; z=1.0;
    NodeV[4].SetVec(x,y,z);
    x=0.0; y=0.0; z=-1.0;
    NodeV[5].SetVec(x,y,z);

    int in,jn,kn;
    char quad, otherquad;

 //node numbers for basic octahedron
    quad=1;
    in=1-1;jn=2-1;kn=5-1;
    Triangles[0].SetTri(in, jn, kn);
    Triangles[0].SetQuad(quad);
    quad=2;
    in=2-1;jn=3-1;kn=5-1;
    Triangles[1].SetTri(in, jn, kn);
    Triangles[1].SetQuad(quad);
    quad=3;
    in=3-1;jn=4-1;kn=5-1;
    Triangles[2].SetTri(in, jn, kn);
    Triangles[2].SetQuad(quad);
    quad=4;
    in=4-1;jn=1-1;kn=5-1;
    Triangles[3].SetTri(in, jn, kn);
    Triangles[3].SetQuad(quad);

    quad=1;
    in=2-1;jn=1-1;kn=6-1;
    Triangles[4].SetTri(in, jn, kn);
    Triangles[4].SetQuad(quad);
    quad=2;
    in=3-1;jn=2-1;kn=6-1;
    Triangles[5].SetTri(in, jn, kn);
    Triangles[5].SetQuad(quad);
    quad=3;
    in=4-1;jn=3-1;kn=6-1;
    Triangles[6].SetTri(in, jn, kn);
    Triangles[6].SetQuad(quad);
    quad=4;
    in=1-1;jn=4-1;kn=6-1;
    Triangles[7].SetTri(in, jn, kn);
    Triangles[7].SetQuad(quad);





    //Neighbours for each edge of each triangle
    //Triangle edges in triangle are the edge number
    //of the edge in the NEIGHBOUR
    //
    //For instance Edge 1 of first triangle
    //is edge 1 of triangle 5, but in opposite direction
    in=5-1;jn=4-1;kn=2-1;
    Triangles[0].SetNeighb(in, jn, kn);

    in=6-1;jn=1-1;kn=3-1;
    Triangles[1].SetNeighb(in, jn, kn);

    in=7-1;jn=2-1;kn=4-1;
    Triangles[2].SetNeighb(in, jn, kn);

    in=8-1;jn=3-1;kn=1-1;
    Triangles[3].SetNeighb(in, jn, kn);

    // *********************************************

    in=1-1;jn=6-1;kn=8-1;
    Triangles[4].SetNeighb(in, jn, kn);

    in=2-1;jn=7-1;kn=5-1;
    Triangles[5].SetNeighb(in, jn, kn);

    in=3-1;jn=8-1;kn=6-1;
    Triangles[6].SetNeighb(in, jn, kn);

    in=4-1;jn=5-1;kn=7-1;
    Triangles[7].SetNeighb(in, jn, kn);

    ntri=8; n_nodes=6; ntrinew=0;   //we have 8 triangles and 6 nodes

    n_nodes=n_nodes-1;  // 6 nodes 0-5

    int *pSplit0,*pSplit1,*pSplit2,*pSplit3;
    bool split_this;

    int in1,in2,in3;
    int is1,is2;
    int nn1,nn2,nn3;

    int inode1,inode2,inode3;
    int new0,new1,new2,new3;  //serves for new node or triangle numbers
    int new1N,new2N,new3N;  

    istart=0;
    istop=8;

      // split all triangles at this level if the neighbours 
      // are on the same level. Then every split triangle
      // has an unsplit neighbour and vice versa
      //
	    //recursive subdivision
	    //
	    for(int idiv=0; idiv <ndiv; idiv++){
		 ntrinew=0;
		 nnodenew=0;
	    for(int itri=istart;itri<istop;itri++){
		    if( Triangles[itri].GetN() ){
		    in1=Triangles[itri].GetN1();
		    in2=Triangles[itri].GetN2();
		    in3=Triangles[itri].GetN3();
		    }
		    else { cout <<" No Neighbours " << endl;  exit(0);  }

		    pSplit0=Triangles[itri].GetS();
		    pSplit1=Triangles[in1].GetS();
		    pSplit2=Triangles[in2].GetS();
		    pSplit3=Triangles[in3].GetS();

                    if(!pSplit0){split_this=true;} else {split_this=false;}

		    if(split_this){

		    // Split This Triangle into 4 with three new nodes

		    bool NodeExists1=false;
		    bool NodeExists2=false;
		    bool NodeExists3=false;

		    if(pSplit3)NodeExists1=true;
		    if(pSplit2)NodeExists2=true;
		    if(pSplit1)NodeExists3=true;

		    int oldnode1=-1,oldnode2=-1,oldnode3=-1;

                    old1=false; old2=false; old3=false;
		    // nodes of internal triangle to current replacement
		    // in current leve;
		    if(NodeExists1){
			    // split exists on edge 3 of current T
			    quad=Triangles[itri].GetQuad();
			    otherquad=Triangles[in3].GetQuad();
                           if(quad != otherquad){
			    oldnode1=
			    Triangles[Triangles[in3].GetS2()].Get3();}
			   else {
			    oldnode1=
			    Triangles[Triangles[in3].GetS3()].Get3();}
			    old1=true;
		    }
		    if(NodeExists2){
			    // split exists on edge 2 of current T
			    //
			    quad=Triangles[itri].GetQuad();
			    otherquad=Triangles[in2].GetQuad();
			    if(quad != otherquad){
			    oldnode2=
			    Triangles[Triangles[in2].GetS3()].Get3(); }
			    else  {
			    oldnode2=
			    Triangles[Triangles[in2].GetS2()].Get3(); }

			    old2=true;
		    }
		    if(NodeExists3){
			    //  edge 1 always matches edge1
			    oldnode3=
			    Triangles[Triangles[in1].GetS2()].Get2();
			    old3=true;
		    }

		    if(oldnode1 < 0){
		       n_nodes++;
		       nnodenew++;
		       NodeV[n_nodes].SetX( 
				NodeV[ Triangles[itri].Get2() ].GetX()
			       +(
				NodeV[ Triangles[itri].Get3() ].GetX()
				-NodeV[ Triangles[itri].Get2() ].GetX()
				)/2.0
				);

		       NodeV[n_nodes].SetY( 
				NodeV[ Triangles[itri].Get2() ].GetY()
			       +(
				NodeV[ Triangles[itri].Get3() ].GetY()
				-NodeV[ Triangles[itri].Get2() ].GetY()
				)/2.0
				);
		       NodeV[n_nodes].SetZ( 
				NodeV[ Triangles[itri].Get2() ].GetZ()
			       +(
				NodeV[ Triangles[itri].Get3() ].GetZ()
				-NodeV[ Triangles[itri].Get2() ].GetZ()
				)/2.0
				);
		    //  Normalise(NodeV[n_nodes]);
		    }


		    if(oldnode2 < 0){
		       n_nodes++;
		       nnodenew++;
		       NodeV[n_nodes].SetX( 
				NodeV[ Triangles[itri].Get1() ].GetX()
			       +(
				NodeV[ Triangles[itri].Get3() ].GetX()
				-NodeV[ Triangles[itri].Get1() ].GetX()
				)/2.0
				);
		       NodeV[n_nodes].SetY( 
				NodeV[ Triangles[itri].Get1() ].GetY()
			       +(
				NodeV[ Triangles[itri].Get3() ].GetY()
				-NodeV[ Triangles[itri].Get1() ].GetY()
				)/2.0
				);
		       NodeV[n_nodes].SetZ( 
				NodeV[ Triangles[itri].Get1() ].GetZ()
			       +(
				NodeV[ Triangles[itri].Get3() ].GetZ()
				-NodeV[ Triangles[itri].Get1() ].GetZ()
				)/2.0
				);
		   //   Normalise(NodeV[n_nodes]);
		    }
                    if(oldnode3 < 0){
		       n_nodes++;
		       nnodenew++;
		       NodeV[n_nodes].SetX( 
				NodeV[ Triangles[itri].Get1() ].GetX()
			       +(
				NodeV[ Triangles[itri].Get2() ].GetX()
				-NodeV[ Triangles[itri].Get1() ].GetX()
				)/2.0
				);
		       NodeV[n_nodes].SetY( 
				NodeV[ Triangles[itri].Get1() ].GetY()
			       +(
				NodeV[ Triangles[itri].Get2() ].GetY()
				-NodeV[ Triangles[itri].Get1() ].GetY()
				)/2.0
				);
		       NodeV[n_nodes].SetZ( 
				NodeV[ Triangles[itri].Get1() ].GetZ()
			       +(
				NodeV[ Triangles[itri].Get2() ].GetZ()
				-NodeV[ Triangles[itri].Get1() ].GetZ()
				)/2.0
				);
		    //  Normalise(NodeV[n_nodes]);
	    }

		     isplit=istop+ntrinew;

		     new0=isplit;
		     new1=isplit+1;
		     new2=isplit+2;
		     new3=isplit+3;
	    
		     Triangles[itri].SetSplit( new0, new1, new2, new3);


		       if(old1)inode1=oldnode1;
		       if(old2)inode2=oldnode2;
		       if(old3)inode3=oldnode3;
                       case0=false;
                       case1=false;
                       case2=false;
                       case3=false;
                       case4=false;
                       case5=false;
                       case6=false;
                       case7=false;

		       if( !old1 && !old2 && !old3)case0=true; 
		       if( old1 && old2 && old3)case7=true;

		       if(case0 ){
			       inode1=n_nodes-2;
			       inode2=n_nodes-1;
			       inode3=n_nodes-0;
		       }
		       if(case7){
			       inode1=oldnode1;
			       inode2=oldnode2;
			       inode3=oldnode3;
		       }


		       if(!case0 &&  !case7)
		       {
		       if(old1){
			       if(old2 || old3){
			       if(old2){   
				  case4=true;   // nodes 1 and 2 exist 
				  inode1=oldnode1;
				  inode2=oldnode2;
				  inode3=n_nodes-0;
			          }
				  else
				  {
			           case6=true;    // nodes 1 and 3 exist
			           inode1=oldnode1;
			           inode2=n_nodes-0;
			           inode3=oldnode3;
				  }
			       }
				      else 
				      {
					      case1=true;  //only node 1 exists
				              inode1=oldnode1;
				              inode2=n_nodes-1;
				              inode3=n_nodes-0;  
				      }
			         }//endif old1
		       if(old2){
			       if(!old1){   // case 4 done above
			       if(old3){   
				  case5=true;   //nodes 2 and 3 exist
				  inode1=n_nodes-0;
				  inode2=oldnode2;
				  inode3=oldnode3;
			          }
				  else
				  {
				  case2=true;  //only node 2 exists
				  inode1=n_nodes-1;
				  inode2=oldnode2;
				  inode3=n_nodes-0;
				  }
			       }
		       } //endif old2
		       if(old3){
			       if( !old1 && !old2){
			       //  1 and 3 and 2 and 3 done already
			          case3=true; 
				  inode1=n_nodes-1;
				  inode2=n_nodes-0;
				  inode3=oldnode3;
			       }
		       }
		       }  //endif (NOT case 0) AND (NOT case 7)

		       quad=Triangles[itri].GetQuad();
		       Triangles[new0].SetTri(inode1, inode2, inode3);  //Centre T
		       Triangles[new0].SetQuad(quad);

		      Triangles[new1].SetTri(Triangles[itri].Get1(), inode3, inode2); 
		      Triangles[new1].SetQuad(quad);
		      Triangles[new2].SetTri(inode3, Triangles[itri].Get2(), inode1);
		      Triangles[new2].SetQuad(quad);
		      Triangles[new3].SetTri(inode2, inode1, Triangles[itri].Get3());
		      Triangles[new3].SetQuad(quad);

		 
                      //Set Neighbours for centre Triangle;
		       new1N=new3;
		       new2N=new2;
		       new3N=new1;
		       Triangles[new0].SetNeighb(new1N,new2N,new3N);
		       Triangles[new1].SetN3(new0);
		       Triangles[new2].SetN2(new0);
		       Triangles[new3].SetN1(new0);

                       if(pSplit1){
			      // have split neighbours on edge 1
			      // These are
			      is1=Triangles[in1].GetS3();
			      is2=Triangles[in1].GetS2();

			      Triangles[new1].SetN1(is1);
			      Triangles[new2].SetN1(is2);
                               
			      Triangles[is1].SetN1(new1); 
			      Triangles[is2].SetN1(new2); 
			      //independent of quadrant
		       }
                       if(pSplit2){
			      // have split neighbours on edge 2
			      // These are
			      quad=Triangles[itri].GetQuad();
			      otherquad=Triangles[in2].GetQuad();

			      if(quad != otherquad){
			      is1=Triangles[in2].GetS3();
			      is2=Triangles[in2].GetS4();
			      }
			      else {
			      is1=Triangles[in2].GetS4();
			      is2=Triangles[in2].GetS2();
			      }
			      if(quad != otherquad){
			      Triangles[is1].SetN3(new1); 
			      Triangles[is2].SetN3(new3);
			      }
			      else {
			      Triangles[is1].SetN2(new1);
			      Triangles[is2].SetN2(new3);
			      }
			      Triangles[new1].SetN2(is1);
			      Triangles[new3].SetN2(is2);

		       }
                       if(pSplit3){
			      // have split neighbours on edge 3
			      // These are
			      quad=Triangles[itri].GetQuad();
			      otherquad=Triangles[in3].GetQuad();
                              if(quad != otherquad){
			      is1=Triangles[in3].GetS2();
			      is2=Triangles[in3].GetS4(); 
			      }
				      else {
			      is1=Triangles[in3].GetS4();
			      is2=Triangles[in3].GetS3(); 
				      }
                              if(quad != otherquad){
			      Triangles[is1].SetN2(new2);
			      Triangles[is2].SetN2(new3);
			      }
			      else
			      {
			      Triangles[is1].SetN3(new2);
			      Triangles[is2].SetN3(new3);  
			      }

			      Triangles[new2].SetN3(is1);
			      Triangles[new3].SetN3(is2);
		       }
		       
		       
		     ntrinew=ntrinew+4;

		    }  //endif neighbours unsplit
	    }  //end loop over tranche of triangles
	      istart=istop;
	      istop=istop+ntrinew;
	   //   cout << " new istart=" << istart  << endl;
	   //   cout << "new istop=" << istop << endl;

	      trinumbers[idiv+1]=ntrinew;
	      nodenumbers[idiv+1]=n_nodes;

	      trinum=trinum+ntrinew;
	      cout << " There are " << ntrinew << " new Triangles in this subdivision"<<endl;
	      cout << " There are " << trinum  << " Triangles in total "<<endl;
	      cout << " There are " << nnodenew << " new nodes in this subdivision "<< endl;
	      cout << " There are " << n_nodes+1 << " Nodes in total "<<endl;
	      cout << "idiv=" << idiv << endl;


	      int ie1, ie2, ie3,  ie4;
	      //check connections!
	      cout << "ISTART  ISTOP " << istart  << "  " << istop << endl;

	      for(int i=0; i< n_nodes; i++)
                Normalise(NodeV[i]);
				      }  

    cout << " calculate metric" << endl;

    double **Gmetric;
    Gmetric =(double**)calloc( Ttotal, sizeof( double[3] )  );
    if(!Gmetric){ cout << "Memory Failed for Gmetric " << endl; exit(0);}

    // 2D vectors in each triangle
    double **avec, **bvec;
    avec=(double**)calloc( Ttotal, sizeof(double[2]) );
    bvec=(double**)calloc( Ttotal, sizeof(double[2]) );
    if(!avec){ cout << "Memory Failed for avec" << endl; exit(0);}
    if(!bvec){ cout << "Memory Failed for bvec" << endl; exit(0);}
    double avecdash[2], bvecdash[2];
    double avecdash_new[2], bvecdash_new[2];

    D3Dvec Ve1,Ve2,VOrig;  //origin and edge vectors
    Ve1=D3Dvec();  Ve2=D3Dvec(); VOrig=D3Dvec();

    D3Dvec Unit_i, Unit_j, Unit_k;
    Unit_i=D3Dvec(); Unit_j=D3Dvec(); Unit_k=D3Dvec();

    int i1,i2,i3;

    double rad=-1;

    for(int i=Ttotal-ntrinew; i<Ttotal; i++){
    Gmetric[i]=new double[3];
    i1=Triangles[i].Get1();
    i2=Triangles[i].Get2();
    i3=Triangles[i].Get3();
    x=NodeV[i1].GetX();
    y=NodeV[i1].GetY();
    z=NodeV[i1].GetZ();
    VOrig.SetVec(x,y,z);
    x=NodeV[i2].GetX()-VOrig.GetX();
    y=NodeV[i2].GetY()-VOrig.GetY();
    z=NodeV[i2].GetZ()-VOrig.GetZ();
    Ve1.SetVec(x,y,z);
    x=NodeV[i3].GetX()-VOrig.GetX();
    y=NodeV[i3].GetY()-VOrig.GetY();
    z=NodeV[i3].GetZ()-VOrig.GetZ();
    Ve2.SetVec(x,y,z);
    Gmetric[i][0]=Ve1.Dot(Ve1);
    Gmetric[i][1]=Ve1.Dot(Ve2);
    Gmetric[i][2]=Ve2.Dot(Ve2);
    rad=Gmetric[i][0];
    rad=sqrt(rad)*6370.;

     // local  cartesians
     Unit_i=Ve1;
     Normalise(Unit_i);
     Unit_k=Ve1*Ve2;
     Normalise(Unit_k);
     Unit_j=Unit_k*Unit_i;

     avec[i]=new double[2];
     bvec[i]=new double[2];

     avec[i][0]=Unit_i.Dot(Ve1);
     avec[i][1]=0.0;
     bvec[i][0]=Unit_i.Dot(Ve2);
     bvec[i][1]=Unit_j.Dot(Ve2);

    }  

    


    // NodeV[0], NodeV[1], ....  NodeV[n_nodes] is n_nodes+1 nodes
    n_nodes=n_nodes+1;
   
    cout << "There were " << ntrinew << "  new triangles in last subdivision" << endl;

    cout << "There should be " << endl;
    cout <<  Ttotal << "  Triangles in total, there are " << istop << endl;
    cout << 3*(int)pow(4,ndiv)  << "  new nodes  in last subdivision " << endl;
    cout <<  Ntotal << " Nodes  in total, there are " << n_nodes << endl;

    cout << ntrinew
	   << "   triangles, length scales of order " <<  rad << " km for Earth" << endl;
    cout << "They are Triangles T[" << Ttotal-ntrinew  <<"] " <<
     "  to Triangles T[" << Ttotal <<"] " << endl;

    // Now for a geodesic !
    //
    // In this specific example we expect ndiv=4
    if(ndiv==4){
    int Tlow=680;    //triangle number range for ndiv=4
    int Thigh=2728;
    int Tstart0=2122;
    int Tstart, TNext;
    double xO,yO,zO;
    double x1,y1,z1;
    double x2,y2,z2;
    double x3,y3,z3;
    char qStart,qNext;
    double  d1, d2, lambda;
    double d1dash,d2dash;
    double ds_total, ds_squared;
    double alpha_start, beta_start;

    double alphaTstart, betaTstart; 
    // values of lambda to hit the line alpha=0, beta=0, alpha+beta=1.
    double Tol1=0.000001;
    // if fabs(d) less than this it lands on edge miles away or never at all
    double Tol2=1e-8;  //possibility that we land on a corner

    double alpha_e1, beta_e1;
    double alpha_e2, beta_e2;
    double alpha_e3, beta_e3;
    bool land_e1, land_e2, land_e3;
    double lambda_edge1, lambda_edge2, lambda_edge3;

    int Node1, Node2, Node3;

    double alphaNext,betaNext;
    double alpha_a, beta_a, alpha_b, beta_b;
    double alpha_a_dash, beta_a_dash, alpha_b_dash, beta_b_dash;

    double CosTheta,SinTheta,Theta,Phi;

    double adot_adash, adot_bdash, bdot_adash, bdot_bdash;

    double W1,W2,X1,X2,Y1,Y2,Z1,Z2,Det;
    double ex1,why1,ex2,why3;

    unsigned int ndim;

    int kount=0;
                              

    Tstart=Tstart0;
    alpha_start=0.25, beta_start=0.25;
    d1=.5; d2=0.25;  

    while(ds_total < 6.0  && kount < GeoPos){

    Node1=Triangles[Tstart].Get1();
    Node2=Triangles[Tstart].Get2();
    Node3=Triangles[Tstart].Get3();

    D3Dvec Rnode1,Rnode2;
    Rnode1=D3Dvec(); Rnode2=D3Dvec();

    xO=NodeV[Node1].GetX();
    yO=NodeV[Node1].GetY();
    zO=NodeV[Node1].GetZ();

    VOrig.SetVec(xO,yO,zO);

    x1=NodeV[Node2].GetX();
    y1=NodeV[Node2].GetY();
    z1=NodeV[Node2].GetZ();
    Rnode1.SetVec(x1,y1,z1);

    x2=NodeV[Node3].GetX();
    y2=NodeV[Node3].GetY();
    z2=NodeV[Node3].GetZ();
    Rnode2.SetVec(x2,y2,z2);

    Ve1=Rnode1-VOrig;
    Ve2=Rnode2-VOrig;

    x3=VOrig.GetX()+alpha_start*Ve1.GetX()+beta_start*Ve2.GetX();
    y3=VOrig.GetY()+alpha_start*Ve1.GetY()+beta_start*Ve2.GetY();
    z3=VOrig.GetZ()+alpha_start*Ve1.GetZ()+beta_start*Ve2.GetZ();


    Geodesy[kount]=new D3Dvec();
    Geodesy[kount].SetX(x3);
    Geodesy[kount].SetY(y3);
    Geodesy[kount].SetZ(z3);

    cout << Geodesy[kount].GetX() << "  " << Geodesy[kount].GetY()
	    << "  " << Geodesy[kount].GetZ() << "  kount=" << kount <<
	     "   Tstart=" << Tstart << endl;
    kount++;


    land_e1=false; land_e2=false; land_e3=false;

    //alpha_start + d1 lambda=0 lands on edge2;
    //beta_start+ d2 lambda=0 lands on edge1;
     if( fabs(d1) > Tol1){lambda_edge1=-beta_start/d2; land_e1=true;}
     if( fabs(d2) > Tol1){lambda_edge2=-alpha_start/d1; land_e2=true;}
     if( fabs(d2+d2) > Tol1)
     {lambda_edge3=(1.0-alpha_start-beta_start)/(d1+d2); 
	     land_e3=true;}

     cout << "d1=" << d1 << endl;
     cout << "d2=" << d2 << endl;
     cout << "alpha_s=" << alpha_start << endl;
     cout << "beta_s=" << beta_start << endl;

     alpha_e1=alpha_start+lambda_edge1*d1;
     beta_e1=beta_start+lambda_edge1*d2;
     alpha_e2=alpha_start+lambda_edge2*d1;
     beta_e2=beta_start+lambda_edge2*d2;
     alpha_e3=alpha_start+lambda_edge3*d1;
     beta_e3=beta_start+lambda_edge3*d2;

     ndim=2;
     Dmatrix Mat(ndim,ndim);  //creates 4 by 3 on the heap.

     Dvector RHS(ndim);
     Dvector LHS(ndim);


     if(  (fabs(alpha_e1) < Tol2)  && (fabs(beta_e1) <Tol2) ){
		     // set it safely on edge 1
		     alpha_e1=2.*Tol2;
		     beta_e1=0.0;
		     }
     if(  (fabs(1.0-alpha_e1) < Tol2)  && (fabs(beta_e1) <Tol2) ){
		     // set it safely on edge 1
		     alpha_e1=1.0-2.0*Tol2;
		     beta_e1=0.0;
		     }

     if(  (fabs(alpha_e2) < Tol2)  && (fabs(beta_e2) <Tol2) ){
		     // set it safely on edge 2
		     alpha_e2=0.0;
		     beta_e2=2.0*Tol2;
		     }
     if(  (fabs(1.0-beta_e2) < Tol2)  && (fabs(alpha_e2) <Tol2) ){
		     // set it safely on edge 2
		     alpha_e2=0.0;
		     beta_e2=1.0-2.0*Tol2;
		     }

     if(  (fabs(1.0-alpha_e3-beta_e3) < Tol2)  ){
         if(fabs(alpha_e3) < Tol2){
		 // set it safely on edge 3
		 alpha_e3=2.0*Tol2;
		 beta_e3=1.0-alpha_e3;
	 }
         if(fabs(beta_e3) < Tol2){
		 // set it safely on edge 3
		 beta_e3=2.0*Tol2;
		 alpha_e3=1.0-beta_e3;
	 }
     }

   
/*     cout << "e1 " << alpha_e1 << "  L e1=" << lambda_edge1 << endl;
     cout << "e2 " << beta_e2 <<  "  L_e2=" <<  lambda_edge2 << endl;
     cout << "a e3 " << alpha_e3 << "  L_e3=" << lambda_edge3 << endl;
     cout << "b e3 " << beta_e3 << endl;  */

     if(lambda_edge1 <0.0 || alpha_e1 < 0.0 || alpha_e1 > 1.0)land_e1=false;
     if(lambda_edge2 <0.0 || beta_e2 < 0.0 || beta_e2 > 1.0)land_e2=false;

     if(lambda_edge3 <0.0 || alpha_e3 < 0.0 || alpha_e3 > 1.0)land_e3=false;
     if(lambda_edge3 <0.0 || beta_e3 < 0.0 || beta_e3 > 1.0)land_e3=false;

     cout << land_e1 << land_e2 << land_e3 << " edges 1 2 and 3" << endl;

     if(land_e1){
	     //beta_e1=0
	     ds_squared=
	     Gmetric[Tstart][0]*(alpha_e1-alpha_start)*(alpha_e1-alpha_start)
	     -2.0*Gmetric[Tstart][1]*(alpha_e1-alpha_start)*beta_start
	     +Gmetric[Tstart][2]*beta_start*beta_start;

	     ds_total=ds_total+sqrt(ds_squared);
     }
     if(land_e2){
	     //alpha_e1=0
	     ds_squared=
	     Gmetric[Tstart][0]*alpha_start*alpha_start
	     -2.0*Gmetric[Tstart][1]*alpha_start*(beta_e2-beta_start)
	     +Gmetric[Tstart][2]*(beta_e2-beta_start)*(beta_e2-beta_start);

	     ds_total=ds_total+sqrt(ds_squared);
     }
     if(land_e3){
	     ds_squared=
	     Gmetric[Tstart][0]*(alpha_e3-alpha_start)*(alpha_e3-alpha_start)
	     +2.0*Gmetric[Tstart][1]*(alpha_e3-alpha_start)*(beta_e3-beta_start)
	     +Gmetric[Tstart][2]*(beta_e3-beta_start)*(beta_e3-beta_start);

	     ds_total=ds_total+sqrt(ds_squared);
     }

     if( land_e1 )TNext=Triangles[Tstart].GetN1();
     if( land_e2 )TNext=Triangles[Tstart].GetN2();
     if( land_e3 )TNext=Triangles[Tstart].GetN3();

     avecdash[0]=avec[TNext][0];
     avecdash[1]=avec[TNext][1];
     bvecdash[0]=bvec[TNext][0];
     bvecdash[1]=bvec[TNext][1];

     qStart=Triangles[Tstart].GetQuad();
     qNext=Triangles[Tstart].GetQuad();

     // edge 1 always crosses into an edge 1 in the opposite sense
     if(land_e1){
	     alphaNext=1.0-alpha_e1;
	     betaNext=0.0; 


             alpha_a=0.0;    //edge 1 
	     beta_a=0.0;
	     alpha_b=1.0; 
	     beta_b=0.0;
	     alpha_a_dash=alpha_b;  // same edge, opposite sense
	     beta_a_dash=beta_b;  
	     alpha_b_dash=alpha_a;
	     beta_b_dash=beta_a;  

      }

     // If the neighbour on edge 2 is in a different quad
     // it lands on edge 3, otherwise minus edge 2
      if(land_e2){
              alpha_a=0.0;   
	      beta_a=0.0;
	      alpha_b=0.0;  
	      beta_b=1.0;


	      if(qNext==qStart){
		 //crosses to edge 2 of neighbour- opposite sense
	         betaNext=1.0-beta_e2;
	         alphaNext=0.0;

	         alpha_a_dash=alpha_b; 
	         beta_a_dash=beta_b;  
	         alpha_b_dash=alpha_a;
	         beta_b_dash=beta_a;  
	      }
	      else
              {
		 //crosses to edge 3 of neighbour- same sense.
		 alphaNext=1.0-beta_e2;
		 betaNext=beta_e2;

	         alpha_a_dash=1.0; 
	         beta_a_dash=0.0;  
	         alpha_b_dash=0.0;
	         beta_b_dash=1.0;  
	      }
      }
      

     
      if(land_e3){
	      alpha_a=1.0;
	      beta_a=0.0;
	      alpha_b=0.0;
	      beta_b=1.0;
	      if(qNext==qStart){
		 //crosses to edge 2 of neighbour- same sense.
		  alphaNext=1.0-alpha_e3;
	          betaNext=alpha_e3;
		  alpha_a_dash=alpha_b;
		  beta_a_dash=beta_b;
		  alpha_b_dash=alpha_a;
		  beta_b_dash=beta_a;
	      }
	      else
              {
		   //crosses to edge 2 of neighbour -- has same sense
		   alphaNext=0.0;
		   betaNext=beta_e3;

		   alpha_a_dash=0.0;
		   beta_a_dash=0.0;
		   alpha_b_dash=0.0;
		   beta_b_dash=1.0;
	      }
      }  
      cout << "ds_total=" << ds_total << endl;

      X1=alpha_a_dash*avecdash[0]+beta_a_dash*bvecdash[0];
      X2=alpha_a_dash*avecdash[1]+beta_a_dash*bvecdash[1];

      Y1=alpha_a*avec[Tstart][0]+beta_a*bvec[Tstart][0];
      Y2=alpha_a*avec[Tstart][1]+beta_a*bvec[Tstart][1];

      W1=alpha_b_dash*avecdash[0]+beta_b_dash*bvecdash[0];
      W2=alpha_b_dash*avecdash[1]+beta_b_dash*bvecdash[1];

      Z1=alpha_b*avec[Tstart][0]+beta_b*bvec[Tstart][0];
      Z2=alpha_b*avec[Tstart][1]+beta_b*bvec[Tstart][1];

      Det=(W2-X2)*(W2-X2)+(W1-X1)*(W1-X1);


      unsigned int ir,ic,ir1,ir2;
      ir=0; ic=0;
      Mat(ir,ic)=W1-X1;
      ir=0; ic=1;
      Mat(ir,ic)=W2-X2;
      ir=1; ic=0;
      Mat(ir,ic)=-(W2-X2);
      ir=1; ic=1;
      Mat(ir,ic)=W1-X1;

      Mat=Mat/Det;
      ir=0;
      RHS(ir)=Z1-Y1;
      ir=1;
      RHS(ir)=Z2-Y2;

      LHS=Mat*RHS;

      ir=0;
      CosTheta=LHS(ir);
      ir=1;
      SinTheta=LHS(ir);

     // cout << "cos and sin "<< CosTheta << "  " << SinTheta << endl; 
     // cout << "cossq+sinsq=" << CosTheta*CosTheta+SinTheta*SinTheta << endl;

      avecdash_new[0]=
	      avecdash[0]*CosTheta-avecdash[1]*SinTheta;
      avecdash_new[1]=
	      avecdash[0]*SinTheta+avecdash[1]*CosTheta;
      bvecdash_new[0]=
	      bvecdash[0]*CosTheta-bvecdash[1]*SinTheta;
      bvecdash_new[1]=
	      bvecdash[0]*SinTheta+bvecdash[1]*CosTheta;

      adot_adash=avec[Tstart][0]*avecdash_new[0]
	      +avec[Tstart][1]*avecdash_new[1];
      adot_bdash=avec[Tstart][0]*bvecdash_new[0]
	      +avec[Tstart][1]*bvecdash_new[1];
      bdot_adash=bvec[Tstart][0]*avecdash_new[0]
	      +bvec[Tstart][1]*avecdash_new[1];
      bdot_bdash=bvec[Tstart][0]*bvecdash_new[0]
	      +bvec[Tstart][1]*bvecdash_new[1];

      /*
      cout  << adot_adash << " a dot adash  "  <<
	      avec[Tstart][0]*avecdash_new[0]+
	      avec[Tstart][1]*avecdash_new[1]  << endl;

      cout << adot_bdash << " a dot bdash  "  << 
	      avec[Tstart][0]*bvecdash_new[0]+
	      avec[Tstart][1]*bvecdash_new[1]  << endl;

      cout << bdot_adash << " b dot adash  "   << 
	      bvec[Tstart][0]*avecdash_new[0]+
	      bvec[Tstart][1]*avecdash_new[1]  << endl;

      cout <<  bdot_bdash << " b dot bdash  "  <<
	      bvec[Tstart][0]*bvecdash_new[0]+
	      bvec[Tstart][1]*bvecdash_new[1]  << endl;
	      */

      // The next couts all check out

  /*    cout  <<  " a dot a  "  <<
	      avec[Tstart][0]*avec[Tstart][0]+
	      avec[Tstart][1]*avec[Tstart][1]  << endl;

      cout << " b dot b  "  << 
	      bvec[Tstart][0]*bvec[Tstart][0]+
	      bvec[Tstart][1]*bvec[Tstart][1]  << endl;

      cout <<  " a dot a in Next  "   << 
	      avec[TNext][0]*avec[TNext][0]+
	      avec[TNext][1]*avec[TNext][1]  << endl;

      cout << bdot_bdash << " b dot b  in Next "  <<
	      bvec[TNext][0]*bvec[TNext][0]+
	      bvec[TNext][1]*bvec[TNext][1]  << endl;

      cout << "a " <<  avec[Tstart][0] 
	               <<  "   " <<  avec[Tstart][1]   << endl;
      cout << "b " <<  bvec[Tstart][0] 
	               << "   " <<   bvec[Tstart][1]   << endl;
		       */
 
       
/*
      cout << "a dash " <<  avecdash[0] 
	               << "   " <<   avecdash[1]   << endl;
      cout << "b dash " <<   bvecdash[0] 
	               <<  "   " <<  bvecdash[1]   << endl;
		       */

 /*     cout << "a dash rotate " <<  avecdash_new[0] 
	               <<  "   " <<  avecdash_new[1]   << endl;
      cout << "b dash rotate " <<   bvecdash_new[0]  
	               <<  "   " <<  bvecdash_new[1]  << endl;
      */


      ir=0;ic=0;
      Mat(ir,ic)=Gmetric[Tstart][2];
      ir=0;ic=1;
      Mat(ir,ic)=-Gmetric[Tstart][1];
      ir=1;ic=0;
      Mat(ir,ic)=-Gmetric[Tstart][1];
      ir=1;ic=1;
      Mat(ir,ic)=Gmetric[Tstart][0];

      Det=Gmetric[Tstart][0]*Gmetric[Tstart][2]
         -Gmetric[Tstart][1]*Gmetric[Tstart][1];

      Mat=Mat/Det;

      ir=0;
      RHS(ir)=adot_adash;
      ir=1;
      RHS(ir)=bdot_adash;

      LHS=Mat*RHS;

      ir=0;
      x1=LHS(ir);
      ir=1;
      y1=LHS(ir);

      ir=0;
      RHS(ir)=adot_bdash;
      ir=1;
      RHS(ir)=bdot_bdash;

      LHS=Mat*RHS;

      ir=0;
      x2=LHS(ir);
      ir=1;
      y2=LHS(ir);

      //check adash=x1 a + y1 b, bdash=x2 a+ y2 b
 /*     cout << x1*avec[Tstart][0]+y1*bvec[Tstart][0] 
	      << "  " << avecdash_new[0] << endl;
      cout << x1*avec[Tstart][1]+y1*bvec[Tstart][1]
	      << "  " << avecdash_new[1] << endl;
      cout << x2*avec[Tstart][0]+y2*bvec[Tstart][0]
	      << "  " << bvecdash_new[0] << endl;
      cout << x2*avec[Tstart][1]+y2*bvec[Tstart][1]
	      << "  " << bvecdash_new[1] << endl;
	      */

      ir=0;ic=0;
      Mat(ir,ic)=y2;
      ir=0;ic=1;
      Mat(ir,ic)=-x2;
      ir=1;ic=0;
      Mat(ir,ic)=-y1;
      ir=1;ic=1;
      Mat(ir,ic)=x1;

      Det=x1*y2-x2*y1;

      Mat=Mat/Det;

      ir=0;
      RHS(ir)=d1;
      ir=1;
      RHS(ir)=d2;

      LHS=Mat*RHS;

      ir=0;
      d1dash=LHS(ir);
      ir=1;
      d2dash=LHS(ir);

      // We have finally arrived in TNext, which will be the new Tstart;
      Tstart=TNext;
      alpha_start=alphaNext;
      beta_start=betaNext;
      d1=d1dash;
      d2=d2dash;

    } //end while ds_total

    Geokount=kount;

    cout << "Geokount=" << Geokount << endl;



    }
    else
    {
	    cout << "This code only propagates a geodesic if ndiv=4" << endl;
    }



 
 while(!quitit){
        
       while(SDL_PollEvent(&event)){

          switch(event.type){
               case SDL_QUIT:
                 quitit=true;
                 break;
                 case SDL_MOUSEBUTTONDOWN:
                    mousedown=true;
                 break;
                 case SDL_MOUSEBUTTONUP:
                    mousedown=false;
                 break;
                 case SDL_MOUSEMOTION:
                  if(mousedown){
                         if(MouseOn)Camera1.MouseView();}
                  else{
                         if(MouseOn)Camera1.MouseLookAt(); }
                 break;  


               case SDL_KEYDOWN:
                   whatkey=&event.key.keysym;
                   HandleKeyPress(whatkey);
                   break;
               case SDL_KEYUP:
                   whatkey=&event.key.keysym;
                   HandleKeyRelease(whatkey);
               default:
                 break;
                     } // end of case
                } //end of inner loop
              CheckMove(Camera1);
              RenderScene(Camera1);
            } //end of outer loop

}
void RenderScene(CCam & Camera1)
{

       glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
       glLoadIdentity();

       //
       //camera  pos      view     up vector
       gluLookAt(
        Camera1.CamPos.GetX(),   Camera1.CamPos.GetY(),  Camera1.CamPos.GetZ(),
        Camera1.CamView.GetX(), Camera1.CamView.GetY(), Camera1.CamView.GetZ(),
        Camera1.CamUp.GetX(),   Camera1.CamUp.GetY(),   Camera1.CamUp.GetZ());   


      //create fonts here
      //Check in Camera.h
      //Camera at (0,0,-5000)
      // stare at (0,0,0)

      //camera angle is 45 degees
      //camera 5000 away
      //-2000. is where xmin will be
      //+2000. is where xmax will be
      //
      //bounding box
 
       double exmin, whymin, zedmin;
       double exmax, whymax, zedmax;
       double exrange, whyrange,zedrange;
       exrange=xrange; whyrange=yrange; zedrange=zrange; 
       exmin=xmin, whymin=ymin; zedmin=zmin;


         double Halfscreen=3000.0;
         double Screen=2.*Halfscreen;
         double maxrange;
         maxrange=exrange;
         if(maxrange < whyrange)maxrange=whyrange;
         if(maxrange < zedrange)maxrange=zedrange;

         double Halfscreenx,Halfscreeny,Halfscreenz;
         Halfscreenx=Halfscreen*exrange/maxrange;
         Halfscreeny=Halfscreen*whyrange/maxrange;
         Halfscreenz=Halfscreen*zedrange/maxrange;
     

	 double x1,y1,z1;
	 double x2,y2,z2;
	 double x3,y3,z3;
	 double x4,y4,z4;
	 

      // materials
      float mat_spec[]={1.0, 0.0, 0.0, 0.0};  //polygon's ref of specular light
      float mat_diff[]={1.0, 0.0, 0.0, 0.0};  //polygon's ref of diffuse light
      float mat_amb[]={1.0, 0.0, 0.0, 0.0};  //polygon's ref of ambient light
      float mat_shine[]= {2.0};  //polygon's specular reflection
    // 0.0 to 128, small values give sharper specular reflection
      //lines
      //for triangulation lines
      float line_spec[]={0.0, 0.0, 0.0, 1.0};  //line's ref of specular light
      float line_amb[]={0.0, 0.0, 0.0, 1.0};  //line's ref of ambient  light
      float line_diff[]={0.0, 0.0, 0.0, 0.0};  //line's ref of diffuse light
      float line_shine[]= {0.0};  //lines's sharpness of specular ref 

      //For geodisic lines
      float geo_spec[]={0.0, 0.0, 1.0, 1.0};  //line's ref of specular light
      float geo_amb[]={0.0, 0.0, 1.0, 1.0};  //line's ref of ambient light
      float geo_diff[]={0.0, 0.0, 1.0, 0.0};  //line's ref of diffuse light
      float geo_shine[]= {0.0};  //lines's sharpness of specular ref 

      glLineWidth(3.0);
      //  glColor3ub(255,0,0);  //(without lighting enabled)
      //


      double xvals[3],yvals[3],zvals[3];

      for(int i=istart; i<istop; i++){

		 int mi,mj,mk;
		 mi=Triangles[i].Get1();
		 mj=Triangles[i].Get2();
		 mk=Triangles[i].Get3();

                 int j=0;
                 xvals[j]=NodeV[ Triangles[i].Get1()].GetX();
                 yvals[j]=NodeV[ Triangles[i].Get1()].GetY();
                 zvals[j]=NodeV[ Triangles[i].Get1()].GetZ();
		 j=1;
                 xvals[j]=NodeV[ Triangles[i].Get2()].GetX();
                 yvals[j]=NodeV[ Triangles[i].Get2()].GetY();
                 zvals[j]=NodeV[ Triangles[i].Get2()].GetZ();
		 j=2;
                 xvals[j]=NodeV[ Triangles[i].Get3()].GetX();
                 yvals[j]=NodeV[ Triangles[i].Get3()].GetY();
                 zvals[j]=NodeV[ Triangles[i].Get3()].GetZ();

	      D3Dvec edge1, edge2,cross,normal;
	      edge1.SetX(xvals[1]-xvals[0]);
	      edge1.SetY(yvals[1]-yvals[0]);
	      edge1.SetZ(zvals[1]-zvals[0]);
	      edge2.SetX(xvals[2]-xvals[0]);
	      edge2.SetY(yvals[2]-yvals[0]);
	      edge2.SetZ(zvals[2]-zvals[0]);

	      cross=edge1*edge2;
	      Normalise(cross);
	      glNormal3f( (float)cross.GetX(), cross.GetY(),cross.GetZ());


              float scaleit=3000.0;

      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shine);
              glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	      glBegin(GL_TRIANGLES);
          	glVertex3f( scaleit*xvals[0], scaleit*yvals[0], scaleit*zvals[0] );
          	glVertex3f( scaleit*xvals[1], scaleit*yvals[1], scaleit*zvals[1] );
          	glVertex3f( scaleit*xvals[2], scaleit*yvals[2], scaleit*zvals[2] );
             glEnd(); 


      scaleit=scaleit*1.001;

      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, line_spec);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, line_amb);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, line_spec);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, line_shine);

              glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	      glBegin(GL_TRIANGLES);
          	glVertex3f( scaleit*xvals[0], scaleit*yvals[0], scaleit*zvals[0] );
          	glVertex3f( scaleit*xvals[1], scaleit*yvals[1], scaleit*zvals[1] );
          	glVertex3f( scaleit*xvals[2], scaleit*yvals[2], scaleit*zvals[2] );
             glEnd(); 
      }

      float scaleit=3000.0;
      scaleit=scaleit*1.1;

      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, geo_spec);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, geo_amb);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, geo_spec);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, geo_shine);
      for(int i=0; i<Geokount-1; i++){
	      xvals[0]=Geodesy[i].GetX();
	      yvals[0]=Geodesy[i].GetY();
	      zvals[0]=Geodesy[i].GetZ();

	      xvals[1]=Geodesy[i+1].GetX();
	      yvals[1]=Geodesy[i+1].GetY();
	      zvals[1]=Geodesy[i+1].GetZ();
	      glBegin(GL_LINES);
          	glVertex3f( scaleit*xvals[0], scaleit*yvals[0], scaleit*zvals[0] );
          	glVertex3f( scaleit*xvals[1], scaleit*yvals[1], scaleit*zvals[1] );
	      glEnd();

	//      cout <<"kount=" << i << endl;
     // cout << "r1=" << xvals[0] << "  " << yvals[0] << "  " << zvals[0] << endl;  
     // cout << "r2=" << xvals[1] << "  " << yvals[1] << "  " << zvals[1] << endl;  
      }



      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

      SDL_GL_SwapBuffers();

}

void HandleKeyPress(SDL_keysym *whatkey)
{ 
             int igetscreen;
             float myspeed=speed;

             switch(whatkey->sym)
             {
                 case SDLK_F1:
                   ToggleFullScreen();
                   break;
                 case SDLK_F2:
                   if(MouseOn==true)
                     {
                      MouseOn=false;
                     }
                     else
                    {
                      MouseOn=true;
                    }
                    break;
                 case SDLK_F9:
                   if(togglez==true)
                        togglez=false;
                   else
                        togglez=true;

                 case SDLK_F10:
                   igetscreen=Screenshot(MainWindow,"screenshot.bmp");
                   break;
                 case SDLK_ESCAPE:
                   SDL_Quit();
                   exit(0);
                   break;
                 case  SDLK_UP:
                   upPressed=true;
                   break;
		 case  SDLK_DOWN:
                   downPressed=true;
                   break;
                 case SDLK_LEFT:
                   leftPressed=true;
                   if( whatkey->mod & KMOD_CTRL)
                   {
                   leftPressed =false;
                   ctrl_leftPressed=true;
                   }
                   if( whatkey->mod & KMOD_SHIFT){
                   leftPressed =false;
                   shift_leftPressed=true;
                   }
                   break;
                 case SDLK_RIGHT:
                   rightPressed=true;
                   if( whatkey->mod & KMOD_CTRL)
                   {
                   rightPressed =false;
                   ctrl_rightPressed=true;
                   }
                   if( whatkey->mod & KMOD_SHIFT){
                   rightPressed =false;
                   shift_rightPressed=true;
                   }
                 default:
                   break;
                 case SDLK_p:
                   p_Pressed=true;
                   break; 
                 case SDLK_F12:
                   iplot++;
                   break;
             }
}

void HandleKeyRelease(SDL_keysym *whatkey)
{ 
             switch(whatkey->sym)
             {
                 case  SDLK_UP:
                   upPressed=false;
                   break;
		 case  SDLK_DOWN:
                   downPressed=false;
                   break;
                 case SDLK_LEFT:
                   leftPressed=false;
                   shift_leftPressed=false;
                   ctrl_leftPressed=false;
                   break;
                 case SDLK_RIGHT:
                   rightPressed=false;
                   shift_rightPressed=false;
                   ctrl_rightPressed=false;
                   break;
                 default:
                   break;
                 case SDLK_p:
                   p_Pressed=false;
                   break; 
             }
}

void CheckMove(CCam & Camera1)
{
    if(downPressed)Camera1.CamMove(-speed);
    if(upPressed)Camera1.CamMove(speed);
             
    if(leftPressed)Camera1.CamRotatePos(-speed/1000);
    if(rightPressed)Camera1.CamRotatePos(speed/1000);

    //assumes noone does ctrl and shift together!
    if(shift_leftPressed)Camera1.CamRotateView(-speed/1000);
    if(shift_rightPressed)Camera1.CamRotateView(speed/1000);
    if(ctrl_leftPressed)Camera1.CamStrafe(speed);
    if(ctrl_rightPressed)Camera1.CamStrafe(-speed);

    if(p_Pressed){

    FILE *fp=fopen("MyFileName.ps","wb");
    GLint buffsize=0;
    GLint state=GL2PS_OVERFLOW;
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    while(state==GL2PS_OVERFLOW){
       buffsize+=1024*1024;
//     could use GL2PS_USE_CURRENT_VIEWPORT,
       int ips=gl2psBeginPage("MyTitle","MyProducer",viewport,
               GL2PS_PS,GL2PS_BSP_SORT,GL2PS_SILENT | 
	       GL2PS_SIMPLE_LINE_OFFSET | GL2PS_BEST_ROOT,GL_RGBA,0,NULL,
               0,0,0,buffsize,fp,"MyFileName.ps");
       RenderScene(Camera1);
       state=gl2psEndPage();

       }  //end while loop
      fclose(fp);
    } // end p_Pressed

}
