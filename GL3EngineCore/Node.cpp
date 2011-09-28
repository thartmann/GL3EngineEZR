#include "Node.h"
#include "common.h"

Node::Node(const std::string name)
{
	m_nodeName = name;
	transform_local = glm::mat4(1.0);
	transform_global = glm::mat4(1.0);
	m_Mesh = 0;
	m_Texture = 0;
}

Node::~Node()
{
	for(GLuint i = 0; i < m_children.size(); i++)
	{
		delete m_children[i];
		removeChild(m_children[i]);
	}
}

bool Node::isRootNode()
{
	return m_isRoot;
}

std::string Node::getNodeName()
{
	return m_nodeName;
}

Mesh* Node::getMesh()
{
	return m_Mesh;
}

void Node::addChild(Node *child)
{
	m_children.push_back(child);
}

void Node::removeChild(Node *child)
{
	for(GLuint i = 0; i < m_children.size(); i++)
	{
		if(!child->getNodeName().compare(m_children[i]->getNodeName()))
		{
			m_children.erase(m_children.begin()+i);
		}
	}
}

Node *Node::getChildByName(const std::string name)
{
	for(GLuint i = 0; i < m_children.size(); i++)
	{
		if(!name.compare(m_children[i]->getNodeName()))
		{
			return m_children[i];
		}
	}
	return 0;
}

Node *Node::getParent()
{
	return m_parent;
}

void Node::nodeAttach(Mesh* mesh)
{
	if(!m_Mesh)m_Mesh = mesh;
}

void Node::nodeAttach(Texture* texture)
{
	if(!m_Texture)m_Texture = texture;
}

void Node::nodeMove(glm::vec3 const &translation, GLuint tspace)
{
	if(tspace == TS_LOCAL)
	{
		transform_local = glm::translate(transform_local, translation);
	}
	if(tspace == TS_GLOBAL)
	{
		glm::vec4 tvec = glm::vec4(translation,.0f);
		tvec = glm::affineInverse(transform_global) * tvec;
		transform_local = glm::translate(transform_local, glm::vec3(tvec.x,tvec.y, tvec.z));
	}

}
void Node::nodeRotate(GLfloat angle, glm::vec3 const &axis, GLuint tspace)
{
	if(tspace == TS_LOCAL)
	{
		transform_local = glm::rotate(transform_local, angle, axis);
	}
	if(tspace == TS_GLOBAL)
	{
		glm::vec4 tvec = glm::vec4(axis,.0f);
		tvec = glm::affineInverse(transform_global) * tvec;
		transform_local = glm::rotate(transform_local, angle, glm::vec3(tvec.x,tvec.y, tvec.z));
	}
}

void Node::Update()
{

}

void Node::Render(GLint texLoc, GLint matLoc)
{
	transform_global = transform_local;

	if (m_Mesh != 0 && m_Texture != 0)
	{
		//m_Shader->BindShader();
		glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::gtc::type_ptr::value_ptr(transform_global));

		glUniform1i(texLoc, m_Texture->getUnit());
		CheckOpenGLError("passing uniform in node");

		glActiveTexture(GL_TEXTURE0 + m_Texture->getUnit());
		glBindTexture(GL_TEXTURE_2D, m_Texture->getID());
		glBindSampler(m_Texture->getUnit(), m_Texture->getSampler());
		CheckOpenGLError("binding texture in node");

		m_Mesh->draw();

		glBindTexture(GL_TEXTURE_2D, 0);

		//m_Shader->UnBindShader();
	}
	for (GLuint i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Render(texLoc, matLoc, transform_global);
		CheckOpenGLError("render children in node");
	}

}

void Node::Render(GLint texLoc, GLint matLoc, glm::mat4 const &transform)
{
	transform_global = transform * transform_local;

	if (m_Mesh != 0 && m_Texture != 0)
	{
		//m_Shader->BindShader();
		glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(transform_global));		

		glUniform1i(texLoc, m_Texture->getUnit());
		CheckOpenGLError("passing uniform in node");

		glActiveTexture(GL_TEXTURE0 + m_Texture->getUnit());
		glBindTexture(GL_TEXTURE_2D, m_Texture->getID());

		m_Mesh->draw();

		glBindTexture(GL_TEXTURE_2D, 0);

		//m_Shader->UnBindShader();
	}
}
