#version 150 core

uniform samplerCube cubeTex;
uniform vec3 cameraPos;

in Vertex {
	vec3 normal;
} IN;

out vec4 fragColour;

void main(void) {
	//the result of sampling the cubeTex sampler using the interpolated direction vector we kept in the normal variable
	fragColour = texture(cubeTex, normalize(IN.normal)); 
}