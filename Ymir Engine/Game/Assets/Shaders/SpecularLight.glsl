#version 450 core

#ifdef VERTEX_SHADER

	layout(location = 0) in vec3 aPos;
	layout(location = 1) in vec3 aNormal;
	layout(location = 2) in vec2 aTexCoords;

	layout(location = 5) in vec3 aTangents;
	layout(location = 6) in vec3 aBitangents;
	
    struct PointLight {    
        vec3 lightDir;
        float lightInt;
	    vec3 lightColor;
    };  

    struct TangentPointLight {    
        vec3 TangentLightPos;
        vec3 lightColor;
        float lightInt;
    }; 

    #define MAX_POINT_LIGHTS 9

	out vec3 Normal;
	out vec2 TexCoords;

    out TangentPointLight tPointLights[MAX_POINT_LIGHTS];
    out vec3 TangentViewPos;
    out vec3 TangentFragPos;
	
	uniform mat4 projection;
	uniform mat4 view;
	uniform mat4 model;
	
	uniform vec3 camPos;

    uniform int numLights;
    uniform PointLight pointLights[MAX_POINT_LIGHTS];

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
   		
        TangentViewPos = TBN * camPos;
    	TangentFragPos = TBN * Position;

        for (int i = 0; i < numLights; i++) {

            tPointLights[i].TangentLightPos = TBN * pointLights[i].lightDir;
            tPointLights[i].lightInt = pointLights[i].lightInt;
            tPointLights[i].lightColor =  pointLights[i].lightColor;

        }
   		
	}

#endif

#ifdef FRAGMENT_SHADER
	
    struct TangentPointLight {    
        vec3 TangentLightPos;
        vec3 lightColor;
        float lightInt;
    }; 
    
    #define MAX_POINT_LIGHTS 9
    in TangentPointLight tPointLights[MAX_POINT_LIGHTS];
    uniform int numLights;

	in vec3 Normal;
	in vec2 TexCoords;
	
    in vec3 TangentViewPos;
    in vec3 TangentFragPos;

    out vec4 FragColor;

    uniform sampler2D texture_diffuse;
	uniform sampler2D texture_specular;
	uniform sampler2D texture_normal;
	uniform sampler2D texture_height;
	uniform sampler2D texture_ambient;
	uniform sampler2D texture_emissive;
    
   	uniform bool enableDiffuse;
    uniform bool enableSpecular;
    uniform bool enableNormal;
    uniform bool enableAmbient;
    uniform bool enableHeight;
    uniform bool enableEmissive;
    
   	uniform float transparency = 1.0f;
   	uniform float ambient = 0.50f;
    uniform float specularLight = 0.50f;
    uniform float normalIntensity = 1.0f;
	uniform float emissive = 1.0f;
	
	vec4 CalculatePointLight(TangentPointLight light, vec3 normal, vec3 TangentFragPos, vec3 viewDirection) {
		
        // Point Light Attenuation

		vec3 lightVec = light.TangentLightPos - TangentFragPos;
		float dist = length(lightVec);
		float a = 0.005;
		float b = 0.0001;
		float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);
            
        // Normal

        vec3 lightDirection = normalize(light.TangentLightPos - TangentFragPos);
        float diffuse = max(dot(normal, lightDirection), 0.0f);
        
        // Specular
        
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
        float specular = specAmount * specularLight;
		
		// Apply texture maps

        vec3 finalColor = vec3(0.0); // Initialize the final color variable

        // Apply diffuse and ambient texture
        finalColor += (enableDiffuse ? texture(texture_diffuse, TexCoords).rgb : vec3(1.0f)) * light.lightColor * (diffuse * intensity +
        ambient * intensity * (enableAmbient ? texture(texture_ambient, TexCoords).r : 1.0f));

        // Apply specular texture
        if (enableSpecular) finalColor += texture(texture_specular, TexCoords).a * specular * intensity;

        // Apply emissive texture
        if (enableEmissive) finalColor += texture(texture_emissive, TexCoords).rgb * emissive;

        // Apply transparency
        return vec4(finalColor,transparency);
	
	}
	
	vec4 DirectionalLight() {
		
		float ambient = 0.50f;
            
        //vec3 normal = normalize(Normal);
        vec3 normalMap = texture(texture_normal, TexCoords).xyz * 2.0f - 1.0f;
        vec3 normal = normalize(normalMap);
        //vec3 lightDirection = normalize(TangentLightPos);
        vec3 lightDirection = normalize(vec3(0.0f));
            
        float specularLight = 0.50f;
        vec3 viewDirection = normalize(TangentViewPos - TangentFragPos);
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
        float specular = specAmount * specularLight;
            
        float diffuse = max(dot(normal, lightDirection), 0.0f);
         
        return vec4(
            texture(texture_diffuse, TexCoords).rgb * /*light.lightColor **/ (diffuse+ ambient) + 
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
        //vec3 lightDirection = normalize(TangentLightPos - TangentFragPos);
        vec3 lightDirection = normalize(vec3(0.0f));
        
        float diffuse = max(dot(normal, lightDirection), 0.0f);
        
        float specularLight = 0.50f;
        vec3 viewDirection = normalize (TangentViewPos - TangentFragPos);
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
        float specular = specAmount * specularLight;
            
        float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
        float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
         
        return vec4(
            texture(texture_diffuse, TexCoords).rgb * /*light.lightColor **/ (diffuse * intensity + ambient) + 
            texture(texture_specular, TexCoords).r * specular * intensity, transparency
        );
	
	}

    vec4 AreaLight() {

        return vec4(0.0f);

    }

    vec4 CalculateBaseLight(vec3 normal, vec3 TangentFragPos, vec3 viewDirection) {
            
        // Normal

        vec3 lightDirection = normalize(TangentFragPos);
        float diffuse = max(dot(normal, lightDirection), 0.0f);
        
        // Specular
        
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
        float specular = specAmount * specularLight;
		
		// Apply texture maps

        vec3 finalColor = vec3(0.0); // Initialize the final color variable

        // Apply diffuse and ambient texture
        finalColor += (enableDiffuse ? texture(texture_diffuse, TexCoords).rgb : vec3(1.0f)) * (diffuse +
        ambient * (enableAmbient ? texture(texture_ambient, TexCoords).r : 1.0f));

        // Apply specular texture
        if (enableSpecular) finalColor += texture(texture_specular, TexCoords).a * specular;

        // Apply emissive texture
        if (enableEmissive) finalColor += texture(texture_emissive, TexCoords).rgb * emissive;

        // Apply transparency
        return vec4(finalColor,transparency);
    }

    void main()
    {   
        vec3 normalMap = texture(texture_normal, TexCoords).xyz * 2.0f - 1.0f;
        vec3 normal = (enableNormal ? normalize(normalMap) : normalize(Normal)) * normalIntensity;

        vec3 viewDirection = normalize(TangentViewPos - TangentFragPos);

        vec4 finalColor;
		
		if (numLights > 0) // Point Light Management
        {
	        for (int i = 0; i < numLights; i++) 
            {
	            finalColor += CalculatePointLight(tPointLights[i], normal, TangentFragPos, viewDirection);
	        }    
        }
        else 
        { 
            // Base Light Management
            finalColor += CalculateBaseLight(normal, TangentFragPos, viewDirection);
        }

        FragColor = finalColor;
    }

#endif







































