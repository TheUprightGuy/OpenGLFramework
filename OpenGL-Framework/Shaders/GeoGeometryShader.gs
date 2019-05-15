#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices= 15) out;

out vec3 outColor;

in VS_GS_VERTEX
{
	in vec4 position;
	in vec3 color;
	in mat4 mvp;
} gs_in[];

void main() 
{
	outColor= gs_in[0].color;

	gl_Position= gs_in[0].position + gs_in[0].mvp* vec4(0.0f, 1.5f, 0.0f, 0.0f); EmitVertex();
	gl_Position= gs_in[0].position + gs_in[0].mvp* vec4(0.85f, -0.8f, 0.0f, 0.0f); EmitVertex();
	gl_Position= gs_in[0].position + gs_in[0].mvp* vec4(-0.85f, -0.8f, 0.0f, 0.0f); EmitVertex();

	EndPrimitive();

	gl_Position= gs_in[0].position + gs_in[0].mvp* vec4(2.0f, 0.0f, 0.0f, 0.0f); EmitVertex();
	gl_Position= gs_in[0].position + gs_in[0].mvp* vec4(0.0f, -1.4f, 0.0f, 0.0f); EmitVertex();
	gl_Position= gs_in[0].position + gs_in[0].mvp* vec4(-0.55f, 0.0f, 0.0f, 0.0f); EmitVertex();

	EndPrimitive();

	gl_Position= gs_in[0].position + gs_in[0].mvp* vec4(1.5f, -2.5f, 0.0f, 0.0f); EmitVertex();
	gl_Position= gs_in[0].position + gs_in[0].mvp* vec4(0.55f, 0.0f, 0.0f, 0.0f); EmitVertex();
	gl_Position= gs_in[0].position + gs_in[0].mvp* vec4(-0.88f, -0.8f, 0.0f, 0.0f); EmitVertex();

	EndPrimitive();

	gl_Position= gs_in[0].position + gs_in[0].mvp* vec4(-1.5f, -2.5f, 0.0f, 0.0f); EmitVertex();
	gl_Position= gs_in[0].position + gs_in[0].mvp* vec4(-0.55f, 0.0f, 0.0f, 0.0f); EmitVertex();
	gl_Position= gs_in[0].position + gs_in[0].mvp* vec4(0.85f, -0.8f, 0.0f, 0.0f); EmitVertex();

	EndPrimitive();

	gl_Position= gs_in[0].position + gs_in[0].mvp* vec4(-2.0f, 0.0f, 0.0f, 0.0f); EmitVertex();
	gl_Position= gs_in[0].position + gs_in[0].mvp* vec4(0.0f, -1.4f, 0.0f, 0.0f); EmitVertex();
	gl_Position= gs_in[0].position + gs_in[0].mvp* vec4(0.55f, 0.0f, 0.0f, 0.0f); EmitVertex();

	EndPrimitive();
}