/* This class was written by Perry Kivolowitz */
#pragma once
#undef	_UNICODE
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#define	BAD_IL_VALUE	IL_INVALID_VALUE
#ifndef BAD_GL_VALUE
#define	BAD_GL_VALUE	GLuint(-1)
#endif

/** This class represents an image loaded using DevIL
 * which is used as a texture */
class ILContainer {
private:
	/* each ILContainer is assigned a unique, incrementing index */
	static int nextIndex;
public:
	ILContainer() {
		this->il_handle = BAD_IL_VALUE;
		this->il_texture_handle = BAD_GL_VALUE;
		this->index = nextIndex++;
	}

	~ILContainer();
	
	/* loads the texture and registers it with GL */
	bool initialize(const char * file_name);
	/* binds the texture to its index */
	void bind();
	/* releases any GL resources */
	void takeDown();

	int index;
	ILint width;
	ILint height;
	ILint format;
	ILuint il_handle;
	GLuint il_texture_handle;
	GLvoid * data;
};
