#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;
uniform samplerCube depthMap2;

uniform vec3 lightPos;
uniform vec3 lightPos2;
uniform vec3 viewPos;

uniform float far_plane;

float ShadowCalculation(vec3 fragPos)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
vec3 fragToLight2 = fragPos - lightPos2;
    // Use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
float closestDepth2 = texture(depthMap2, fragToLight2).r;
    // It is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= far_plane;
closestDepth2 *= far_plane;
    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
float currentDepth2 = length(fragToLight2);
    // Now test for shadows
    float bias = 0.05; 
    float shadow2 = currentDepth2 -  bias > closestDepth2 ? 1.0 : 0.0;
float shadow1 = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
float shadow = (shadow1+shadow2)/2;
    return shadow;
} 

void main()
{   gPosition = fs_in.FragPos;
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
	gNormal = normal;
    vec3 lightColor = vec3(0.3);
    // Ambient
    vec3 ambient = 0.4 * color;
    // Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
vec3 lightDir2 = normalize(lightPos2 - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
float diff2 = max(dot(lightDir2, normal), 0.0);
    vec3 diffuse = diff * lightColor;
vec3 diffuse2 = diff2 * lightColor;
    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
vec3 reflectDir2 = reflect(-lightDir2, normal);
    float spec = 0.0;
float spec2 = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
vec3 halfwayDir2 = normalize(lightDir2 + viewDir); 
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
spec2 = pow(max(dot(normal, halfwayDir2), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
vec3 specular2 = spec2 * lightColor;  
    // Calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPos);                      
    vec3 lighting = (ambient + (1.0 - shadow) * ((diffuse+diffuse2)/2 + (specular+specular2)/2)) * color;    
    
    gAlbedoSpec = vec4(lighting, 1.0f);
} 