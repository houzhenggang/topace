#include"Draw.h"
#include <stdlib.h>
#include <GL/glew.h>
#include <math.h>
#include <stdio.h>	
#include "Glsl.h"
#include <lib3ds.h>
#include <string.h>	
#include "Load3DS.h"
#include "Font2D.h"
bool extern domultiR ;
bool extern doangleR ;
float angleR= 0.0f;
CFont2D * Font2D=NULL; 
bool Inited=false;
LARGE_INTEGER TimeStart,TimeEnd,Timefeq;
int FPSNum=0;
int FPSNumShow=0; 
char ShowFPS[64]={0};
void InitDraw()
{
	if(Inited)
		return;
	Inited=true;
	if(!Font2D)
	{
		char szPath[MAX_PATH];
		GetWindowsDirectory(szPath,sizeof(szPath));
		char FontPath[MAX_PATH];
		sprintf(FontPath,"%s/Fonts/simsun.ttc",szPath);
	//FontsTest.LoadFont(FontPath,16,16,128,128);
	//FontsTest.inputTxt("qwerttyyuioop[]");
		Font2D=new CFont2D;
		Font2D->LoadFont(FontPath,32,32,256,256);
		//Font2D->inputTxt("ab");
	}
	sprintf(ShowFPS,"-");
	QueryPerformanceCounter(&TimeStart);
	QueryPerformanceFrequency(&Timefeq);

}
void DeinitDraw()
{
	if(Font2D)
	delete Font2D;
	Font2D=NULL;
}
void Draw()
{
	QueryPerformanceFrequency(&Timefeq);
	QueryPerformanceCounter(&TimeEnd);
	if(double((TimeEnd.QuadPart-TimeStart.QuadPart)/Timefeq.QuadPart)>=1.0)
	{
		QueryPerformanceCounter(&TimeStart);
		FPSNumShow=FPSNum;
		FPSNum=0;
		sprintf(ShowFPS,"测试FPS:%6d",FPSNumShow);
		Font2D->inputTxt(ShowFPS);
	}
	FPSNum=FPSNum+1;


	// ROACH
	if(domultiR)
		glEnable(GL_MULTISAMPLE_ARB);							// Enable Our Multisampling
	// ENDROACH

	glClearColor(0.0f, 0.0f, 0.0f, 0.5);						// Set The Clear Color To Black
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity();											// Reset The View	

	//glDisable( GL_TEXTURE_2D );
	for(float i=-3;i<3;i++)
		for(float j=-3;j<3;j++)
		{
			glPushMatrix();
			glTranslatef(i*2.0f,j*2.0f,-5.0f);
			glRotatef(angleR,0.f,0.f,1.f);
				glBegin(GL_QUADS);
				glColor3f(1.0f,0.0f,0.0f);glVertex3f(i,j,0.0f);glTexCoord2d(0,0);
				glColor3f(0.0f,1.0f,0.0f);glVertex3f(i + 2.0f,j,0.0f);glTexCoord2d(1,0);
				glColor3f(0.0f,0.0f,1.0f);glVertex3f(i + 2.0f,j + 2.0f,0.0f);glTexCoord2d(1,1);
				glColor3f(1.0f,1.0f,1.0f);glVertex3f(i,j + 2.0f,0.0f);glTexCoord2d(0,1);
				glEnd();
			glPopMatrix();
		}

	
	//if(doangleR)
	//	angleR+=0.05f;

	//glFlush ();													// Flush The GL Rendering Pipeline
	glEnable( GL_TEXTURE_2D );
	Font2D->DrawTXT(800,600,0,0,24,24,800,3);

	if(domultiR)
	glDisable(GL_MULTISAMPLE_ARB);
	// ROACH
	

	// ENDROACH

}