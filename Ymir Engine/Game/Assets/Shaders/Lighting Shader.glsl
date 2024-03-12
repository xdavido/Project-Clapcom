#version 450 core

#ifdef VERTEX_SHADER

	layout(location = 0) in vec3 aPos;
	layout(location = 1) in vec3 aNormal;
	layout(location = 2) in vec2 aTexCoords;

	out vec3 Position;
	out vec3 Normal;
	out vec2 TexCoords;

	uniform mat4 projection;
	uniform mat4 view;
	uniform mat4 model;

	void main()
	{
		gl_Position = projection * view * model * vec4(aPos, 1.0f);

		Position  = aPos;
		Normal = aNormal;
		TexCoords = aTexCoords;
	}

#endif 

#ifdef FRAGMENT_SHADER
	
	in vec3 Position;
	in vec3 Normal;
	in vec2 TexCoords;

	out vec4 FragColor;

	uniform sampler2D texture_diffuse;
	uniform sampler2D texture_specular;
	uniform sampler2D texture_normal;
	uniform sampler2D texture_height;
	uniform sampler2D texture_ambient;
	uniform sampler2D texture_emissive;

	uniform vec3 lightDir;
	uniform float lightInt;
	uniform vec3 lightColor;

	vec2 blinnPhongDir(vec3 lightDir, float lightInt, float Ka, float Kd, float Ks, float shininess)
	{
		vec3 s = normalize(lightDir);
		vec3 v = normalize(-Position);
		vec3 n = normalize(Normal);
		vec3 h = normalize(v+s);
		float diffuse = Ka + Kd * lightInt * max(0.0, dot(n, s));
		float spec = Ks * pow(max(0.0, dot(n,h)), shininess);
		return vec2(diffuse, spec);
	}

	void main()
	{
		// Calculate the lighting intensity using Blinn-Phong
		vec2 inten = blinnPhongDir(lightDir, lightInt, 0.2, 0.8, 0.3, 80.0);
    
		// Sample the diffuse texture
		vec3 textureColor = texture(texture_diffuse, TexCoords).rgb;

		// Multiply the texture color with the light intensity and add ambient term
		vec3 finalColor = textureColor * inten.x + vec3(1.0) * inten.y;

		// Apply the light color
		finalColor *= lightColor;

		// Output the final color
		FragColor = vec4(finalColor, 1.0);
	}

#endif








