#version 450 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec4 mWorldPos;
out vec4 color;

uniform sampler2D texGrass;
uniform sampler2D texEarth;
uniform sampler2D texSnow;

float fRenderHeight = 75.0f;

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
    float tilingNum = 32.0f;
    float fScale = mWorldPos.y/fRenderHeight;

    const float fRange1 = 0.01f;
	const float fRange2 = 0.15f;
	const float fRange3 = 0.6f;
    const float fRange4 = 0.85f;

	if(fScale <= fRange1)
    {
		vTexColor = texture2D(texGrass, fragTexCoord * tilingNum);
    }	
	else if(fScale <= fRange2){
		fScale -= fRange1;
		fScale /= (fRange2-fRange1);
		
		float fScale2 = fScale;
		fScale = 1.0-fScale; 
		
		vTexColor += texture2D(texGrass, fragTexCoord * tilingNum)*fScale;
		vTexColor += texture2D(texEarth, fragTexCoord * tilingNum)*fScale2;
	}
	else if(fScale <= fRange3)
    {
		vTexColor = texture2D(texEarth, fragTexCoord * tilingNum);
    }
    else if(fScale <= fRange4){
		fScale -= fRange3;
		fScale /= (fRange4-fRange3);
		
		float fScale2 = fScale;
		fScale = 1.0-fScale; 
		
		vTexColor += texture2D(texEarth, fragTexCoord * tilingNum)*fScale;
		vTexColor += texture2D(texSnow, fragTexCoord * tilingNum)*fScale2;		
	}	
	else if(fScale > fRange4) {vTexColor = texture2D(texSnow, fragTexCoord * tilingNum);}


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