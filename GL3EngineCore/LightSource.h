#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include "common.h"

enum LightType
{
	LT_DIRECTIONAL = 0,
	LT_POINT = 1,
	LT_SPOT = 2,
	LT_AREA = 3
};

class LightSource
{
public:
	LightSource();
	LightSource(const glm::vec3 pos, const glm::vec3 nor, const glm::vec3 col, GLfloat brightness, LightType typ);
	virtual ~LightSource();

	

private:
	glm::vec3 m_Position;
	glm::vec3 m_Normal;
	glm::vec3 m_Color;
	GLfloat m_Brightness;
	LightType m_Type;
};

#endif
