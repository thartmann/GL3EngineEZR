#version 330

in vec2 UV;
 
out vec4 color;

uniform mat4 projMat;
uniform mat4 viewMat;

////textures for deferred shading
uniform sampler2D colorMap;
uniform sampler2D positionMap;
uniform sampler2D normalMap;
uniform sampler2D fluxMap;
uniform sampler2D depthMap;

uniform sampler2D randMap;

//switch for debug output
uniform int outputSwitch;

//viewport dimension
uniform vec2 viewport;

uniform vec3 sampleKernel[16];

//values to tweak ssao
float rad = 16.0;

//ssao will now be based on: http://www.john-chapman.net/content.php?id=8
float AmbientOcclusion()
{
	vec3 origin = texture(positionMap, UV).rgb;
	vec3 normal = texture(normalMap, UV).rgb * 2.0 - 1.0;
	normal = normalize(normal);

	vec3 randVec = texture(randMap, UV * (float(viewport) / 4.0)).rgb * 2.0 - 1.0;

	vec3 tangent = normalize(randVec - normal * dot(randVec, normal));
	vec3 bitangent = cross(normal, tangent);

	mat3 alignToNormalMat = mat3(tangent, bitangent, normal);

	float depth = texture(depthMap, UV).r;

	float ao = 0.0;

	for (int i = 0; i < 16; i++)
	{
		//get sample position
		vec3 aosample = alignToNormalMat * sampleKernel[i];
		aosample = aosample * rad + origin;

		//project sample position
		vec4 offset = vec4(aosample, 1.0);
        offset = projMat * offset;
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;
        
		//get sample depth:
		float sampleDepth = texture(depthMap, offset.xy).r;
        
		//range check & accumulate:
		float range = abs(origin.z - sampleDepth) < rad ? 1.0 : 0.0;
        ao += (sampleDepth <= aosample.z ? 1.0 : 0.0) * range;
    }

	ao = 1.0 - (ao / 16.0);
	
	return ao;
}

void main()
{	
	switch (outputSwitch)
	{
		case 0: //final shading
		{
			float x = AmbientOcclusion();
		    color = texture(colorMap, UV) * x;
			break;
		}
		case 1: //show colorMap
		{
		    color = texture(colorMap, UV);
			break;
		}
		case 2: //show positionMap
		{
		    color = texture(positionMap, UV);
			break;
		}
		case 3: //show normalMap
		{
		    color = texture(normalMap, UV);
			break;
		}
		case 4: //show fluxMap
		{
		    color = texture(fluxMap, UV);
			break;
		}
		case 5: //show depthMap
		{
		    color = vec4(texture(depthMap, UV).rgb, 1.0);
			break;
		}
		case 6: //show ssao
		{
			float x = AmbientOcclusion();
		    color = vec4(x, x, x, 1.0);
			break;
		}
		default:
		{
		    color = vec4(1.0, 0.0, 0.0, 1.0);
			break;
		}
	}
}