#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "/usr/lib64/qt-3.3/include/qstring.h"
#include "/usr/lib64/qt-3.3/include/qcolor.h"
#include "/usr/include/oglft/OGLFT.h"
#include "/usr/include/SDL/SDL.h"
#include "D3Dvec.h"
#include "Triangle.h"

#include "throwcatch.h"

typedef unsigned char byte;
typedef unsigned short int USHORT;
typedef unsigned int UINT;

const USHORT WIDTH=800, HEIGHT=600;
const USHORT SCREEN_DEPTH=16;

extern const double twopi,pi;  //initialised externally

#include "Camera.h"
#include "gl2ps.h"

extern int VideoFlags;
extern SDL_Surface * MainWindow;

extern const double speed;
extern const double convert;


// Prototypes for functions
void EventLoop(CCam  & Camera1);
void RenderScene(CCam  & Camera1);
void SetUp();
void ToggleWindow(void);
void MakeWindow(const char *Name, 
		const USHORT WIDTH,
	        const USHORT HEIGHT,
		 int VideoFlags);
void SizeOpenGLScreen(const USHORT, const USHORT);
void Init();
void InitialiseGL(int, int);
void HandleKeyPress(SDL_keysym * );
void HandleKeyRelease(SDL_keysym *);
void CheckMove(CCam &);
int Screenshot(SDL_Surface *screen, char *filename);

void CreateTexture(unsigned int textureArray[], char *strFilename, int textureID);

void OrthoMode(int, int, int, int);
void PerspectiveMode(int, int, int, int);
#include "Init.h"


