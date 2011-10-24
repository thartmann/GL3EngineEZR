#version 330

// Input vertex data, different for all executions of this shader.
layout(location = 3) in vec3 vertexPosition;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main(){
	gl_Position =  vec4(vertexPosition, 1.0);
	UV = (vertexPosition.xy) * 0.5 + vec2(0.5, 0.5);
}
