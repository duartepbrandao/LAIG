#include "Object.h"
#include <iostream>
#include <GL/glut.h>
#include <glm/glm.hpp>


Object::Object(std::string filename){

	scene = importer.ReadFile(filename, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	if(!scene){
		std::cout << importer.GetErrorString();
	}

}

void Object::draw(){

	if(!scene)
		return;

	if(!scene->mNumMeshes){
		std::cout << "NO PRIMITIVES" << std::endl;
		return;
	}


	glm::vec3 AB;
	glm::vec3 AC;
	glm::vec3 Normal;

	glm::vec3 A, B, C;

	
	for(int i = 0; i < scene->mNumMeshes ; i++){
		
		for (int j = 0; j < scene->mMeshes[i]->mNumFaces ; j++)
		{



			/*switch(face->mNumIndices)
            {
                case 1: face_mode = GL_POINTS; break;
                case 2: face_mode = GL_LINES; break;
                case 3: face_mode = GL_TRIANGLES; break;
                default: face_mode = GL_POLYGON; break;
            }

            glBegin(face_mode);

            for(i = 0; i < face->mNumIndices; i++)// go through all vertices in face
            {
                 int vertexIndex = face->mIndices[i];// get group index for current index
                 if(mesh->mColors[0] != NULL)
                     Color4f(&mesh->mColors[0][vertexIndex]);
                 if(mesh->mNormals != NULL)
                     if(mesh->HasTextureCoords(0))//HasTextureCoords(texture_coordinates_set)
                     {
                         glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1 - mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]
                     }
                     glNormal3fv(&mesh->mNormals[vertexIndex].x);
                     glVertex3fv(&mesh->mVertices[vertexIndex].x);
             }
             glEnd();*/
			
			A = glm::vec3(scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[0]].x, scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[0]].y, scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[0]].z);
			B = glm::vec3(scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[1]].x, scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[1]].y, scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[1]].z);
			C = glm::vec3(scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[2]].x, scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[2]].y, scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[2]].z);
			
			AB = B - A;
			AC = C - A;

			Normal = glm::cross(AB, AC);

			

			glBegin(GL_TRIANGLES);
			glNormal3f(scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[0]].x,
				scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[0]].y,
				scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[0]].z);
			glVertex3f(A.x,A.y,A.z);


			glNormal3f(scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[1]].x,
				scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[1]].y,
				scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[1]].z);
			glVertex3f(B.x, B.y, B.z);


			glNormal3f(scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[2]].x,
				scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[2]].y,
				scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[2]].z);
			glVertex3f(C.x, C.y, C.z);

			glEnd();
		}
		
	}
	
}

const aiScene* Object::getScene(){
	return scene;
}

Object::~Object(){

}


