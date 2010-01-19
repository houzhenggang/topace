//#pragma once
#ifndef _LOAD3DS_H
#define _LOAD3DS_H
#include <gl\glew.h>
#include <lib3ds.h>
#include"Textures.h"
#define NO_ERROR_3DS 0
#define ERROR_NOT_OPEN_FILE 1
#define ERROR_NO_MESH 2
struct tModelNodes
{
	unsigned int VerticeID;
	unsigned int ColorID;
	unsigned int TexCoordID;
	unsigned int NormalID;
	unsigned int TangentID;
	int VerticeNum;
	float MeshMatrix[4][4];

	bool UseMaterial;
	bool IsBone;//骨骼，不渲染
	bool CX;//俯仰
	bool CY;//左右
	bool CZ;//侧翻
	bool EN;//引擎推力
	bool AB;//减速板
	bool WP;//起落架
	bool WW;//起落架盖
	bool SP;//特殊
	bool GR;//透明
	bool MH;//导弹仓门
	float mat_specular[4];
	float mat_ambient[4];
	float mat_diffuse[4];
	float mat_shininess;
};
class CLoad3DS
{
public:
	CLoad3DS(void);
	~CLoad3DS(void);
	bool Loadfile(char * filename);
	bool LoadToVRAM(void);
	void Del_RAM(void);
	void Del_VRAM(void);
	bool isRAM;
	bool isVRAM;
	bool VBOSupported;
	Lib3dsFile *Model3ds;
	int TotelVertices;
	int TotelMeshs;
	tModelNodes * VBOIDs;
	void Render(float current_frame=0.0f);
	void RenderNode(Lib3dsNode *Node,bool isTranslucent=false);
	bool LoadNode(Lib3dsNode *Node);
	int Error;
	int MeshLoadNum;
	bool Clear3DSIDs(Lib3dsNode *Node);
	unsigned int GrassTexID;
	unsigned int DiffuseTexID;
	void GetNodeType(int NodeID,const char * NodeName);
};

#endif