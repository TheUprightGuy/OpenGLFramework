#version 450 core

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragPos;

out vec4 color;

uniform float fStartX;
uniform float fStartY;
uniform float fEndX;
uniform float fEndY;

uniform sampler2D tex;
uniform vec3 camPos; // = glm::vec3(1.0f, 1.0f, 3.0f);
uniform samplerCube skybox;

void main(void)
{	

	vec2 newCoords = vec2(fragTexCoord.x * fEndX, fragTexCoord.y * fEndY);
	newCoords = vec2(newCoords.x + fStartX, newCoords.y + fStartY);

	//vec4 newAlpha = vec4(1.0, 1.0, 1.0, 1.0);
	//vec4 texture = (texture(tex, newCoords) * newAlpha);

	vec3 norm = normalize(fragNormal);
	vec3 viewDir = normalize(fragPos - camPos);
	vec3 reflectDir = reflect(viewDir, norm);


	vec3 skyboxRGB = texture(skybox, reflectDir).rgb;
	color = vec4( skyboxRGB , 1.0f);
} 