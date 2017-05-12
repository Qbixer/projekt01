#version 330 core
in vec2 TexCoords;
layout (location = 0) out vec4 color; // <vec2 pos, vec2 tex>
layout (location = 1) out vec4 blur; // <vec2 pos, vec2 tex>

uniform sampler2D text;
uniform vec3 textColor;
uniform float intensivity;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
	blur = vec4(textColor, intensivity) * sampled;
}  