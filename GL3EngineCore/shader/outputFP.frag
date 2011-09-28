#version 330

in vec2 UV;
 
out vec4 gl_FragColor;

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

//float depth = texture(depthMap, UV).x;

//float sampleRad = depth * 0.2;
//float intensity = 8.0f;
//float bias = 0.1f;

float calculateOcclusion(float pos)
{
vec3 H = texture(positionMap, vec2(UV.x+ pos , UV.y)).xyz - texture(positionMap, vec2(UV.x , UV.y)).xyz;

vec3 pcT = cross(texture( normalMap, vec2(UV.x + pos, UV.y)).xyz, H );

vec3 T = cross(texture( normalMap, vec2(UV.x + pos, UV.y)).xyz, pcT );

float h = atan(H.z / length(H.xy));
float t = atan(T.z / length(T.xy));

float am_oc = sin(h)-sin(t);


	return am_oc;
}

float AmbientOcclusion()
{
	
	float res = 1.0;
	float value = 0.0f;
	for(float i = 1/600; i < 4/600; i+=i/600)
	{
		value = texture(depthMap, vec2(UV.x, UV.y +i )).x;
		if(value < res){ res = i;}
	}
	float ao = calculateOcclusion(res);

	for(float i = 1/600; i < 4/600; i+=i/600)
	{
		value = texture(depthMap, vec2(UV.x, UV.y -i)).x;
		if(value < res){ res = i;}
	}
//	res = 0.0;
	ao += calculateOcclusion(res);
//float	ao = texture(depthMap, vec2(UV.x, UV.y-res )).x;
	ao = ao/2;

	ao = clamp(ao, 0.0, 1.0);
	return ao;
}

void main()
{	
	switch (outputSwitch)
	{
		case 0: //final shading
		{
		    gl_FragColor = vec4(texture(colorMap, UV).rgb * AmbientOcclusion(), 1.0f);
			break;
		}
		case 1: //show gl_FragColorMap
		{
		    gl_FragColor = vec4(texture(colorMap, UV).rgb, 1.0f);
			break;
		}
		case 2: //show positionMap
		{
		    gl_FragColor = vec4(texture(positionMap, UV).rgb, 1.0f);
			break;
		}
		case 3: //show normalMap
		{
		    gl_FragColor = vec4(texture(normalMap, UV).rgb, 1.0f);
			break;
		}
		case 4: //show fluxMap
		{
		    gl_FragColor = vec4(texture(fluxMap, UV).rgb, 1.0f);
			break;
		}
		case 5: //show depthMap
		{
		    gl_FragColor = vec4(texture(depthMap, UV).rgb, 1.0f);
			break;
		}
		case 6: //show ssao
		{
			float x = AmbientOcclusion();
		    gl_FragColor = vec4(x, x, x, 1.0f);
//		gl_FragColor = vec4(AmbientOcclusion(), 1.0f);
			break;
		}
		default:
		{
		    gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		}
	}
}