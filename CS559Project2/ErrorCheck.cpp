#include <iostream>
#include "ErrorCheck.h"
#include <GL/freeglut.h>

using namespace std;

/* checks for an error and prints it */
bool checkError(char * s) {
	bool return_error = false;
#ifdef _DEBUG
	GLenum glerror;

	while ((glerror = glGetError()) != GL_NO_ERROR)
	{
		return_error = true;
		cerr << s << ": " << gluErrorString(glerror) << endl;
	}
#endif
	return return_error;
}
