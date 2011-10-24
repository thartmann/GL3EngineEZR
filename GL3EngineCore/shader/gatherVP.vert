#version 330
// Vertex position in worldspace
layout(location = 0) in vec3 vVertex;

// UV texture coordinates
layout(location = 1) in vec2 vUV;

// Vertexnormal direction in worldspace
layout(location = 2) in vec3 vNormal;

// Output data will be interpolated for each fragment.
out vec2 UV;
out vec4 vertPosition;
out vec3 vertNormals;
out vec4 SM_tex_coord;
out float vertDepth;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 LightSpaceMatrix;

uniform mat3 normalMat;

uniform float farPlane;

void main()
{
   gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vVertex, 1.0);
   vertPosition = (ModelMatrix * vec4(vVertex, 1.0));
   vertNormals = normalMat * vNormal;
   vertDepth = gl_Position.z * gl_Position.w / farPlane;
   UV = vUV;
   SM_tex_coord = LightSpaceMatrix * ModelMatrix * vec4(vVertex, 1.0);
}