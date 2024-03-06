#version 450 core

#ifdef VERTEX_SHADER

	layout(location = 0) in vec3 aPos;
	layout(location = 1) in vec3 aNormal;
	layout(location = 2) in vec2 aTexCoords;
	layout(location = 3) in ivec4 boneIds; 
	layout(location = 4) in vec4 weights;
	
	const int MAX_BONES = 100;
	const int MAX_BONE_INFLUENCE = 4;
	uniform mat4 finalBonesMatrices[MAX_BONES];

	out vec2 TexCoords;
	out vec3 Normal;

	uniform mat4 projection;
	uniform mat4 view;
	uniform mat4 model;

	void main()
	{
		vec4 totalPosition = vec4(0.0f);
		for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
		{
			if(boneIds[i] == -1)
				continue;
			if(boneIds[i] >=MAX_BONES)
			{
				totalPosition = vec4(aPos,1.0f);
				break;
			}
			vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos,1.0f);
			totalPosition += localPosition * weights[i];
			vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * aNormal;
		}
		gl_Position = projection * view * model * totalPosition;
		TexCoords = aTexCoords;
		Normal = aNormal; 
	}

#endif 

#ifdef FRAGMENT_SHADER

in vec2 TexCoords;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform bool displayNormalMap; // Boolean uniform to toggle between normal visualization and texture display
uniform bool selected;

vec4 DisplayNormalMap() {

    // Normalize the interpolated normal since it's interpolated across the surface
    vec3 norm = normalize(Normal);
    
    // Adjust the normalized normal to be in the range [0, 1] for visualization purposes
    vec3 color = 0.5 * (norm + vec3(1.0));

    return vec4(color, 1.0);

}

vec4 AddOutline(vec4 mainTexture, vec4 color, float outlineWidth) {

    // Sample the neighboring pixels

    vec2 pixelSize = 1.0 / textureSize(texture_diffuse1, 0);
    vec4 leftColor = texture(texture_diffuse1, TexCoords - vec2(outlineWidth, 0) * pixelSize);
    vec4 rightColor = texture(texture_diffuse1, TexCoords + vec2(outlineWidth, 0) * pixelSize);
    vec4 topColor = texture(texture_diffuse1, TexCoords + vec2(0, outlineWidth) * pixelSize);
    vec4 bottomColor = texture(texture_diffuse1, TexCoords - vec2(0, outlineWidth) * pixelSize);

    // Check if the current pixel is on the border

    bool isBorder = any(notEqual(mainTexture, leftColor)) || any(notEqual(mainTexture, rightColor)) ||
                    any(notEqual(mainTexture, topColor)) || any(notEqual(mainTexture, bottomColor));

    // Set the outline color if it's a border pixel, otherwise use the main color

    return isBorder ? color : mainTexture;

}

void main()
{   
    if (displayNormalMap) {
    
        FragColor = DisplayNormalMap();

    } 
    else {

        vec4 mainTexture = texture(texture_diffuse1, TexCoords);
        FragColor = mainTexture;

        if (selected) {

            FragColor = AddOutline(mainTexture, vec4(1.0, 0.5, 0.0, 1.0), 0.2);

        }

    }
}

#endif









