#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
	mat3 TBN;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

uniform samplerCube depthMap[4];
uniform vec3 lightPos[4];
uniform vec3 lightColor[4];


uniform vec3 viewPos;
uniform int NR_SHADOWS;
uniform float far_plane;


// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), 
   vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
   vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
   vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

float ShadowCalculation(vec3 fragPos, int nr)
{
    vec3 fragToLight = fragPos - lightPos[nr];
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap[nr], fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // Undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    return shadow;
}


void main()
{      
	gPosition = fs_in.FragPos;
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = texture(normalTexture, fs_in.TexCoords).rgb;	
	normal = normalize(normal * 2.0 - 1.0);   
	normal = normalize(fs_in.TBN * normal); 
	gNormal = normal;
    // Ambient
    vec3 ambient = 0.3 * color;
	vec3 lightDir;
	float diff;
	float shadow = 0;
	vec3 diffuse = vec3(0,0,0);
	
    // Diffuse
	for(int i = 0; i<NR_SHADOWS;i++){
	lightDir = normalize(lightPos[i] - fs_in.FragPos);
    diff = max(dot(lightDir, normal), 0.0);
    diffuse += diff * lightColor[i]*2.0;
	shadow += ShadowCalculation(fs_in.FragPos,i);
	}
	
	if(NR_SHADOWS!= 0){
	diffuse /= NR_SHADOWS;
	shadow /= NR_SHADOWS;
	}
	
    vec3 lighting = (ambient + (1.0 - shadow) * diffuse * color);    
    
    gAlbedoSpec = vec4(lighting, 1.0f);
}