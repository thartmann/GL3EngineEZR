#version 330

// Interpolated values from the vertex shaders
in vec2 UV;
in vec4 vertPosition;
in vec4 vertNormals;

// FragmentColor
layout(location = 0 ) out vec4 vFragColor;
layout(location = 1 ) out vec4 vViewCoord;
layout(location = 2 ) out vec4 vViewNormals;
layout(location = 3 ) out vec4 vFlux;

uniform sampler2D meshtexture;
//uniform vec3 megaLightArray[8];

void main(void)
{
	vFragColor = texture(meshtexture, UV);
	vViewCoord = vertPosition;
	vViewNormals = vertNormals;
	vFlux = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}