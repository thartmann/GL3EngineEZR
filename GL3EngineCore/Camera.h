#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void camPerspective(GLfloat fovy, GLfloat aspect, GLfloat znear, GLfloat zfar);
	void camLookAt(glm::vec3 const & eye, glm::vec3 const & center, glm::vec3 const & up);
	void camMove(glm::vec3 const & translation, GLuint tspace = TS_GLOBAL);
	void camRotate(GLfloat angle, glm::vec3 axis, GLuint tspace = TS_GLOBAL);

	glm::mat4 getProjection();
	glm::mat4 getView();

	float getFar();
	
private:
	glm::mat4 m_transform;
	glm::mat4 m_projection;

	float m_far;
	
};

#endif