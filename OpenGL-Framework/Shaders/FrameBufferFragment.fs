#version 450 core

in vec2 fragTexCoord;

out vec4 color;

uniform sampler2D renderTexture;

uniform float fTime;
uniform float fWidth;
uniform float fHeight;

void main(void)
{	
    float pixelX = (fragTexCoord.x - 1.0f)  * fWidth;
    float pixelY = (1.0f + fragTexCoord.y) * fHeight;

    vec2 fRes = vec2(fWidth, fHeight);
    vec2 fFragXY = vec2(pixelX, pixelY );
    vec2 uv = fFragXY / fRes;

    float d = length(uv - vec2(0.5,0.5));
     vec3 col;
 
   	float blur = 0.0;	
	blur = (1.0 + sin(fTime*6.0)) * 0.35;
	blur *= 1.0 + sin(fTime*16.0) * 0.35;
	blur = pow(blur, 3.0);
	blur *= 0.05;
	// reduce blur towards center
	blur *= d;

    col.r = texture(renderTexture, vec2(uv.x+blur,uv.y)).r;
    col.g = texture(renderTexture, uv).g;
    col.b = texture(renderTexture, vec2(uv.x-blur,uv.y) ).b;

    float scanline = sin(uv.y*800.0)*0.04;
	col -= scanline;

    col *= 1.0 - d * 0.3;
    color = vec4(col,1.0);
	//color =  texture(renderTexture, fragTexCoord);
} 