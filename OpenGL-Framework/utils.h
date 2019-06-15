#ifndef __UTILS_H__
#define __UTILS_H__

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h" 
#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include <vector>

enum MeshType
{
	MESH_2D_SPRITE,
	MESH_CUBE,
	MESH_SPHERE
};

struct ScreenCoords
{
	GLfloat iX = (GLfloat)glutGet(GLUT_WINDOW_WIDTH);
	GLfloat iY = (GLfloat)glutGet(GLUT_WINDOW_HEIGHT);
};

namespace utils
{
	//glm::vec3 lerpfunc(glm::vec3 vecA, glm::vec3 vecB, float percent)
	//{
	//	return(vecA + percent * (vecB - vecA));
	//}
	
}


#endif // !__UTILS_H__
