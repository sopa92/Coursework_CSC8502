#version 150 core

uniform sampler2D diffuseTex;
uniform float heightThreshold;
uniform float maxHeightPoint;
uniform float minHeightPoint;


in Vertex {
	vec2 texCoord;
	float heightVertex;
} IN;

out vec4 glFragColor;

void main(void) {
	float ratio = (IN.heightVertex - heightThreshold) / (maxHeightPoint - heightThreshold);
	if (ratio > 0.0)
		glFragColor = mix(texture(diffuseTex, IN.texCoord), vec4(1, 1, 1, 1), ratio);
	else {
		ratio = (IN.heightVertex - minHeightPoint) / (heightThreshold - minHeightPoint);
		glFragColor = mix(vec4(0.5, 0.4, 0.1, 1), texture(diffuseTex, IN.texCoord), ratio);
	}
}