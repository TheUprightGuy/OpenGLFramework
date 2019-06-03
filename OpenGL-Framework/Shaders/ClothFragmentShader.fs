#version 450 core

in vec3 fragColor; 
in vec2 fragTexCoord;
in vec4 mWorldPos;

out vec4 color;

uniform vec3 setColor;


void main(void)
{	
	color =  vec4(setColor, 1.0f);
} 