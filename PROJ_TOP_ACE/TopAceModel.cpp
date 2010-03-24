#include "TopAceModel.h"//文件头

CTopAceModel::CTopAceModel(void)
:TAM_File_States(_TAM_File_States_NoRead)
, TAM_FileData(NULL)
, pTAM_FileHead(NULL)
, SuppotVBO(false)
, MeshVBOIDs(NULL)
, isVRAM(false)
{
}

CTopAceModel::~CTopAceModel(void)
{
	DeleteVRAM();
	if(TAM_FileData)
		_aligned_free(TAM_FileData);
	TAM_FileData = NULL;
	pTAM_FileHead = NULL;

}

bool CTopAceModel::ReadTAMFile(char * FileName)
{
	if(TAM_File_States==_TAM_File_States_ReadOK)
		return false;
	HANDLE   hFile;     
      
	hFile   =   CreateFileA(FileName,                 
                                  GENERIC_READ,                 
                                  FILE_SHARE_READ,                     
                                  NULL,                                  
                                  OPEN_EXISTING,                        
                                  FILE_ATTRIBUTE_NORMAL,       
                                  NULL); 
	if   (hFile   ==   INVALID_HANDLE_VALUE)  
	{
		CloseHandle(hFile);
		TAM_File_States=_TAM_File_States_NoFile;
		TAM_FileData = NULL;
		return false;
	}

	_TAM_FileHead TAM_FileHeadTMP;
	DWORD Readedsize=0;
	BOOL ReadStates=ReadFile(hFile,&TAM_FileHeadTMP,sizeof(TAM_FileHeadTMP),&Readedsize,NULL);
	if(!ReadStates)
	{
		CloseHandle(hFile);
		TAM_File_States=_TAM_File_States_NoFile;
		TAM_FileData = NULL;
		return false;
	}
	if(TAM_FileHeadTMP.FileSize<sizeof(TAM_FileHeadTMP))
	{
		TAM_File_States=_TAM_File_States_ReadFail;
		TAM_FileData = NULL;
		CloseHandle(hFile);
		return false;
	}
	TAM_FileData = (unsigned char * ) _aligned_malloc (TAM_FileHeadTMP.FileSize,16);

	Readedsize = 0;
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);
	ReadStates=ReadFile(hFile,TAM_FileData,TAM_FileHeadTMP.FileSize,&Readedsize,NULL);
	if(!ReadStates)
	{
		DWORD dwError = GetLastError();
		_aligned_free(TAM_FileData);
		TAM_FileData = NULL;
		CloseHandle(hFile);
		TAM_File_States=_TAM_File_States_ReadFail;
		return false;
	}
	if(!InitTAMFile(TAM_FileData))
	{
		_aligned_free(TAM_FileData);
		TAM_FileData = NULL;
		CloseHandle(hFile);
		TAM_File_States=_TAM_File_States_ReadFail;
		return false;
	}


	CloseHandle(hFile);
	return true;
}
bool CTopAceModel::ReadTAMFile(wchar_t * FileName)
{
	HANDLE   hFile;     
      
	hFile   =   CreateFileW(FileName,                       //   open   MYFILE.TXT     
                                  GENERIC_READ,                             //   open   for   reading     
                                  FILE_SHARE_READ,                       //   share   for   reading     
                                  NULL,                                             //   no   security     
                                  OPEN_EXISTING,                           //   existing   file   only     
                                  FILE_ATTRIBUTE_NORMAL,           //   normal   file     
                                  NULL);     
	if   (hFile   ==   INVALID_HANDLE_VALUE)  
	{
		CloseHandle(hFile);
		TAM_File_States=_TAM_File_States_NoFile;
		return false;
	}

	_TAM_FileHead TAM_FileHeadTMP;
	DWORD Readedsize=0;
	BOOL ReadStates=ReadFile(hFile,&TAM_FileHeadTMP,sizeof(TAM_FileHeadTMP),&Readedsize,NULL);
	if(!ReadStates)
	{
		CloseHandle(hFile);
		TAM_File_States=_TAM_File_States_NoFile;
		TAM_FileData = NULL;
		return false;
	}
	if(TAM_FileHeadTMP.FileSize<sizeof(TAM_FileHeadTMP))
	{
		TAM_File_States=_TAM_File_States_ReadFail;
		TAM_FileData = NULL;
		CloseHandle(hFile);
		return false;
	}
	TAM_FileData = (unsigned char * ) _aligned_malloc (TAM_FileHeadTMP.FileSize,16);

	Readedsize = 0;
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);
	ReadStates=ReadFile(hFile,TAM_FileData,TAM_FileHeadTMP.FileSize,&Readedsize,NULL);
	if(!ReadStates)
	{
		DWORD dwError = GetLastError();
		_aligned_free(TAM_FileData);
		TAM_FileData = NULL;
		CloseHandle(hFile);
		TAM_File_States=_TAM_File_States_ReadFail;
		return false;
	}
	if(!InitTAMFile(TAM_FileData))
	{
		_aligned_free(TAM_FileData);
		TAM_FileData = NULL;
		CloseHandle(hFile);
		TAM_File_States=_TAM_File_States_ReadFail;
		return false;
	}


	CloseHandle(hFile);
	return true;
}
bool CTopAceModel::InitTAMFile(unsigned char * TAM_FileData_IN)
{
	if(!TAM_FileData_IN)
		return false;

	_TAM_FileHead  * TAM_FileHead_IN=(_TAM_FileHead*)TAM_FileData_IN;
	pTAM_FileHead=TAM_FileHead_IN;

	if((unsigned int(TAM_FileHead_IN->MeshHeadAddress)!=0xFFFFFFFF)&&(TAM_FileHead_IN->MeshNum)>0)
	{
		TAM_FileHead_IN->MeshHeadAddress=(_TAM_Mesh ** )&TAM_FileData_IN[(int)TAM_FileHead_IN->MeshHeadAddress];
		for (unsigned int i=0;i<TAM_FileHead_IN->MeshNum;i++)
		{
			TAM_FileHead_IN->MeshHeadAddress[i]=(_TAM_Mesh *)(int(TAM_FileHead_IN->MeshHeadAddress[i])+int(TAM_FileData_IN));
			InitTAMMesh(TAM_FileHead_IN->MeshHeadAddress[i]);
		}
	}

	if((unsigned int(TAM_FileHead_IN->MatsAddress)!=0xFFFFFFFF)&&(TAM_FileHead_IN->MatNum>0))
	{
		TAM_FileHead_IN->MatsAddress=(_TAM_Mat * )&TAM_FileData_IN[(int)TAM_FileHead_IN->MatsAddress];
		for (unsigned int i=0;i<TAM_FileHead_IN->MatNum;i++)
		{
			InitTAMMat(&(TAM_FileHead_IN->MatsAddress[i]));
		}
	}

	if((unsigned int(TAM_FileHead_IN->BoneHeadAddress)!=0xFFFFFFFF)&&(TAM_FileHead_IN->BoneNum>0))
	{
		TAM_FileHead_IN->BoneHeadAddress=(_TAM_Bone ** )&TAM_FileData_IN[(int)TAM_FileHead_IN->BoneHeadAddress];
		for (unsigned int i=0;i<TAM_FileHead_IN->BoneNum;i++)
		{
			TAM_FileHead_IN->BoneHeadAddress[i]=(_TAM_Bone *)(int(TAM_FileHead_IN->BoneHeadAddress[i])+int(TAM_FileData_IN));
			InitTAMBone(TAM_FileHead_IN->BoneHeadAddress[i]);
		}
	}

	if((unsigned int(TAM_FileHead_IN->LightHeadAddress)!=0xFFFFFFFF)&&(TAM_FileHead_IN->LightNum>0))
	{
		TAM_FileHead_IN->LightHeadAddress=(_TAM_Light ** )&TAM_FileData_IN[(int)TAM_FileHead_IN->LightHeadAddress];
		for (unsigned int i=0;i<TAM_FileHead_IN->LightNum;i++)
		{
			TAM_FileHead_IN->LightHeadAddress[i]=(_TAM_Light *)(int(TAM_FileHead_IN->LightHeadAddress[i])+int(TAM_FileData_IN));
			InitTAMLight(TAM_FileHead_IN->LightHeadAddress[i]);
		}
	}

	if((unsigned int(TAM_FileHead_IN->CameraAddress)!=0xFFFFFFFF)&&(TAM_FileHead_IN->CameraNum>0))
	{
		TAM_FileHead_IN->CameraAddress=(_TAM_Camera * )&TAM_FileData_IN[(int)TAM_FileHead_IN->CameraAddress];
		for (unsigned int i=0;i<TAM_FileHead_IN->CameraNum;i++)
		{
			InitTAMCamera(&(TAM_FileHead_IN->CameraAddress[i]));
		}
	}
	
/*
	//_TAM_Mesh ** OBJAddressTMP=NULL;
	//OBJAddressTMP=(_TAM_Mesh **)(TAM_FileHead_IN->MeshHeadAddress);
	for (int i=0;i<TAM_FileHead_IN->MeshNum;i++)
	{
		//TAM_FileHead_IN->MeshHeadAddress[i]=(_TAM_Mesh *)&TAM_FileData_IN[int(TAM_FileHead_IN->MeshHeadAddress[i])];
		TAM_FileHead_IN->MeshHeadAddress[i]=(_TAM_Mesh *)(int(TAM_FileHead_IN->MeshHeadAddress[i])+int(TAM_FileData_IN));
		int sererwer=12132;
	}
	_TAM_Mesh * TTT_TAM_Mesh=TAM_FileHead_IN->MeshHeadAddress[0];
	*/
	TAM_File_States = _TAM_File_States_ReadOK;
	return true;
}
bool CTopAceModel::InitTAMMesh(_TAM_Mesh * TAM_MeshData_IN)
{
	if(!((TAM_MeshData_IN->FaceNum>0)&&(TAM_MeshData_IN->vecNum>0)))
		return false;
	TAM_MeshData_IN->vertices=(__m128 *)&TAM_FileData[(int)TAM_MeshData_IN->vertices];
	TAM_MeshData_IN->Normals=(__m128 *)&TAM_FileData[(int)TAM_MeshData_IN->Normals];
	TAM_MeshData_IN->texcos=(float *)&TAM_FileData[(int)TAM_MeshData_IN->texcos];
	TAM_MeshData_IN->Faces=(unsigned int *)&TAM_FileData[(int)TAM_MeshData_IN->Faces];
	//for(unsigned int i=0;i<TAM_MeshData_IN->FaceNum*3;i++)
	//	TAM_MeshData_IN->Faces[i]=TAM_MeshData_IN->Faces[i]-1;

	return true;
}

bool CTopAceModel::InitTAMMat(_TAM_Mat * TAM_MatData_IN)
{
	return true;
}
bool CTopAceModel::InitTAMBone(_TAM_Bone * TAM_BoneData_IN)
{
	for(unsigned int i=0;i<TAM_BoneData_IN->FrameChunkNum;i++)
	{
		if(unsigned int(TAM_BoneData_IN->FramesHeadAddress[i])==0xFFFFFFFF)
			continue;

		TAM_BoneData_IN->FramesHeadAddress[i]=(_TAM_Bone_Frame_Head *)&TAM_FileData[int(TAM_BoneData_IN->FramesHeadAddress[i])];
		if(TAM_BoneData_IN->FramesHeadAddress[i]->MAXFrameNum>1)
		{
			_TAM_Bone_Frame ** TAM_Bone_Frames=(_TAM_Bone_Frame **) &(TAM_BoneData_IN->FramesHeadAddress[i]->FramesAddress);
			for(unsigned int j=0;j<=TAM_BoneData_IN->FramesHeadAddress[i]->MAXFrameNum;j++)
			{
				TAM_Bone_Frames[j]=(_TAM_Bone_Frame *)&TAM_FileData[int(TAM_Bone_Frames[j])];
			}
			_TAM_Bone_Frame * pTAM_Bone_FrameTMP=TAM_Bone_Frames[TAM_BoneData_IN->FramesHeadAddress[i]->MAXFrameNum];
			while(pTAM_Bone_FrameTMP->FrameTime!=0)
			{
				pTAM_Bone_FrameTMP->PreviousFrameAddress=(_TAM_Bone_Frame *)&TAM_FileData[int(pTAM_Bone_FrameTMP->PreviousFrameAddress)];
				pTAM_Bone_FrameTMP=pTAM_Bone_FrameTMP->PreviousFrameAddress;
			}
			if(pTAM_Bone_FrameTMP->FrameTime==0)
			{
				pTAM_Bone_FrameTMP->PreviousFrameAddress=pTAM_Bone_FrameTMP;
			}
		}
	}
	return true;
}
bool CTopAceModel::InitTAMLight(_TAM_Light * TAM_LightData_IN)
{
	for(unsigned int i=0;i<3;i++)
	{
		if(unsigned int(TAM_LightData_IN->FramesHeadAddress[i])==0xFFFFFFFF)
			continue;
		TAM_LightData_IN->FramesHeadAddress[i]=(_TAM_Light_Frame_Head *)&TAM_FileData[int(TAM_LightData_IN->FramesHeadAddress[i])];
		if(TAM_LightData_IN->FramesHeadAddress[i]->MAXFrameNum>1)
		{
			_TAM_Light_Frame ** TAM_Light_Frames=(_TAM_Light_Frame **)&(TAM_LightData_IN->FramesHeadAddress[i]->FramesAddress);
			for(unsigned int j=0;j<=TAM_LightData_IN->FramesHeadAddress[i]->MAXFrameNum;j++)
				TAM_Light_Frames[j]=(_TAM_Light_Frame *)&TAM_FileData[int(TAM_Light_Frames[j])];
			_TAM_Light_Frame * pTAM_Light_FrameTMP=TAM_Light_Frames[TAM_LightData_IN->FramesHeadAddress[i]->MAXFrameNum];
			while(pTAM_Light_FrameTMP->FrameTime!=0)
			{
				pTAM_Light_FrameTMP->PreviousFrameAddress=(_TAM_Light_Frame *)&TAM_FileData[int(pTAM_Light_FrameTMP->PreviousFrameAddress)];
				pTAM_Light_FrameTMP=pTAM_Light_FrameTMP->PreviousFrameAddress;
			}
			if(pTAM_Light_FrameTMP->FrameTime==0)
			{
				pTAM_Light_FrameTMP->PreviousFrameAddress=pTAM_Light_FrameTMP;
			}
		}
	}
	return true;
}
bool CTopAceModel::InitTAMCamera(_TAM_Camera * TAM_CameraData_IN)
{
	return true;
}
_TAM_Mesh * CTopAceModel::TAM_Get_Mesh(unsigned int MeshID)
{
	if(!MeshID)
		return NULL;
	if(!pTAM_FileHead)
		return NULL;
	if(unsigned int(pTAM_FileHead->MeshHeadAddress)==0xFFFFFFFF)
		return NULL;
	if(pTAM_FileHead->MeshNum<=0)
		return NULL;
	if(pTAM_FileHead->MeshNum<=(MeshID-1))
		return NULL;

	return pTAM_FileHead->MeshHeadAddress[(MeshID-1)];
}

_TAM_Bone * CTopAceModel::TAM_Get_Bone(unsigned int BoneID)
{
	if(!BoneID)
		return NULL;
	if(!pTAM_FileHead)
		return NULL;
	if(unsigned int(pTAM_FileHead->BoneHeadAddress)==0xFFFFFFFF)
		return NULL;
	if(pTAM_FileHead->BoneNum<=0)
		return NULL;
	if(pTAM_FileHead->BoneNum<=(BoneID-1))
		return NULL;

	return pTAM_FileHead->BoneHeadAddress[(BoneID-1)];
}

_TAM_Light * CTopAceModel::TAM_Get_Light(unsigned int LightID)
{
	if(!LightID)
		return NULL;
	if(!pTAM_FileHead)
		return NULL;
	if(unsigned int(pTAM_FileHead->LightHeadAddress)==0xFFFFFFFF)
		return NULL;
	if(pTAM_FileHead->LightNum<=0)
		return NULL;
	if(pTAM_FileHead->LightNum<=(LightID-1))
		return NULL;

	return pTAM_FileHead->LightHeadAddress[(LightID-1)];
}

_TAM_Bone_Frame * CTopAceModel::TAM_Get_Bone_Frame(unsigned int BoneFrameID,unsigned int BoneID,_TAM_Bone_Frame_Type TAM_Bone_Frame_Type)
{
	return TAM_Get_Bone_Frame(BoneFrameID,TAM_Get_Bone(BoneID),TAM_Bone_Frame_Type);
}
_TAM_Bone_Frame * CTopAceModel::TAM_Get_Bone_Frame(unsigned int BoneFrameID,_TAM_Bone * TAM_Bone,_TAM_Bone_Frame_Type TAM_Bone_Frame_Type)
{
	if(!pTAM_FileHead)
		return NULL;
	if(unsigned int(pTAM_FileHead->BoneHeadAddress)==0xFFFFFFFF)
		return NULL;
	if(pTAM_FileHead->BoneNum<=0)
		return NULL;
	if(!TAM_Bone)
		return NULL;

	unsigned int BoneFrameIDTMP=min(TAM_Bone->FramesHeadAddress[TAM_Bone_Frame_Type]->MAXFrameNum,BoneFrameID);
	_TAM_Bone_Frame ** TAM_Bone_Frames=(_TAM_Bone_Frame **) &(TAM_Bone->FramesHeadAddress[TAM_Bone_Frame_Type]->FramesAddress);
	return TAM_Bone_Frames[BoneFrameIDTMP];
}

_TAM_Light_Frame * CTopAceModel::TAM_Get_Light_Frame(unsigned int LightFrameID,unsigned int LightID,_TAM_Light_Frame_Type TAM_Light_Frame_Type)
{
	return TAM_Get_Light_Frame(LightFrameID,TAM_Get_Light(LightID),TAM_Light_Frame_Type);
}
_TAM_Light_Frame * CTopAceModel::TAM_Get_Light_Frame(unsigned int LightFrameID,_TAM_Light * TAM_Light,_TAM_Light_Frame_Type TAM_Light_Frame_Type)
{
	if(!pTAM_FileHead)
		return NULL;
	if(unsigned int(pTAM_FileHead->LightHeadAddress)==0xFFFFFFFF)
		return NULL;
	if(pTAM_FileHead->LightNum<=0)
		return NULL;
	if(!TAM_Light)
		return NULL;

	unsigned int LightFrameIDTMP=min(TAM_Light->FramesHeadAddress[TAM_Light_Frame_Type]->MAXFrameNum,LightFrameID);
	_TAM_Light_Frame ** TAM_Light_Frames=(_TAM_Light_Frame **) &(TAM_Light->FramesHeadAddress[TAM_Light_Frame_Type]->FramesAddress);
	return TAM_Light_Frames[LightFrameIDTMP];
}
_TAM_Bone * CTopAceModel::TAM_Get_NextBone(unsigned int BoneID)
{
	return TAM_Get_NextBone(TAM_Get_Bone(BoneID));
}
_TAM_Bone * CTopAceModel::TAM_Get_NextBone(_TAM_Bone * TAM_Bone)
{
	if(TAM_Bone)
		return TAM_Get_Bone(TAM_Bone->NextBoneID);
	else
		return NULL;
}

_TAM_Bone * CTopAceModel::TAM_Get_ChildBone(unsigned int BoneID)
{
	return TAM_Get_ChildBone(TAM_Get_Bone(BoneID));
}
_TAM_Bone * CTopAceModel::TAM_Get_ChildBone(_TAM_Bone * TAM_Bone)
{
	if(TAM_Bone)
		return TAM_Get_Bone(TAM_Bone->ChildBoneID);
	else
		return NULL;
}
bool CTopAceModel::LoadToVRAM(void)
{
	if(!pTAM_FileHead)
		return false;
	if(unsigned int(pTAM_FileHead->MeshHeadAddress)==0xFFFFFFFF)
		return false;
	if(pTAM_FileHead->MeshNum<=0)
		return false;
	if (glewIsSupported("GL_ARB_vertex_buffer_object"))
		SuppotVBO=true;
	else
		SuppotVBO=false;
	if(!SuppotVBO)
		return false;

	MeshVBOIDs= new tMeshVBOID[pTAM_FileHead->MeshNum+2];
	for (unsigned int i=0;i<pTAM_FileHead->MeshNum+2;i++)
	{
		MeshVBOIDs[i].FaceID=0;
		MeshVBOIDs[i].NormalID=0;
		MeshVBOIDs[i].TexCoordID=0;
		MeshVBOIDs[i].VerticeID=0;
	}
	for (unsigned int i=0;i<pTAM_FileHead->MeshNum;i++)
		LoadMeshToVRAM(pTAM_FileHead->MeshHeadAddress[i]);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );

	isVRAM=true;
	return true;
}
bool CTopAceModel::LoadMeshToVRAM(_TAM_Mesh * TAM_Mesh)
{

	if(unsigned int(TAM_Mesh->vecBoneWeightsAndBoneIDs)!=0xFFFFFFFF)
		return false;
	if(unsigned int(TAM_Mesh->vertices)==0xFFFFFFFF)
		return false;
	if(TAM_Mesh->vecNum<=0)
		return false;
	tMeshVBOID * MeshVBOID = &MeshVBOIDs[TAM_Mesh->OBJID];
	glGenBuffersARB( 1,&MeshVBOID->VerticeID);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, MeshVBOID->VerticeID );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, TAM_Mesh->vecNum*sizeof(__m128), TAM_Mesh->vertices, GL_STATIC_DRAW_ARB );

	if((TAM_Mesh->Normals)&&(unsigned int(TAM_Mesh->Normals)!=0xFFFFFFFF))
	{
		glGenBuffersARB( 1,&MeshVBOID->NormalID);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, MeshVBOID->NormalID );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, TAM_Mesh->vecNum*sizeof(__m128), TAM_Mesh->Normals, GL_STATIC_DRAW_ARB );
	}

	if((TAM_Mesh->texcos)&&(unsigned int(TAM_Mesh->texcos)!=0xFFFFFFFF))
	{
		glGenBuffersARB( 1,&MeshVBOID->TexCoordID);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, MeshVBOID->TexCoordID );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, TAM_Mesh->vecNum*2*sizeof(float), TAM_Mesh->texcos, GL_STATIC_DRAW_ARB );
	}

	glGenBuffersARB( 1,&MeshVBOID->FaceID);
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, MeshVBOID->FaceID );
	glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, TAM_Mesh->FaceNum*3*sizeof(unsigned int), TAM_Mesh->Faces, GL_STATIC_DRAW_ARB );
	return true;
}
void CTopAceModel::DeleteVRAM()
{
	if(!pTAM_FileHead)
		return ;
	if(unsigned int(pTAM_FileHead->MeshHeadAddress)==0xFFFFFFFF)
		return ;
	if(pTAM_FileHead->MeshNum<=0)
		return ;
	if(!SuppotVBO)
		return ;
	if(!MeshVBOIDs)
		return ;
	if(!isVRAM)
		return ;
	//for (unsigned int i=0;i<pTAM_FileHead->MeshNum;i++)
		//DeleteMeshVRAM(pTAM_FileHead->MeshHeadAddress[i]);
	tMeshVBOID * MeshVBOID = NULL;
	for (unsigned int i=0;i<pTAM_FileHead->MeshNum+2;i++)
	{
		MeshVBOID = &MeshVBOIDs[i];
		if(MeshVBOID->VerticeID)
			glDeleteBuffersARB(1,&MeshVBOID->VerticeID);
		if(MeshVBOID->NormalID)
			glDeleteBuffersARB(1,&MeshVBOID->NormalID);
		if(MeshVBOID->TexCoordID)
			glDeleteBuffersARB(1,&MeshVBOID->TexCoordID);
		if(MeshVBOID->FaceID)
			glDeleteBuffersARB(1,&MeshVBOID->FaceID);
	}
	if(MeshVBOIDs)
		delete[] MeshVBOIDs;
	MeshVBOIDs=NULL;
	isVRAM=false;
}
void CTopAceModel::DeleteMeshVRAM(_TAM_Mesh * TAM_Mesh)
{
	tMeshVBOID * MeshVBOID = &MeshVBOIDs[TAM_Mesh->OBJID];
	if(MeshVBOID->VerticeID)
		glDeleteBuffersARB(1,&MeshVBOID->VerticeID);
	if(MeshVBOID->NormalID)
		glDeleteBuffersARB(1,&MeshVBOID->NormalID);
	if(MeshVBOID->TexCoordID)
		glDeleteBuffersARB(1,&MeshVBOID->TexCoordID);
	if(MeshVBOID->FaceID)
		glDeleteBuffersARB(1,&MeshVBOID->FaceID);

}
void CTopAceModel::Draw(void)
{
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	if(!pTAM_FileHead)
		return ;
	if(unsigned int(pTAM_FileHead->MeshHeadAddress)==0xFFFFFFFF)
		return ;
	_TAM_Mesh * TAM_Mesh_Draw=NULL;
	for(unsigned int i=0;i<pTAM_FileHead->MeshNum;i++)
	{
		TAM_Mesh_Draw=pTAM_FileHead->MeshHeadAddress[i];
		if(TAM_Mesh_Draw)
		{
			if(unsigned int(TAM_Mesh_Draw->vecBoneWeightsAndBoneIDs)!=0xFFFFFFFF)
			{
				DrawRAMMeshFiexible(TAM_Mesh_Draw);
			}
			else
			{
				if(SuppotVBO)
					DrawMeshRigid(TAM_Mesh_Draw);
				else
					DrawRAMMeshFiexible(TAM_Mesh_Draw);
			}
		}
	}

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
	glEnable(GL_CULL_FACE);
}
void CTopAceModel::DrawRAM(void)
{
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	if(!pTAM_FileHead)
		return ;
	if(unsigned int(pTAM_FileHead->MeshHeadAddress)==0xFFFFFFFF)
		return ;
	_TAM_Mesh * TAM_Mesh_Draw=NULL;
	for(unsigned int i=0;i<pTAM_FileHead->MeshNum;i++)
	{
		TAM_Mesh_Draw=pTAM_FileHead->MeshHeadAddress[i];
		if(TAM_Mesh_Draw)
		{
			if(TAM_Mesh_Draw->BoneWeights[0]==1.0f)
				DrawRAMMeshRigid(TAM_Mesh_Draw);
			else
				DrawRAMMeshFiexible(TAM_Mesh_Draw);
		}
	}
	glEnable(GL_CULL_FACE);
}
bool CTopAceModel::DrawMeshRigid(_TAM_Mesh * TAM_Mesh)
{
	if(TAM_Mesh->vecNum==0)
		return false;
	tMeshVBOID * MeshVBOID = &MeshVBOIDs[TAM_Mesh->OBJID];
	glBindTexture(GL_TEXTURE_2D, 1);

	glEnableClientState( GL_VERTEX_ARRAY );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, MeshVBOID->VerticeID );
	glVertexPointer( 3, GL_FLOAT, 16, 0 );

	if((TAM_Mesh->Normals)&&(unsigned int(TAM_Mesh->Normals)!=0xFFFFFFFF))
	{
		glEnableClientState( GL_NORMAL_ARRAY );
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, MeshVBOID->NormalID );
		glNormalPointer( GL_FLOAT, 16, 0 );
	}
	if((TAM_Mesh->texcos)&&(unsigned int(TAM_Mesh->texcos)!=0xFFFFFFFF))
	{
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, MeshVBOID->TexCoordID );
		glTexCoordPointer( 2, GL_FLOAT, 0, 0 );
	}

	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, MeshVBOID->FaceID );
	glDrawElements(GL_TRIANGLES,TAM_Mesh->FaceNum*3,GL_UNSIGNED_INT,0);

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	return true;
}
bool CTopAceModel::DrawRAMMeshRigid(_TAM_Mesh * TAM_Mesh)
{
	if(TAM_Mesh->vecNum==0)
		return false;
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBindTexture(GL_TEXTURE_2D, 1);
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 16, (float*)&(TAM_Mesh->vertices[0].m128_f32[0]) );
	if((TAM_Mesh->Normals)&&(unsigned int(TAM_Mesh->Normals)!=0xFFFFFFFF))
	{
		glEnableClientState( GL_NORMAL_ARRAY );
		glNormalPointer( GL_FLOAT, 16, (float*)&(TAM_Mesh->Normals[0].m128_f32[0]) );
	}
	if((TAM_Mesh->texcos)&&(unsigned int(TAM_Mesh->texcos)!=0xFFFFFFFF))
	{
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer( 2, GL_FLOAT, 0, TAM_Mesh->texcos );
	}
	glDrawElements(GL_TRIANGLES,TAM_Mesh->FaceNum*3,GL_UNSIGNED_INT,(GLvoid*)&(TAM_Mesh->Faces[0]));
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	//if()GL_TRIANGLES

	return true;
}

bool CTopAceModel::DrawRAMMeshFiexible(_TAM_Mesh * TAM_Mesh)
{
	return true;
}
