#include "Mesh.h"

#include "assimp/assimp.hpp"
#include "assimp/aiScene.h"
#include "assimp/aiPostProcess.h"

#include <cassert>

Mesh::Mesh()
{
	index = 0;
	m_DrawType = GL_POINTS;
	m_BufferCnt = 0;
	m_IndexSize = 0;
	indices = std::vector<GLuint>();

	m_vpos = 0;
	m_vsize = 0;
}

Mesh::~Mesh()
{
	glBindVertexArray(m_vaoID);
	/*for (GLuint i = 0; i < m_BufferCnt; i++) {
    glDisableVertexAttribArray(i);
  }*/
    glDeleteVertexArrays(1, &m_vaoID);
	delete[] m_vpos;
}

void Mesh::setDrawType(GLint drawType){}

void Mesh::draw()
{
	glBindVertexArray(m_vaoID);
    //glDrawElements(m_DrawType, m_IndexSize, GL_UNSIGNED_INT, 0);
	//glDrawArrays(m_DrawType, 0, buffers[VERTEX_POSITION].size()/3);
	//glDrawArrays(m_DrawType, 0, m_vsize);
	glDrawElements(GL_TRIANGLES, m_isize,GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
}

void Mesh::init(){
	// Allocate and assign a Vertex Array Object to our handle
	glGenVertexArrays(1, &m_vaoID);
	std::cout << "Generating Vertex Array Object #" << m_vaoID << std::endl;

	//Bind our Vertex Array Object as the current used object
	glBindVertexArray(m_vaoID);

	glGenBuffers(1, &m_vboVerticesID);
	/* Bind the first VBO as being the active buffer
	* and storing vertex attributes (coordinates) */
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVerticesID);
	glBufferData(GL_ARRAY_BUFFER, m_vsize * sizeof(GLfloat), m_vpos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	assert(glGetError()== GL_NO_ERROR);

	glGenBuffers(1, &m_vboTexCoordID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboTexCoordID);
	glBufferData(GL_ARRAY_BUFFER, m_vuvsize * sizeof(GLfloat), m_vuvc, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &m_vboNormalsID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboNormalsID);
	glBufferData(GL_ARRAY_BUFFER, m_vsize * sizeof(GLfloat), m_vnorm, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	assert(glGetError()== GL_NO_ERROR);

	glGenBuffers(1, &m_vboIndicesID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_isize * sizeof(GLfloat), m_ind, GL_STATIC_DRAW);

	assert(glGetError()== GL_NO_ERROR);
	glBindVertexArray(0);
}

void Mesh::initBuffer(std::string name, std::string linkage, GLuint vertexSize){}

void Mesh::loadFromFile(const std::string filename)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( filename, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate );
	if(!scene)
	{
		std::cout << "ERROR: " << importer.GetErrorString() << std::endl;
		return;
	}

	// Now we can access the file's contents.
	const aiMesh* mesh = scene->mMeshes[0];
	std::cout << "LOADING:" << mesh->mName.data << std::endl;

	GLuint cvcnt = 0;

	// load vertices
	m_vsize =  mesh->mNumVertices * 3;
	m_vpos = new GLfloat [m_vsize];
	for(GLuint v = 0; v < mesh->mNumVertices; v++)
	{
		m_vpos[cvcnt] = mesh->mVertices[v].x;
		m_vpos[cvcnt+1] = mesh->mVertices[v].y;
		m_vpos[cvcnt+2] = mesh->mVertices[v].z;
		cvcnt += 3;
	}

	// load vertex normals
	cvcnt = 0;
	m_vnorm = new GLfloat [m_vsize];
	for(GLuint n = 0; n < mesh->mNumVertices; n++)
	{
		m_vnorm[cvcnt] = mesh->mNormals[n].x;
		m_vnorm[cvcnt+1] = mesh->mNormals[n].y;
		m_vnorm[cvcnt+2] = mesh->mNormals[n].z;
		cvcnt += 3;
	}

	//load uv coords
	cvcnt = 0;
	m_vuvsize = mesh->mNumVertices * 2;
	m_vuvc = new GLfloat [m_vuvsize];
	for(GLuint u = 0; u < mesh->mNumVertices; u++)
	{
		m_vuvc[cvcnt] = mesh->mTextureCoords[0][u].x;
		m_vuvc[cvcnt+1] = mesh->mTextureCoords[0][u].y;
		cvcnt += 2;
	}

	// load indices
	cvcnt = 0;
	m_isize = mesh->mNumFaces * 3;
	m_ind = new GLuint [m_isize];
	for(GLuint f = 0; f < mesh->mNumFaces; f++)
	{
		for(GLuint i = 0; i < 3; i++)
		{
			m_ind[cvcnt+i] = mesh->mFaces[f].mIndices[i];
		}
		cvcnt += 3;
	}
}
