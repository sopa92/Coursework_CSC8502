#version 150 core

uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 glFragColor;

void main(void){
	glFragColor = texture(diffuseTex, IN.texCoord);
}