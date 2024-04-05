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

		Position  = (model * vec4(aPos, 1.0f)).xyz;
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
    uniform int numLights;
    uniform vec3 camPos;

    uniform bool displayNormalMap; // Boolean uniform to toggle between normal visualization and texture display
    uniform bool selected;
    
    uniform float transparency;

    vec4 DisplayNormalMap() {

        // Normalize the interpolated normal since it's interpolated across the surface
        vec3 norm = normalize(Normal);
    
        // Adjust the normalized normal to be in the range [0, 1] for visualization purposes
        vec3 color = 0.5 * (norm + vec3(1.0));

        return vec4(color, transparency);

    }

    vec4 AddOutline(vec4 mainTexture, vec4 color, float outlineWidth) {

        // Sample the neighboring pixels

        vec2 pixelSize = 1.0 / textureSize(texture_diffuse, 0);
        vec4 leftColor = texture(texture_diffuse, TexCoords - vec2(outlineWidth, 0) * pixelSize);
        vec4 rightColor = texture(texture_diffuse, TexCoords + vec2(outlineWidth, 0) * pixelSize);
        vec4 topColor = texture(texture_diffuse, TexCoords + vec2(0, outlineWidth) * pixelSize);
        vec4 bottomColor = texture(texture_diffuse, TexCoords - vec2(0, outlineWidth) * pixelSize);

        // Check if the current pixel is on the border

        bool isBorder = any(notEqual(mainTexture, leftColor)) || any(notEqual(mainTexture, rightColor)) ||
                        any(notEqual(mainTexture, topColor)) || any(notEqual(mainTexture, bottomColor));

        // Set the outline color if it's a border pixel, otherwise use the main color

        return isBorder ? color : mainTexture;

    }

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
	
	vec4 PointLight() {
		
        // Point Light Attenuation

		vec3 lightVec = lightDir - Position;
		float dist = length(lightVec);
		float a = 0.005;
		float b = 0.0001;
		float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);
		
        // Ambient Occlusion

		float ambient = 0.30f;
            
        // Diffuse

        vec3 normal = normalize(Normal);
        vec3 lightDirection = normalize(lightDir - Position);
        float diffuse = max(dot(normal, lightDirection), 0.0f);
        
        // Specular

        float specularLight = 0.50f;
        vec3 viewDirection = normalize (camPos - Position);
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
        float specular = specAmount * specularLight;
         
        // Apply all texture maps

        return vec4(
            texture(texture_diffuse, TexCoords).rgb * lightColor * (diffuse * intensity + 
            texture(texture_ambient, TexCoords).r * ambient) +
            texture(texture_specular, TexCoords).a * specular * intensity, transparency
        );
	
	}
	
	vec4 DirectionalLight() {
		
		float ambient = 0.50f;
            
        vec3 normal = normalize(Normal);
        vec3 lightDirection = normalize(lightDir);
            
        float specularLight = 0.50f;
        vec3 viewDirection = normalize (camPos - Position);
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
        float specular = specAmount * specularLight;
            
        float diffuse = max(dot(normal, lightDirection), 0.0f);
         
        return vec4(
            texture(texture_diffuse, TexCoords).rgb * lightColor * (diffuse+ ambient) + 
            texture(texture_specular, TexCoords).r * specular, transparency
        );
	
	}
	
	vec4 SpotLight() {
		
		float outerCone = 0.90f;
		float innerCone = 0.95f;
		
		float ambient = 0.20f;
            
        vec3 normal = normalize(Normal);
        vec3 lightDirection = normalize(lightDir - Position);
        float diffuse = max(dot(normal, lightDirection), 0.0f);
        
        float specularLight = 0.50f;
        vec3 viewDirection = normalize (camPos - Position);
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
        float specular = specAmount * specularLight;
            
        float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
        float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
         
        return vec4(
            texture(texture_diffuse, TexCoords).rgb * lightColor * (diffuse * intensity + ambient) + 
            texture(texture_specular, TexCoords).r * specular * intensity, transparency
        );
	
	}

    void main()
    {   
        if (displayNormalMap) {
    
            FragColor = DisplayNormalMap();

        } 
        else {
            
            FragColor = PointLight();

        }
        
    }

#endif


















