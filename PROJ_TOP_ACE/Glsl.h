﻿#ifndef _GLSL_H
#define _GLSL_H
#include <GL/glew.h>
struct _ShaderGLSL
{
	GLhandleARB g_VS;
	GLhandleARB g_PS;
	GLhandleARB g_PO;//ProgramObject
};
void InitGLSL(int LightSet=2);
void DeinitGLSL();
//void GLSL_Enable_ATC();
//void GLSL_Disable_ATC();
void GLSL_Enable_PhoneLight(int OmniLightNum=1,int SpotLightNum=0);
void GLSL_Disable();
void GLSL_Enable_StarPass0();
void GLSL_Enable_StarPass1();
void DrawBloomMapGLSL(int WinW,int WinH);
void DrawBloomWGLSL(int WinW);
void DrawBloomHGLSL(int WinH);
void ToneMappingGLSL();
void SSAOPass0();
void SSAOPass1(float SSAOset[4]);
void InitBlurTex();
void DeinitBlurTex();
void BlurTex(int Size,bool WorH);
GLhandleARB GLSL_CompileShader(unsigned char *ShaderAssembly,unsigned int ShaderObject);
GLhandleARB GLSL_CompileShader(const char* shaderfilename,unsigned int ShaderObject);
GLhandleARB GLSL_CompileShader(char *ShaderAssembly,GLenum ShaderObject);
GLhandleARB GLSL_CompileShader(const wchar_t* shaderfilename,GLenum ShaderObject);

#endif