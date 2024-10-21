#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);