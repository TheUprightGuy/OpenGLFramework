#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;


uniform mat4 MVP;
uniform mat4 model;

out vec4 mWorldPos;
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main()
{	

	mWorldPos = model * vec4(position, 1.0f);

	gl_Position =  MVP  * vec4(position, 1.0f); 

	fragPos = vec3(model * vec4(position, 1.0f));
	fragNormal = mat3(transpose(inverse(model))) * normal;

	fragTexCoord = texCoord;
} 