//#pragma once
#ifndef _GLSLLOADER_H
#define _GLSLLOADER_H
#include <GL/glew.h>
#include "TALogSys.h"
#define ShaderPath L"data/shader/GLSL/"
GLhandleARB CompileShader(const wchar_t* shaderfilename,GLenum ShaderObject);
GLhandleARB CompileShader(char *ShaderAssembly,GLenum ShaderObject,GLint * bCompiled);
char * GetGLSLInfoLog(GLhandleARB GLSLShaderObject);
bool LinkShader(GLhandleARB GLSL_programObj);
class CGLSLLoader
{
public:
	CGLSLLoader(void);
	~CGLSLLoader(void);
	bool GetGLSLLinkSTATUS(GLhandleARB g_programObj);
	CTALogSys GLSLLOG;
	bool LoadShader(const wchar_t* VSfilename,const wchar_t* PSfilename);
	void ClearShader(void);
	GLhandleARB g_VS;
	GLhandleARB g_TC;
	GLhandleARB g_TE;
	GLhandleARB g_GS;
	GLhandleARB g_PS;
	GLhandleARB g_PO;//ProgramObject
	bool LoadShader2(const wchar_t* ShaderFullName,int ShaderLevel);
	bool LoadShader(const wchar_t* ShaderName,int ShaderLevel,bool ClearOldShader=false);
};

#endif