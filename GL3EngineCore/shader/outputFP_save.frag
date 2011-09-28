#version 330

in vec2 UV;
 
out vec4 color;

////textures for deferred shading
uniform sampler2D colorMap;
uniform sampler2D positionMap;
uniform sampler2D normalMap;
uniform sampler2D fluxMap;
uniform sampler2D depthMap;

//switch for debug output
uniform int outputSwitch;

//viewport dimension
uniform vec2 viewport;

float sampleRad = texture(depthMap, UV).x;
float intensity = 12.0f;
float scale = 5.0f;
float bias = 0.2f;


//ssao inspired by: http://www.gamedev.net/page/resources/_/reference/programming/140/lighting-and-shading/a-simple-and-practical-approach-to-ssao-r2753
float calculateOcclusion(vec2 sampleUV, vec3 position, vec3 normal)
{
	float depth = texture(depthMap, UV).x;
	float sampleDepth = texture(depthMap, UV + sampleUV).x;

	vec3 diff = texture(positionMap, UV + sampleUV).xyz - position;
	vec3 v = normalize(diff);
	float d = length(diff) * scale;

	if (depth >= sampleDepth)
	{
		return 0.0f;
	}
	else
	{
		return max(0.0, dot(normal, v) - bias) * (1.0f / (1.0f + d)) * intensity;
	}

}

vec4 ssao()
{
	//offsets for neighbor texel access
	float offsetX = 1.0f / viewport.x;
	float offsetY = 1.0f / viewport.y;

	//fragment values
	vec3 position = texture(positionMap, UV).xyz;
	vec3 normal = texture(normalMap, UV).xyz;

	vec4 white = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	if (position == normal && position == white.xyz)
	{
		return white;
	}


	float rad = sampleRad / position.z;

	//sample texture coordinates
	vec2 sample0UV = vec2(UV.x, UV.y + offsetY);
	vec2 sample1UV = vec2(UV.x + offsetX, UV.y + offsetY);
	vec2 sample2UV = vec2(UV.x + offsetX, UV.y);
	vec2 sample3UV = vec2(UV.x + offsetX, UV.y - offsetY);
	vec2 sample4UV = vec2(UV.x, UV.y - offsetY);
	vec2 sample5UV = vec2(UV.x - offsetX, UV.y - offsetY);
	vec2 sample6UV = vec2(UV.x - offsetX, UV.y);
	vec2 sample7UV = vec2(UV.x - offsetX, UV.y + offsetY);

	vec2 vecs[8];
	
	vecs[0] = vec2(0.0f, 1.0f);
	vecs[1] = vec2(1.0f, 1.0f);
	vecs[2] = vec2(1.0f, 0.0f);
	vecs[3] = vec2(1.0f, -1.0f);
	vecs[4] = vec2(0.0f, -1.0f);
	vecs[5] = vec2(-1.0f, -1.0f);
	vecs[6] = vec2(-1.0f, 0.0f);
	vecs[7] = vec2(-1.0f, 1.0f);

	vec2 reflectNorms[8];

	reflectNorms[0] = texture(normalMap, sample0UV).xy;
	reflectNorms[1] = texture(normalMap, sample1UV).yz;
	reflectNorms[2] = texture(normalMap, sample2UV).xz;
	reflectNorms[3] = texture(normalMap, sample3UV).xy;
	reflectNorms[4] = texture(normalMap, sample4UV).yz;
	reflectNorms[5] = texture(normalMap, sample5UV).xz;
	reflectNorms[6] = texture(normalMap, sample6UV).xy;
	reflectNorms[7] = texture(normalMap, sample7UV).yz;
													
	float ao = 0.0f;
	int neighbors = 8;

	for (int i = 0; i < neighbors; i++)
	{
		vec2 coord1 = reflect(vecs[i], reflectNorms[i]) * rad;
		vec2 coord2 = vec2(coord1.x * 0.707 - coord1.y * 0.707, coord1.x * 0.707 + coord1.y * 0.707);
		
		ao += calculateOcclusion(coord1*0.25, position, normal);
		ao += calculateOcclusion(coord2*0.5, position, normal);
		ao += calculateOcclusion(coord1*0.75, position, normal);
		ao += calculateOcclusion(coord2, position, normal);
	}

	ao /= float(neighbors) * 4.0f;

	return vec4(ao, ao, ao, 1.0f);
}

void main()
{	
	switch (outputSwitch)
	{
		case 0: //final shading
		{
		    color = vec4(texture(colorMap, UV).rgb, 1.0f) * ssao();
			break;
		}
		case 1: //show colorMap
		{
		    color = vec4(texture(colorMap, UV).rgb, 1.0f);
			break;
		}
		case 2: //show positionMap
		{
		    color = vec4(texture(positionMap, UV).rgb, 1.0f);
			break;
		}
		case 3: //show normalMap
		{
		    color = vec4(texture(normalMap, UV).rgb, 1.0f);
			break;
		}
		case 4: //show fluxMap
		{
		    color = vec4(texture(fluxMap, UV).rgb, 1.0f);
			break;
		}
		case 5: //show depthMap
		{
		    color = vec4(texture(depthMap, UV).rgb, 1.0f);
			break;
		}
		case 6: //show ssao
		{
		    color = ssao();
			break;
		}
		default:
		{
		    color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		}
	}
}