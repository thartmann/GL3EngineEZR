#version 330

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec4 in_Color;

out vec4 vert_Color;

void main()
{
	gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * in_Position;
    vert_Color = in_Color;
}
