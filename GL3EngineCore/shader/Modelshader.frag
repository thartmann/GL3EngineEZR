#version 330
//smooth in vec4 vSmoothColor;

// Interpolated values from the vertex shaders
in vec2 UV;

// FragmentColor
out vec3 vFragColor;

// Values that stay constant for the whole mesh.
uniform sampler2D assignedtexture;

void main(void)
{
   vFragColor = texture2D( assignedtexture, UV ).rgb;
}