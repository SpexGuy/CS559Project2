/*	A more sophisticated example of modern OpenGL
	Perry Kivolowitz - UW - Madison - CS 559 demo

	In this example program, I will build a solid
	object comprised of two triangle fans. Note 
	these are topologically triangle fans but the
	OpenGL triangle fan functionality is not being
	used. 

	Created:	02/28/13
	Updates:	03/05/13 - continued improvements
				Added solid color shader for drawing normals
				Switched to timer based redisplay.
*/

#include <iostream>
#include <assert.h>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"
#include "Window.h"
#include "View.h"
#include "Camera.h"
#include "Graphics.h"

using namespace std;
using namespace glm;

class Globals {
public:
	Window *window;
	View *view;
	ViewOverlay *overlay;
	PerspectiveProjection *proj;
	SpheroidCamera *cam;
	Model *model;
	Mesh *mars;
	Mesh *cylinder;
	Mesh *sphere;


	int period;
	bool wireframe;

	Globals();
	bool initialize();
	void takeDown();
	virtual ~Globals();
} globals;

Globals::Globals() {
	period = 1000 / 120;
	proj = new PerspectiveProjection(35.0f);
	cam = new SpheroidCamera(proj);
	cam->setRadius(3);
	overlay = new ViewOverlay();
	model = new Model();
	mars = Mesh::newMars(1, 0.08f, "mars_low_rez.txt", true);
	sphere = Mesh::newSphere(10,10, true);
	cylinder = Mesh::newCylinder(10,10,true);

	view = new View(cam, model, overlay);
	window = new SingleViewportWindow(view);
	wireframe = false;
}

void windowReshape(int x, int y);
void windowClose();
void windowDisplay();
void KeyboardFunc(unsigned char c, int x, int y);
void SpecialFunc(int c, int x, int y);
void TimerFunc(int value);

bool Globals::initialize() {
	if (!window->initialize("Mars"))
		return false;
	glutReshapeFunc(windowReshape);
	glutCloseFunc(windowClose);
	glutDisplayFunc(windowDisplay);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutTimerFunc(period, TimerFunc, 0);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);


	if (glewInit() != GLEW_OK) {
		cerr << "GLEW failed to initialize." << endl;
		return false;
	}

	if (!Graphics::inst()->initialize())
		return false;
	if (!mars->initialize())
		return false;
	if (!cylinder->initialize())
		return false;
	if (!sphere->initialize())
		return false;

	return true;
}

void Globals::takeDown() {
	Graphics::inst()->takeDown();
	mars->takeDown();
	cylinder->takeDown();
	sphere->takeDown();
}

Globals::~Globals() {
	delete proj;
	delete cam;
	delete overlay;
	delete model;
	delete view;
	delete window;
	delete mars;
}

class WindowPK
{
public:
	WindowPK()
	{
		this->time_last_pause_began = this->total_time_paused = 0;
		this->normals = this->wireframe = this->paused = false;
		this->slices = 20;
		this->interval = 1000 / 120;
		this->window_handle = -1;
	}

	float time_last_pause_began;
	float total_time_paused;
	bool paused , wireframe, normals;
	int window_handle;
	int interval;
	int slices;
	ivec2 size;
	float window_aspect;
	vector<string> instructions;
};


void CloseFunc() {
	glutLeaveMainLoop();
	globals.takeDown();
}

void KeyboardFunc(unsigned char c, int x, int y) {
	//float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	switch (c)
	{
	case 'w':
		globals.wireframe = !globals.wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, globals.wireframe ? GL_LINE : GL_FILL);
		break;

	case 'n':
		globals.mars->setDrawNormals(!globals.mars->isDrawingNormals());
		break;

	//case 'p':
	//	if (window.paused == true)
	//	{
	//		// Will be leaving paused state
	//		window.total_time_paused += (current_time - window.time_last_pause_began);
	//	}
	//	else
	//	{
	//		// Will be entering paused state
	//		window.time_last_pause_began = current_time;
	//	}
	//	window.paused = !window.paused;
	//	break;

	case 'e':
		globals.mars->position(globals.mars->position() + vec3(0.0f, 0.05f, 0.0f));
		break;
	case 'd':
		globals.mars->position(globals.mars->position() + vec3(0.0f, -0.05f, 0.0f));
		break;

	case 'm':
		globals.model->clearElements();
		globals.model->addElement(globals.mars);
		break;

	case 's':
		globals.model->clearElements();
		globals.model->addElement(globals.sphere);
		break;

	case 'c':
		globals.model->clearElements();
		globals.model->addElement(globals.cylinder);
		break;

	case 'x':
	case 27:
		glutLeaveMainLoop();
		return;
	}
}

void SpecialFunc(int c, int x, int y) {
	switch (c) {		
	case GLUT_KEY_LEFT:
		globals.cam->addAngle(-1.0f);
		break;
	case GLUT_KEY_RIGHT:
		globals.cam->addAngle(1.0f);
		break;
	case GLUT_KEY_UP:
		globals.cam->addAxisAngle(-1.0f);
		break;
	case GLUT_KEY_DOWN:
		globals.cam->addAxisAngle(1.0f);
		break;
	case GLUT_KEY_PAGE_UP:
		globals.proj->addFov(1.0f);
		break;
	case GLUT_KEY_PAGE_DOWN:
		globals.proj->addFov(-1.0f);
		break;
	}
}

void TimerFunc(int value) {
	if (!globals.window->isClosed()) {
		glutTimerFunc(globals.period, TimerFunc, value);
		globals.window->update();
	}
}

void windowReshape(int x, int y) {
	globals.window->reshape(x, y);
}
void windowClose() {
	globals.window->onClose();
	CloseFunc();
}
void windowDisplay() {
	globals.window->render();
}

int main(int argc, char * argv[]) {
	glutInit(&argc, argv);
	glutInitWindowSize(1024, 512);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

	if (!globals.initialize())
		return -1;

	glutMainLoop();
}
