#version 330 core
in vec2 TexCoords;
layout (location = 0) out vec4 color; // <vec2 pos, vec2 tex>
layout (location = 1) out vec4 blur; // <vec2 pos, vec2 tex>

uniform sampler2D image;
uniform vec3 spriteColor;
uniform float intensivity;

void main()
{    
    color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
	blur = vec4(color.rgb, intensivity);
}  