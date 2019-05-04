#version 450 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec4 mWorldPos;

out vec4 color;

uniform sampler2D tex;

uniform vec3 lightColor; // = glm::vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos; // = glm::vec3(5.0f, 5.0f, 0.5f);

uniform float ambientStr;
uniform vec3 ambientColor;

uniform float lightSpecStr; // = 1.0f;
uniform vec3 camPos; // = glm::vec3(1.0f, 1.0f, 3.0f);

uniform float shininess; // = 32.0f;

uniform bool isTextured;
uniform vec3 setColor;

void main(void)
{	
	vec4 vTexColor = vec4(0.0);
	vTexColor = texture2D(tex, fragTexCoord);

	//ambient lighting
	vec3 ambient = ambientStr * ambientColor;

	//direction lighting
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(fragPos - lightPos);

	//Diffuse coloring
	float diffuseStr = max(dot(norm, - lightDir), 0.0f);
	vec3 diffuse = diffuseStr * lightColor;

	color =  vec4(ambient + diffuse, 1.0f) * vTexColor;
} 