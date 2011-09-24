#ifndef MESH_H
#define MESH_H

#include "common.h"

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	std::string name;
	GLuint index;
	std::vector<GLuint> indices;

	//MinX, MinY, MinZ, MaxX, MaxY, MaxZ
	GLfloat AABB [6];

	void setDrawType(GLint drawType);
	void draw();
	void init();
	void initBuffer(std::string name, std::string linkage, GLuint vertexSize);

	void loadFromFile(const std::string filename);

private:

	GLuint m_vboVerticesID, m_vboTexCoordID, m_vboIndicesID, m_vboNormalsID, m_vaoID;
	GLint m_DrawType;

	GLuint m_IndexSize;
	GLuint m_BufferCnt;

	GLuint m_vsize;
	GLfloat* m_vpos;
	GLfloat* m_vnorm;

	GLuint m_vuvsize;
	GLfloat* m_vuvc;

	GLuint m_isize;
	GLuint* m_ind;

	void addBuffer(const std::vector<GLfloat> &content, GLuint size, std::string name);
	void addElementBuffer(const std::vector<GLuint> &content);
};

#endif //MESH_H
