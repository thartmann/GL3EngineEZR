#ifndef HEADER_18A121ED327AD0B4
#define HEADER_18A121ED327AD0B4

#pragma once

#include "common.h"
#include "Mesh.h"
#include "Texture.h"
#include "shader.h"


/*!
 *\brief    scenegraph node object
 *
 *\author   Dominik Ospelt, Hans-Christian Wollert, Tobias Hartmann
 *\date     22-08-2011
 */

class Node
{
public:

	Node(const std::string name);
	virtual ~Node();

	bool isRootNode();
	std::string getNodeName();
	Mesh* getMesh();
	void addChild(Node *child);
	void removeChild(Node *child);
	Node *getChildByName(const std::string name);
	Node *getParent();

	void nodeAttach(Mesh* mesh);
	void nodeAttach(Texture* texture);

	void nodeMove(glm::vec3 const &translation, GLuint tspace = TS_GLOBAL);
	void nodeRotate(GLfloat angle, glm::vec3 const &axis, GLuint tspace = TS_GLOBAL);

	void Update();
	void Render(GLint texLoc, GLint matLoc);
	void Render(GLint texLoc, GLint matLoc, glm::mat4 const &transform);

	//all transformations of this node should be represented by this matrix
	glm::core::type::mat4 transform_local;
	glm::core::type::mat4 transform_global;

	protected:

	private:

		bool m_isRoot;
		std::string m_nodeName;
		Node *m_parent;
		std::vector<Node*> m_children;
		Mesh *m_Mesh;
		Texture *m_Texture;
};

#endif // header guard 
