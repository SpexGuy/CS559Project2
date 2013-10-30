#include "Camera.h"



CameraDecorator::~CameraDecorator() {
	if (next != NULL)
		delete next;
}





