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
#include "SpheroidLight.h"
#include "SplineEditor.h"
#include "Graphics.h"
#include "Rocket.h"
#include "Function.h"
#include "Animation.h"

using namespace std;
using namespace glm;

class OverlayTest : public ViewOverlay {
public:
	mat4 draw();
};

class Globals {
public:
	Window *window;
	View *view;
	ViewOverlay *baseOverlay;
	SplineEditorOverlay *splineOverlay;
	PerspectiveProjection *proj;
	FreeFlyCamera *flyCam;
	SpheroidCamera *cam;
	SpheroidLight *light;
	Model *model;
	Mesh *mars;
	Mesh *cylinder;
	Mesh *sphere;
	Rocket *rocket;

	Animation *marsAnim;
	TimeFunction<float> *marsAngle;
	TimeFunction<vec3> *marsAxis;
	TimeFunction<float> *const0;
	TimeFunction<float> *const1;

	int period;
	bool wireframe;
	bool editMode;
	bool flyMode;

	Globals();
	bool initialize();
	void enterEditMode();
	void exitEditMode();
	void enterFreeflyMode();
	void exitFreeflyMode();
	void takeDown();
	virtual ~Globals();
} globals;

Globals::Globals() {
	period = 1000 / 120;
	proj = new PerspectiveProjection(45.0f);
	cam = new SpheroidCamera(proj);
	cam->setRadius(3);
	flyCam = new FreeFlyCamera(proj);
	flyCam->setPosition(vec3(0.0f, 0.0f, 3.0f));
	flyCam->setAngle(180);
	splineOverlay = new SplineEditorOverlay(5);
	baseOverlay = new ViewOverlay();
	model = new Model();
	mars = Mesh::newMars(1, 0.08f, "mars_low_rez.txt", true);
	sphere = Mesh::newSphere(10,10, 1.0f, true);
	cylinder = Mesh::newCylinder(10,10, 0.2f, 0.1f, true);
	rocket = new Rocket();

	light = new SpheroidLight();
	light->setAngle(45);
	light->setAxisAngle(45);
	light->setRadius(5);

	const0 = new ConstantTimeFunction(0.0f);
	const1 = new ConstantTimeFunction(1.0f);
	marsAngle = new LinearTimeFunction(6.0f/1000.0f, 0.0f);
	marsAxis = new Vec3TimeFunction(const0, const1, const0);
	marsAnim = new RotationAnimation(mars, marsAxis, marsAngle);

	model->addLight(light);
	model->addElement(mars);
	model->addAnimation(marsAnim);

	view = new View(flyCam, model, baseOverlay);
	window = new SingleViewportWindow(view);
	wireframe = false;
}

void windowReshape(int x, int y);
void windowClose();
void windowDisplay();
void KeyboardFunc(unsigned char c, int x, int y);
void SpecialFunc(int c, int x, int y);
void TimerFunc(int value);
void PassiveMotionFunc(int x, int y);

bool Globals::initialize() {
	if (!window->initialize("Mars"))
		return false;
	glutReshapeFunc(windowReshape);
	glutCloseFunc(windowClose);
	glutDisplayFunc(windowDisplay);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutTimerFunc(period, TimerFunc, 0);
	glutPassiveMotionFunc(PassiveMotionFunc);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	


	if (glewInit() != GLEW_OK) {
		cerr << "GLEW failed to initialize." << endl;
		return false;
	}

	if (!ShaderFlyweight::inst()->initialize())
		return false;
	if (!Graphics::inst()->initialize())
		return false;
	if (!mars->initialize())
		return false;
	if (!cylinder->initialize())
		return false;
	if (!sphere->initialize())
		return false;
	if (!rocket->initialize())
		return false;

	editMode = true;
	flyMode = true;
	exitFreeflyMode();
	exitEditMode();

	return true;
}

void Globals::enterEditMode() {
	if (editMode)
		return;
	editMode = true;
	view->setOverlay(splineOverlay);
}

void Globals::exitEditMode() {
	if (!editMode)
		return;
	editMode = false;
	view->setOverlay(baseOverlay);
}

void Globals::enterFreeflyMode() {
	if (flyMode)
		return;
	flyMode = true;
	view->setCamera(flyCam);
}

void Globals::exitFreeflyMode() {
	if (!flyMode)
		return;
	flyMode = false;
	view->setCamera(cam);
}

void Globals::takeDown() {
	mars->takeDown();
	cylinder->takeDown();
	sphere->takeDown();
	rocket->takeDown();
	Graphics::inst()->takeDown();
	ShaderFlyweight::inst()->takeDown();
}

Globals::~Globals() {
	delete proj;
	delete cam;
	delete flyCam;
	delete splineOverlay;
	delete model;
	delete view;
	delete window;
	
	delete mars;
	delete cylinder;
	delete sphere;

	delete const0;
	delete const1;
	delete marsAngle;
	delete marsAxis;
	delete marsAnim;
}

//@Deprecated
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

int lastX = 150;
int lastY = 150;
/** this function is adapted from a post by Steven Canfield
 * on StackOverflow.com:
 * http://stackoverflow.com/questions/728049/glutpassivemotionfunc-and-glutwarpmousepointer */
void PassiveMotionFunc(int x, int y) {
	int deltaX = x - lastX;
	int deltaY = y - lastY;

	lastX = x;
	lastY = y;

	ivec2 size = Graphics::inst()->getSize();
	int centerx = size.x/2;
	int centery = size.y/2;

	if( !globals.flyMode || (deltaX == 0 && deltaY == 0) ) return;

	globals.flyCam->addAngle(-deltaX/10.0f);
	globals.flyCam->addAxisAngle(deltaY/10.0f);

	if( x <= 10 || (y) <= 10 || x >= size.x-10 || y >= size.y-10) {
		lastX = centerx;
		lastY = centery;
		glutWarpPointer( lastX, lastY );
	}
}

void KeyboardFunc(unsigned char c, int x, int y) {
	//float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
	bool normals;

	if (globals.editMode) {
		switch(c) {
		case '4':
			globals.splineOverlay->moveHorizontal(-0.005);
			break;
		case '6':
			globals.splineOverlay->moveHorizontal(0.005);
			break;
		case '2':
			globals.splineOverlay->moveVertical(-0.005);
			break;
		case '8':
			globals.splineOverlay->moveVertical(0.005);
			break;
		case '7':
			globals.splineOverlay->addAngle(1);
			break;
		case '9':
			globals.splineOverlay->addAngle(-1);
			break;
		case '1':
			globals.splineOverlay->addSize(0.005);
			break;
		case '3':
			globals.splineOverlay->addSize(-0.005);
			break;
		case '5':
			globals.splineOverlay->next();
			break;
		}
	}

	switch (c)
	{
	case 'w':
		globals.wireframe = !globals.wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, globals.wireframe ? GL_LINE : GL_FILL);
		break;

	case 'n':
		normals = !globals.mars->isDrawingNormals();
		globals.mars->setDrawNormals(normals);
		globals.cylinder->setDrawNormals(normals);
		globals.sphere->setDrawNormals(normals);
		break;

	case 'p':
		if (globals.model->isPaused())
			globals.model->play();
		else
			globals.model->pause();
		break;
	case ';':
		globals.light->addAngle(1);
		break;
	case 'k':
		globals.light->addAngle(-1);
		break;
	case 'o':
		globals.light->addAxisAngle(-1);
		break;
	case 'l':
		globals.light->addAxisAngle(1);
		break;

	case 'm':
		globals.model->clearElements();
		globals.model->addLight(globals.light);
		globals.model->addElement(globals.mars);
		break;

	case '.':
		globals.exitEditMode();
		break;

	case '0':
		globals.enterEditMode();
		globals.sphere->takeDown();
		delete globals.sphere;
		globals.sphere = Mesh::newSurfaceOfRotation(globals.splineOverlay->getSpline(30), 30, true);
		globals.sphere->initialize();
		//no break here.
	case 'a':
		globals.model->clearElements();
		globals.model->addLight(globals.light);
		globals.model->addElement(globals.sphere);
		break;

	case 'c':
		globals.model->clearElements();
		globals.model->addLight(globals.light);
		globals.model->addElement(globals.cylinder);
		break;
	case 'r':
		globals.model->clearElements();
		globals.model->addLight(globals.light);
		globals.model->addElement(globals.rocket);
		break;

	case 'z':
		if (globals.flyMode)
			globals.exitFreeflyMode();
		else
			globals.enterFreeflyMode();
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
		if(globals.flyMode)
		{
			globals.flyCam->moveRight(-0.05f);
		} else {
			globals.cam->addAngle(-1.0f);
		}
		break;
	case GLUT_KEY_RIGHT:
		if(globals.flyMode)
		{
			globals.flyCam->moveRight(0.05f);
		} else {
			globals.cam->addAngle(1.0f);
		}
		break;
	case GLUT_KEY_UP:
		if(globals.flyMode)
		{
			globals.flyCam->moveForward(0.05f);
		} else {
			globals.cam->addAxisAngle(-1.0f);
		}
		break;
	case GLUT_KEY_DOWN:
		if(globals.flyMode)
		{
			globals.flyCam->moveForward(-0.05f);
		} else {
			globals.cam->addAxisAngle(1.0f);
		}
		break;
	case GLUT_KEY_CTRL_L:
		if(globals.flyMode)
		{
			globals.flyCam->moveUp(0.05f);
		}
		break;

	case GLUT_KEY_CTRL_R:
		if(globals.flyMode)
		{
			globals.flyCam->moveUp(-0.05f);
		}
		break;

	case GLUT_KEY_F11:
		globals.window->toggleFullscreen();
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
		int time = glutGet(GLUT_ELAPSED_TIME);
		globals.model->update(time);
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
