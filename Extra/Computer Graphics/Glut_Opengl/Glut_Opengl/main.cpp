#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "cyGL.h"
#include "cyMatrix.h"
#include "cyVector.h"
#include "cyTriMesh.h"

using namespace std;

cy::TriMesh mesh;
cy::GLSLProgram prog;
cy::Matrix4f Projection;

//To set the viewport
void changeViewPort(int x, int y)
{
	glViewport(0, 0, x, y);
}

//function for Rendering the Cube by Triangles
void Display(void) 
{
	glClearColor(0.3, 0, 0.3, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawElements(GL_TRIANGLES, mesh.NF() * 3, GL_UNSIGNED_INT, 0);
	glutSwapBuffers();

}


//Main Func
int main(int argc, char* argv[]) {

	//Creating Window and its Size
	glutInit(&argc, argv);
	//glutInitContextFlags(GLUT_DEBUG);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Window");
	
	//Intializing Glew
	GLenum err = glewInit();

	//Checking GLEW is intialize or not
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}


	//Binding Vertex Shader and FragmentShader
	prog.BuildFiles("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	prog.Bind();


	//Checking the obj is present or not
	if (!mesh.LoadFromFileObj("cube.obj"))
	{
		cout << " Failed to load model";
	}
	
	//Creating Vertex Array Object
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Creating Vertex Buffer Object
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f) * mesh.NV(), &mesh.V(0), GL_STATIC_DRAW);

	//Creating Index Buffer Object
	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.NF() * 3, &mesh.F(0), GL_STATIC_DRAW);

	//Orthogonal Projection Matrix
	float right = 10.0f;
	float left = -10.0f;
	float n = 0.0f;
	float f = 100.0f;
	float top = 10.0f;
	float bottom = -10.0f;

	cy::Vec4f x = { 2 / (right - left),	0,	0, 0 };
	cy::Vec4f y = { 0, 2 / (top - bottom), 0, 0 };
	cy::Vec4f z = { 0, 0, -2 / (f - n),0 };
	cy::Vec4f pos = { -((right + left) / (right - left)), -((top + bottom) / (top - bottom)), -((f + n) / (f - n)), 1 };
	
	Projection.Set(x, y, z, pos);

	cy::Matrix4f View;
	cy::Matrix4f Model;
	cy::Matrix4f mvp;

	//View Matrix
	View = cy::Matrix4f::View(
		cy::Vec3f(0.0f, 2.0f, 2.0f),//camera position
		cy::Vec3f(0.0f, 0.0f, 0.0f),//Target Position
		cy::Vec3f(1.0f, 0.0f, 0.0f) //Normal 
	);

	//Model Veiw Projection Ploting 3D coordinates on 2D coordinates(Screen)
	Model = cy::Matrix4f(1.0f);
	mvp = Projection * View * Model;
	prog.SetUniformMatrix4("mvp", &mvp[0]);
	
	//glut function
	glutReshapeFunc(changeViewPort);
	glutDisplayFunc(Display);
	glutMainLoop();

	return 0;
}