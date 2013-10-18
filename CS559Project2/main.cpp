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
#include "Function.h"
#include "Animation.h"

using namespace std;
using namespace glm;

class OverlayTest : public ViewOverlay {
public:
	mat4 draw();
};

mat4 OverlayTest::draw() {
	setupCamera();
	mat4 base(1.0f);
	Graphics *g = Graphics::inst();
	ivec2 size = g->getSize();
	g->setColor(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	g->drawRect2D(base, 0, 0, float(size.x), size.y/4.0f);
	g->setColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	g->drawLine2D(base, vec2(200, 100), vec2(100, 200));
	g->setColor(vec4(0.0f, 1.0f, 0.0f, 0.0f));
	g->drawCircle2D(base, 100, 100, 50);

	return base;
}

class Globals {
public:
	Window *window;
	View *view;
	ViewOverlay *baseOverlay;
	SplineEditorOverlay *splineOverlay;
	PerspectiveProjection *proj;
	SpheroidCamera *cam;
	SpheroidLight *light;
	Model *model;
	Mesh *mars;
	Mesh *cylinder;
	Mesh *sphere;

	Animation *marsAnim;
	TimeFunction<float> *marsAngle;
	TimeFunction<vec3> *marsAxis;
	TimeFunction<float> *const0;
	TimeFunction<float> *const1;

	int period;
	bool wireframe;
	bool editMode;

	Globals();
	bool initialize();
	void enterEditMode();
	void exitEditMode();
	void takeDown();
	virtual ~Globals();
} globals;

Globals::Globals() {
	period = 1000 / 120;
	proj = new PerspectiveProjection(45.0f);
	cam = new SpheroidCamera(proj);
	cam->setRadius(3);
	splineOverlay = new SplineEditorOverlay(5);
	baseOverlay = new ViewOverlay();
	model = new Model();

	light = new SpheroidLight();
	light->setAngle(45);
	light->setAxisAngle(45);
	light->setRadius(5);

	mars = Mesh::newMars(1, 0.04f, "mars_low_rez.txt", true);
	sphere = Mesh::newSphere(10,10, true);
	cylinder = Mesh::newCylinder(10,10,true);

	const0 = new ConstantTimeFunction(0.0f);
	const1 = new ConstantTimeFunction(1.0f);
	marsAngle = new LinearTimeFunction(6.0f/1000.0f, 0.0f);
	marsAxis = new Vec3TimeFunction(const0, const1, const0);
	marsAnim = new RotationAnimation(mars, marsAxis, marsAngle);

	model->addLight(light);
	model->addElement(mars);
	model->addAnimation(marsAnim);

	editMode = false;

	view = new View(cam, model, baseOverlay);
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

void Globals::takeDown() {
	mars->takeDown();
	cylinder->takeDown();
	sphere->takeDown();
	Graphics::inst()->takeDown();
	ShaderFlyweight::inst()->takeDown();
}

Globals::~Globals() {
	delete proj;
	delete cam;
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

	case 'e':
		globals.mars->position(globals.mars->position() + vec3(0.0f, 0.05f, 0.0f));
		break;
	case 'd':
		globals.mars->position(globals.mars->position() + vec3(0.0f, -0.05f, 0.0f));
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
	case 's':
		globals.model->clearElements();
		globals.model->addLight(globals.light);
		globals.model->addElement(globals.sphere);
		break;

	case 'c':
		globals.model->clearElements();
		globals.model->addLight(globals.light);
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

	case GLUT_KEY_F11:
		globals.window->toggleFullscreen();
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
