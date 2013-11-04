/* This class was written by Perry Kivolowitz */
#include <assert.h>
#include "ilcontainer.h"

//a static counter to assign unique indexes
int ILContainer::nextIndex = 0;

bool ILContainer::initialize(const char * file_name)
{
	// We are asserting that we have not initialized this object before.
	assert(this->il_handle == BAD_IL_VALUE);

	if ((this->il_handle = ilGenImage()) == BAD_IL_VALUE)
		return false;
	ilBindImage(this->il_handle);
	if (!ilLoadImage(file_name))
		return false;

	glGenTextures(1, &this->il_texture_handle);
	glBindTexture(GL_TEXTURE_2D, this->il_texture_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), this->width = ilGetInteger(IL_IMAGE_WIDTH), this->height = ilGetInteger(IL_IMAGE_HEIGHT), 0, this->format = ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, this->data = ilGetData());
	return true;
}

void ILContainer::bind()
{
	// We are asserting that we have initialized this object before.
	assert(this->il_handle != BAD_IL_VALUE);

	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, this->il_texture_handle);
}

void ILContainer::takeDown()
{
	//free resources
	if (this->il_texture_handle != BAD_GL_VALUE)
		glDeleteTextures(1, &this->il_texture_handle);
	if (this->il_handle != BAD_IL_VALUE)
		ilDeleteImage(this->il_handle);

	this->il_handle = BAD_IL_VALUE;
	this->il_texture_handle = BAD_GL_VALUE;
}

ILContainer::~ILContainer() {
	//assert that takeDown has been called
	assert(il_handle == BAD_IL_VALUE);
	assert(il_texture_handle == BAD_GL_VALUE);
}
