#include "stdafx.h"
#include "sceneLoader.h"
	
void meshLoader::recursiveProcess(aiNode* node,const aiScene* scene)
{
	//process
	for(int i=0;i<node->mNumMeshes;i++)
	{
		aiMesh* mesh=scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh,scene);
	}
	
	
	
	//recursion
	for(int i=0;i<node->mNumChildren;i++)
	{
		recursiveProcess(node->mChildren[i],scene);
	}
}

void meshLoader::processMesh(aiMesh* monMesh,const aiScene* scene)
{
	std::vector<vertexData> data;
	std::vector<unsigned int> indices;
	std::vector<textureData> textures; 
	aiColor4D col;
	aiMaterial* mat = scene->mMaterials[monMesh->mMaterialIndex];
	aiGetMaterialColor(mat,AI_MATKEY_COLOR_DIFFUSE,&col);
	vector3d defaultColor(col.r,col.g,col.b);
	
		
	for (int i = 0; i<monMesh->mNumVertices; i++)
	{
			vertexData tmp;
			vector3d tmpVec;
			
			//position
			tmpVec.x = monMesh->mVertices[i].x;
			tmpVec.y = monMesh->mVertices[i].y;
			tmpVec.z = monMesh->mVertices[i].z;
			tmp.position=tmpVec;
			
			//normals
			tmpVec.x = monMesh->mNormals[i].x;
			tmpVec.y = monMesh->mNormals[i].y;
			tmpVec.z = monMesh->mNormals[i].z;
			tmp.normal=tmpVec;			
			
			
			//tangent
			if (monMesh->mTangents)
			{
				tmpVec.x = monMesh->mTangents[i].x;
				tmpVec.y = monMesh->mTangents[i].y;
				tmpVec.z = monMesh->mTangents[i].z;
			}else{
				tmpVec.x=1.0;
				tmpVec.y=tmpVec.z=0;
			}
			tmp.tangent=tmpVec;		
			
			
			//colors
			if (monMesh->mColors[0])
			{
				//!= material color
				tmpVec.x = monMesh->mColors[0][i].r;
				tmpVec.y = monMesh->mColors[0][i].g;
				tmpVec.z = monMesh->mColors[0][i].b;
			}else{
				tmpVec=defaultColor;
			}
			tmp.color=tmpVec;
			
			//color
			if (monMesh->mTextureCoords[0])
			{
				tmpVec.x = monMesh->mTextureCoords[0][i].x;
				tmpVec.y = monMesh->mTextureCoords[0][i].y;
			}else{
				tmpVec.x=tmpVec.y=tmpVec.z=0.0;
			}
			tmp.U=tmpVec.x;
			tmp.V=tmpVec.y;
			data.push_back(tmp);
	}
	
	for (int i = 0; i<monMesh->mNumFaces; i++)
	{
		aiFace face = monMesh->mFaces[i];
		for(int j=0;j<face.mNumIndices;j++) //0..2
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	
	
	for(int i=0;i<mat->GetTextureCount(aiTextureType_DIFFUSE);i++)
	{
		aiString str;
		mat->GetTexture(aiTextureType_DIFFUSE,i,&str);
		textureData tmp;
		const char* a=str.C_Str();
		tmp.id=loadTexture(str.C_Str());
		tmp.type=0;
		textures.push_back(tmp);
	}

	meshes.push_back(new mesh(&data,&indices,&textures));	
}

unsigned int meshLoader::loadTexture(const char* filename)
{
	unsigned int num;
	glGenTextures(1,&num);
	SDL_Surface* img=IMG_Load(filename);
	if(img==NULL)
	{
		//std::cout << "img was not loaded" << std::endl;
		return -1;
	}
	SDL_PixelFormat form={NULL,32,4,0,0,0,0,0,0,0,0,0xff000000,0x00ff0000,0x0000ff00,0x000000ff,0,255};
	SDL_Surface* img2=SDL_ConvertSurface(img,&form,SDL_SWSURFACE);
	if(img2==NULL)
	{
		//std::cout << "img2 was not loaded" << std::endl;
		return -1;		
	}
	glBindTexture(GL_TEXTURE_2D,num);		
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img2->w,img2->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);		
	SDL_FreeSurface(img);
	SDL_FreeSurface(img2);
	return num;	
}


meshLoader::meshLoader(const char* filename)
{
	Assimp::Importer importer;
	const aiScene* scene=importer.ReadFile(filename, aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);
	if(scene->mFlags==AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "The file wasn't successfuly opened " << filename << std::endl;
		return; 
	}
	
	recursiveProcess(scene->mRootNode,scene);
}

meshLoader::~meshLoader()
{
	for(int i=0;i<meshes.size();i++)
		delete meshes[i];
}

void meshLoader::draw(unsigned int programId)
{
	for(int i=0;i<meshes.size();i++)
		meshes[i]->draw(programId);	
}

std::vector<mesh*>& meshLoader::getMeshes()
{
	return meshes;
}
