#include "LightSource.h"

LightSource::LightSource()
{
	counter = 0;
	m_positionVector = glm::vec3(0.0, 0.0, 0.0);
}

LightSource::~LightSource()
{

}

void LightSource::addLight(const glm::vec3 pos, const glm::vec3 nor, const glm::vec3 col, GLfloat brightness, LightType typ)
{
	if(counter == 7)
	{
		std::cout << "you can not add more than 8 lightsources" << std::endl;
	}

	int index = (counter*12);
	lightSourceArray[0+index] = pos[0];
	lightSourceArray[1+index] = pos[1];
	lightSourceArray[2+index] = pos[2];
	lightSourceArray[3+index] = nor[0];
	lightSourceArray[4+index] = nor[1];
	lightSourceArray[5+index] = nor[2];
	lightSourceArray[6+index] = col[0];
	lightSourceArray[7+index] = col[1];
	lightSourceArray[8+index] = col[2];
	lightSourceArray[9+index] = brightness;
	lightSourceArray[10+index] = typ;
	lightSourceArray[11+index] = 0;

	counter++;
}

void LightSource::updatePosition(float *array, int ls)
{
	array[ls*12] += m_positionVector[0];
	array[(ls*12) + 1] += m_positionVector[1];
	array[(ls*12) + 2] += m_positionVector[2];

	m_positionVector[0] = 0.0;
	m_positionVector[1] = 0.0;
	m_positionVector[2] = 0.0;
}

void LightSource::updatePositionVector(glm::vec3 vector)
{
	m_positionVector += vector;
}

void LightSource::updateDirection(float *array, int ls)
{
	array[ls*12] += m_directionVector[0];
	array[(ls*12) + 1] += m_directionVector[1];
	array[(ls*12) + 2] += m_directionVector[2];

	m_directionVector[0] = 0.0;
	m_directionVector[1] = 0.0;
	m_directionVector[2] = 0.0;
}

void LightSource::updateDirectionVector(glm::vec3 vector)
{
	m_directionVector += vector;
}

int LightSource::getLightArraySize()
{
	return sizeof(lightSourceArray);
}

int LightSource::getCounter()
{
	return counter + 1;
}

void LightSource::getLightArray(float *array)
{
	for(int i = 0; i < 96; i++)
	{
		array[i] = lightSourceArray[i]; 
	}
	
}