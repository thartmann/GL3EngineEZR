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

void main(){	
	switch (outputSwitch)
	{
		case 0: //deferred shading
		{
			//break;
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
		default:
		{
		    color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		}
	}
}