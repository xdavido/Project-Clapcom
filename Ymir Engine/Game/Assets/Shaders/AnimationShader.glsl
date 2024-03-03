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
	}

#endif 

#ifdef FRAGMENT_SHADER

	in vec2 TexCoords;

	out vec4 FragColor;

	uniform sampler2D texture_diffuse1;

	void main()
	{
		FragColor = texture(texture_diffuse1, TexCoords);
	}

#endif







