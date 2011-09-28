#version 330

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 vertPosition;
in vec3 vertNormals;
in vec4 SM_tex_coord;

// FragmentColor
layout(location = 0 ) out vec3 vFragColor;
layout(location = 1 ) out vec3 vWorldCoord;
layout(location = 2 ) out vec3 vNormals;
layout(location = 3 ) out vec3 vFlux;

uniform sampler2D shadowMap;
uniform sampler2D assignedtexture;
uniform vec3 m_lightArray[8];

vec3 spotDir, N, V, L;
float directionalLambertTerm, spotLambertTerm, M, K;

void main(void)
{
//directional lighting
	V = normalize(m_lightArray[1]);
	N = normalize(vertNormals);
	directionalLambertTerm = max(dot(N,V), 0.0 );
	//vFragColor = (texture2D( assignedtexture, UV ).rgb ) * directionalLambertTerm;

//spot lighting
	spotDir = m_lightArray[4] - vertPosition;
	L = normalize(spotDir);
	M = max(dot(L , m_lightArray[5]), 0.0);
	K = max(dot(N , L ), 0.0 );
	spotLambertTerm = smoothstep( 0.95, 1.0, M );
	

	//vFragColor = vec3(0.0f, 1.0f, 0.0f);
	vWorldCoord = vertPosition;
	//vWorldCoord = vec3(1.0f, 0.0f, 1.0f);
	vNormals = vertNormals;
	vFlux = vec3( 0.0, 0.0, 1.0 );

	// note the perspective division!
	vec3 tex_coords = SM_tex_coord.xyz/SM_tex_coord.w;
	// read depth value from shadow map
	float depth = texture2D(shadowMap, tex_coords.xy).r;
	float z_diff = tex_coords.z -(depth + 0.007f);
	bool inShadow = ((depth + 0.007f) < (tex_coords.z)) ? true : false;
	if(tex_coords.x > 1.0f || tex_coords.x < 0.0f) inShadow = true;
	if(tex_coords.y > 1.0f || tex_coords.y < 0.0f) inShadow = true;

	vFragColor = texture2D( assignedtexture, UV ).rgb * spotLambertTerm * K * m_lightArray[7].x;
	if(inShadow) vFragColor *= min(z_diff , 1.0f);
}