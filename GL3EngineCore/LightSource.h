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
	virtual ~LightSource();

	void addLight(const glm::vec3 pos, const glm::vec3 nor, const glm::vec3 col, GLfloat brightness, LightType typ);

	int getLightArraySize();
	void getLightArray(float *array);
	int getCounter();

	void updatePosition(float *array, int ls);
	void updateDirection(float *array, int ls);
	void updatePositionVector(glm::vec3 vector);
	void updateDirectionVector(glm::vec3 vector);
	

private:
	glm::vec3 m_Position;
	glm::vec3 m_Normal;
	glm::vec3 m_Color;
	GLfloat m_Brightness;
	LightType m_Type;

	glm::vec3 m_positionVector;
	glm::vec3 m_directionVector;

	int counter;
	float lightSourceArray[96];
};

#endif
