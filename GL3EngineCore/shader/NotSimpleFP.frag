#version 330

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 vertPosition;
in vec3 vertNormals;

// FragmentColor
layout(location = 0 ) out vec3 vFragColor;
layout(location = 1 ) out vec3 vWorldCoord;
layout(location = 2 ) out vec3 vNormals;
layout(location = 3 ) out vec3 vFlux;

uniform sampler2D meshtexture;
//uniform vec3 megaLightArray[8];

void main(void)
{
	vFragColor = texture(meshtexture, UV).rgb;
	//vFragColor = vec3(0.0f, 1.0f, 0.0f);
	vWorldCoord = vertPosition;
	//vWorldCoord = vec3(1.0f, 0.0f, 1.0f);
	vNormals = vertNormals;
	vFlux = vec3(0.0f,1.0f,0.0f);
}