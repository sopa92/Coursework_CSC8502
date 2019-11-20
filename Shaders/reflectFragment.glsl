#version 150 core

uniform sampler2D diffuseTex;
//this type allows cubemap's sampling using a direction vector, it will automatically 
//handle which of the 6 separate textures the resulting sample will be taken from
uniform samplerCube cubeTex; 

uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform float lightRadius;

in Vertex{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 fragColour;

void main(void) {

	vec4 diffuse = texture(diffuseTex, IN.texCoord)* IN.colour; //calculate the diffuse colour
	vec3 incident = normalize(IN.worldPos - cameraPos);			//calculate incident vector
	float dist = length(lightPos - IN.worldPos);				//calculate light distance
	float atten = 1.0 - clamp(dist / lightRadius, 0.2, 1.0);	//calculate light attenuation

	//We sample the cubeTex sampler with a direction vector, which we calculate 
	//using the glsl reflect function, which calculates the angle of reflection
	vec4 reflection = texture(cubeTex,reflect(incident, normalize(IN.normal)));

	fragColour = (lightColour * diffuse * atten)*(diffuse + reflection); // (attenuated light colour) * (sampled cubemap reflection colour)
}