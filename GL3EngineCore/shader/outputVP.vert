#version 330

// Input vertex data, different for all executions of this shader.
layout(location = 3) in vec3 vertexPosition_modelspace;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
//out vec4 gl_Position;

void main(){
	gl_Position =  vec4(vertexPosition_modelspace, 1.0f);
	UV = (vertexPosition_modelspace.xy) * 0.5f + vec2(0.5f, 0.5f);
}
