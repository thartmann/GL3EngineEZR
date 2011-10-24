#version 330

// Interpolated values from the vertex shaders
in vec2 UV;
in vec4 vertPosition;
in vec3 vertNormals;
in vec4 SM_tex_coord;
in float vertDepth;

// FragmentColor
layout(location = 0 ) out vec4 vFragColor;
layout(location = 1 ) out vec4 vViewCoord;
layout(location = 2 ) out vec4 vViewNormals;
layout(location = 3 ) out vec4 vFlux;
layout(location = 4 ) out vec4 vDepth;

uniform sampler2D shadowMap;
uniform sampler2D assignedtexture;
uniform vec3 m_lightArray[8];

vec3 spotDir, N, V, L;
float directionalLambertTerm, spotLambertTerm, M, K;

void main(void)
{
//directional lighting
	V = normalize(m_lightArray[1]);
	N = normalize(vertNormals.xyz);
	directionalLambertTerm = max(dot(N,V), 0.0 );
	//vFragColor = (texture2D( assignedtexture, UV ).rgb ) * directionalLambertTerm;

//spot lighting
	spotDir = m_lightArray[4] - vertPosition.xyz;
	L = normalize(spotDir);
	M = max(dot(L , m_lightArray[5]), 0.0);
	K = max(dot(N , L ), 0.0 );
	spotLambertTerm = smoothstep( 0.95, 1.0, M );
	

	vViewCoord = vertPosition;
	vViewNormals = vec4(normalize(vertNormals), 1.0);
	vFlux = vec4( 0.0, 0.0, 1.0, 1.0 );

	// note the perspective division!
	vec3 tex_coords = SM_tex_coord.xyz / SM_tex_coord.w;
	// read depth value from shadow map
	float depth = texture2D(shadowMap, tex_coords.xy).r;
	float z_diff = tex_coords.z - (depth + 0.007);
	bool inShadow = ((depth + 0.007) < (tex_coords.z)) ? true : false;
	if(tex_coords.x > 1.0 || tex_coords.x < 0.0) inShadow = true;
	if(tex_coords.y > 1.0 || tex_coords.y < 0.0) inShadow = true;

	vec3 tempCol = texture2D( assignedtexture, UV ).rgb * spotLambertTerm * K * m_lightArray[7].x;
	vFragColor = vec4(tempCol, 1.0);
	if(inShadow) vFragColor *= min(z_diff, 1.0);

	vDepth = vec4(vertDepth, vertDepth, vertDepth, 1.0);
}