#include "Camera.h"

Camera::Camera()
{
	m_far = 100.0;
	m_transform = glm::core::type::mat4(1.0f);
	m_projection = glm::gtc::matrix_transform::perspective(45.0f, 1.0f, 0.1f, m_far);
}

Camera::~Camera(){

}

void Camera::camPerspective(GLfloat fovy, GLfloat aspect, GLfloat znear, GLfloat zfar){
	m_projection = glm::gtc::matrix_transform::perspective(fovy, aspect, znear, zfar);
}

void Camera::camLookAt(glm::vec3 const & eye, glm::vec3 const & center, glm::vec3 const & up){
	m_transform = glm::lookAt(eye,center,up);
}

void Camera::camMove(glm::vec3 const & translation, GLuint tspace)
{
	if(tspace == TS_GLOBAL)
	{
		m_transform = glm::translate(m_transform, translation);
	}
	if(tspace == TS_LOCAL)
	{
		glm::vec4 tvec = glm::vec4(translation,.0f);
		tvec = glm::affineInverse(m_transform) * tvec;
		m_transform = glm::translate(m_transform, glm::vec3(tvec.x,tvec.y, tvec.z));
	}
}

void Camera::camRotate(GLfloat angle, glm::vec3 axis, GLuint tspace)
{
	//translate to origin
	glm::vec4 tvec = glm::vec4(m_transform[3][0],m_transform[3][1],m_transform[3][2],.0f);
	tvec = glm::affineInverse(m_transform) * tvec;
	m_transform = glm::translate(m_transform, glm::vec3(-tvec.x,-tvec.y, -tvec.z));

	//rotate
	if(tspace == TS_GLOBAL)
	{
		m_transform = glm::rotate(m_transform, angle, axis);
	}
	if(tspace == TS_LOCAL)
	{
		glm::vec4 rvec = glm::vec4(axis,.0f);
		rvec = glm::affineInverse(m_transform) * rvec;
		m_transform = glm::rotate(m_transform, angle, glm::vec3(rvec.x,rvec.y, rvec.z));
	}

	//translate back
	m_transform = glm::translate(m_transform, glm::vec3(tvec.x,tvec.y, tvec.z));

}

glm::mat4 Camera::getProjection()
{
	return m_projection;
}

glm::mat4 Camera::getView()
{
	return m_transform;
}


float Camera::getFar()
{
	return m_far;
}
