#version 450 core

in vec2 fragTexCoord;

out vec4 color;

uniform sampler2D renderTexture;

void main(void)
{	

	color =  texture(renderTexture, fragTexCoord);

} 