#version 450 core


in vec3 fragColor; 
in vec2 fragTexCoord;
in vec4 mWorldPos;

out vec4 color;

uniform sampler2D tex;

uniform float fStartX;
uniform float fStartY;

uniform float fEndX;
uniform float fEndY;

uniform float alpha;

uniform vec3 camPos; // = glm::vec3(1.0f, 1.0f, 3.0f);

uniform bool isTextured;
uniform vec3 setColor;


void main(void)
{	
	color =  vec4(1.0f, 0.0f, 0.0f, 1.0f);;
} 