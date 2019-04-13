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
	//color = vec4((fragColor), 1.0);
	vec4 colorval;
	if(isTextured)
	{
		vec2 newCoords = vec2(fragTexCoord.x * fEndX, fragTexCoord.y * fEndY);
		newCoords = vec2(newCoords.x + fStartX, newCoords.y + fStartY);

		vec4 newAlpha = vec4(1.0, 1.0, 1.0, alpha);
		vec4 texture = (texture(tex, newCoords) * newAlpha);
		colorval = (texture);
	}
	else
	{
		colorval = vec4((setColor), 1.0);
	}

	
	float d = distance(mWorldPos.xyz, camPos);
	float lerp = (d -10.0f)/10.f;
	lerp = clamp(lerp, 0.0, 1.0);
	vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	
	color =  mix(colorval, vFogColor, lerp);
	//color =  colorval;
} 