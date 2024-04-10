#version 450 core

#ifdef VERTEX_SHADER

	layout(location = 0) in vec3 aPos;
	layout(location = 1) in vec3 aNormal;
	layout(location = 2) in vec2 aTexCoords;

	out vec2 TexCoords;

	uniform mat4 projection;
	uniform mat4 view;
	uniform mat4 model;

	float outline = 0.3f;

	void main()
	{
		gl_Position = projection * view * model * normalize(vec4(aPos, 1.0f));
	}

#endif 

#ifdef FRAGMENT_SHADER

	out vec4 FragColor;

	void main()
	{
		FragColor = vec4(0.91f, 0.527f, 0.129f, 1.0f);
	}

#endif






