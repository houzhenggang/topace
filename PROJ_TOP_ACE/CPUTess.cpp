﻿#include "CPUTess.h"


CCPUTess::CCPUTess(void)
{
}


CCPUTess::~CCPUTess(void)
{
}


bool CCPUTess::AddModel(float * vertices,float * Normals,float * texcos,unsigned int Count)
{
	if((!vertices)||(Count<=0))return false;

	return true;
}
void PNTriangle(
	float * Vertices,unsigned int VerticeFloatNum,
	float * Normals,unsigned int NormalFloatNum,
	float * Texcos,unsigned int TexcosFloatNum,
	float * NewVertices,float * NewNormals,float * NewTexcos,float * TriangleCos)
{
}
bool TessModel_HalfCut(
	float * Vertices,unsigned int VerticeFloatNum,
	float * Normals,unsigned int NormalFloatNum,
	float * Texcos,unsigned int TexcosFloatNum,
	unsigned int * Count,unsigned int TessLevel)
{
	if((!Vertices)||(!Count))return false; if((TessLevel<=0)||(Count[0]<=0))return false;
	unsigned int NewCount = Count[0]*4;
	float * NewVertices	= new float[NewCount*VerticeFloatNum];
	float * NewNormals	= NULL; if(Normals) NewNormals = new float[NewCount*NormalFloatNum];
	float * NewTexcos	= NULL; if(Texcos ) NewTexcos  = new float[NewCount*TexcosFloatNum];
	memcpy(NewVertices,Vertices,Count[0]*sizeof(float)*VerticeFloatNum);
	if(Normals) 
	memcpy(NewNormals ,Normals ,Count[0]*sizeof(float)*NormalFloatNum );
	if(Texcos ) 
	memcpy(NewVertices,Texcos  ,Count[0]*sizeof(float)*TexcosFloatNum );
	float TriangleCos[3];
	for(unsigned int i=Count[0];i<NewCount;i++)
	{
		TriangleCos[0]=1.0f;
		TriangleCos[1]=0.0f;
		TriangleCos[2]=0.0f;
		/*PNTriangle(
			NewVertices+(i-Count[0])*VerticeFloatNum,VerticeFloatNum,
			NewNormals +(i-Count[0])*NormalFloatNum ,NormalFloatNum ,
			NewTexcos  +(i-Count[0])*TexcosFloatNum ,TexcosFloatNum ,
			NewVertices+i*VerticeFloatNum
			*/
		//NewVertices+i*VerticeFloatNum
		//NewVertices[i*VerticeStride]
	}
	return true;
}