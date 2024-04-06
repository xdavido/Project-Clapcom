#version 450 core

#ifdef VERTEX_SHADER

	layout(location = 0) in vec3 aPos;
	layout(location = 1) in vec3 aNormal;
	layout(location = 2) in vec2 aTexCoords;

	layout(location = 5) in vec3 aTangents;
	layout(location = 6) in vec3 aBitangents;
	
	out vec3 Normal;
	out vec2 TexCoords;

	out vec3 TangentLightPos;
    out vec3 TangentViewPos;
    out vec3 TangentFragPos;
	
	uniform mat4 projection;
	uniform mat4 view;
	uniform mat4 model;
	
	uniform vec3 lightDir;
	uniform vec3 camPos;

	void main()
	{
		gl_Position = projection * view * model * vec4(aPos, 1.0f);

		vec3 Position  = (model * vec4(aPos, 1.0f)).xyz;
		Normal = aNormal;
		TexCoords = aTexCoords;
		
		vec3 T = normalize(vec3(model * vec4(aTangents,   0.0)));
   		vec3 B = normalize(vec3(model * vec4(aBitangents, 0.0)));
   		vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
   		mat3 TBN = mat3(T, B, N);
   		TBN = transpose(TBN);
   		
   		TangentLightPos = TBN * lightDir;
    	TangentViewPos = TBN * camPos;
    	TangentFragPos = TBN * Position;
   		
	}

#endif

#ifdef FRAGMENT_SHADER

	in vec3 Normal;
	in vec2 TexCoords;
	
	in vec3 TangentLightPos;
    in vec3 TangentViewPos;
    in vec3 TangentFragPos;

    out vec4 FragColor;

    uniform sampler2D texture_diffuse;
	uniform sampler2D texture_specular;
	uniform sampler2D texture_normal;
	uniform sampler2D texture_height;
	uniform sampler2D texture_ambient;
	uniform sampler2D texture_emissive;

	uniform float lightInt;
	uniform vec3 lightColor;
    uniform int numLights;

    uniform bool displayNormalMap; // Boolean uniform to toggle between normal visualization and texture display
    uniform bool selected;
    
    uniform float transparency;
    
    float heightScale = 0.0001f;

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

    vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
    { 
        float height =  texture(texture_height, texCoords).r;     
        return texCoords - viewDir.xy * (height * heightScale);  
    }
	
	vec4 PointLight() {
		
        // Point Light Attenuation

		vec3 lightVec = TangentLightPos - TangentFragPos;
		float dist = length(lightVec);
		float a = 0.005;
		float b = 0.0001;
		float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);
		
        // Ambient Occlusion

		float ambient = 0.50f;

        // Parallax Occlusion Mapping (Height Map)

        // offset texture coordinates with Parallax Mapping
        vec3 viewDirection = normalize(TangentViewPos - TangentFragPos);
        vec2 UVs = TexCoords;
        
        UVs = ParallaxMapping(TexCoords,  TangentViewPos);       
        if(UVs.x > 1.0 || UVs.y > 1.0 || UVs.x < 0.0 || UVs.y < 0.0)
            discard;

        // Diffuse

        vec3 normalMap = texture(texture_normal, UVs).xyz * 2.0f - 1.0f;
        vec3 normal = normalize(normalMap);
        vec3 lightDirection = normalize(TangentLightPos - TangentFragPos);
        float diffuse = max(dot(normal, lightDirection), 0.0f);
        
        // Specular

        float specularLight = 0.50f;
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
        float specular = specAmount * specularLight;
         
        // Apply all texture maps

        return vec4(
            texture(texture_diffuse, UVs).rgb * lightColor * (diffuse * intensity + 
            texture(texture_ambient, UVs).r * ambient) +
            texture(texture_specular, UVs).a * specular * intensity, transparency
        );
	
	}
	
	vec4 DirectionalLight() {
		
		float ambient = 0.50f;
            
        //vec3 normal = normalize(Normal);
        vec3 normalMap = texture(texture_normal, TexCoords).xyz * 2.0f - 1.0f;
        vec3 normal = normalize(normalMap);
        vec3 lightDirection = normalize(TangentLightPos);
            
        float specularLight = 0.50f;
        vec3 viewDirection = normalize(TangentViewPos - TangentFragPos);
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
            
        //vec3 normal = normalize(Normal);
        vec3 normalMap = texture(texture_normal, TexCoords).xyz * 2.0f - 1.0f;
        vec3 normal = normalize(normalMap);
        vec3 lightDirection = normalize(TangentLightPos - TangentFragPos);
        float diffuse = max(dot(normal, lightDirection), 0.0f);
        
        float specularLight = 0.50f;
        vec3 viewDirection = normalize (TangentViewPos - TangentFragPos);
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

























