#version 150 core

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 position;

out Vertex {
	vec3 normal;
} OUT;

void main(void) {
	vec3 multiply = vec3(0,0,0);
	//we must also take into consideration the aspect ratio of the screen, and the desired field of vision
	multiply.x = 1.0f / projMatrix[0][0];
	multiply.y = 1.0f / projMatrix[1][1];

	vec3 tempPos = (position * multiply) - vec3(0,0,1);
		
	//We don't want to apply the translation component to the rotation, as this would
	//warp our direction vector as the camera moved away from the origin, so we downcast the view matrix
	//to a mat3 - remember, the translation component is in the fourth column of the view matrix.
	//We use the transpose of the view matrix -> we need the inverse of the view matrix to rotate the world objects correctly
	OUT.normal = transpose(mat3(viewMatrix)) * normalize(tempPos); //we rotate the direction vector by multiplying it by the view matrix...sort of
	gl_Position = vec4(position, 1.0);
}