#pragma once

#include "utils.h"
#include <iostream>


class ShaderLoader
{
	private:

		std::string ReadShader(char *filename);
		GLuint CreateShader(GLenum shaderType,
			std::string source,
			char* shaderName);

	public:
		ShaderLoader(void);
		~ShaderLoader(void);

		//If you are not using a geo shader or teselation shaders use nullptr
		GLuint CreateProgram(char* VertexShaderFilename,char* FragmentShaderFilename, 
			char * geoShaderFile = nullptr,
			char* TessControlShaderFile = nullptr, char* TessEvalShaderFile = nullptr);

};
