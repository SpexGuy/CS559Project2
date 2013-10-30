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
#include "P2Cameras.h"
#include "Projection.h"
#include "SpheroidLight.h"
#include "SplineEditor.h"
#include "Graphics.h"
#include "Rocket.h"
#include "Function.h"
#include "Animation.h"
#include "PointMesh.h"

using namespace std;
using namespace glm;

class Globals {
public:
	Window *window;
	View *view;
	ViewOverlay *baseOverlay;
	SplineEditorOverlay *splineOverlay;
	PerspectiveProjection *proj;
	FreeFlyCamera *flyCam;
	SpheroidCamera *cam;
	Camera *camMars;
	SpheroidLight *light;
	Model *model;
	Mesh *marsMesh;
	Mesh *cylinder;
	Mesh *sphere;
	PointMesh *starfieldMesh;
	Rocket *rocketMesh;

	Drawable *starfield;
	Drawable *rocket;
	Drawable *centeredRocket;
	Drawable *mars;

	TimeFunction<float> *const0;
	TimeFunction<float> *const1;
	TimeFunction<vec3> *yAxis;

	TimeFunction<float> *rocketAngle;
	TimeFunction<float> *orbitAngle;
	TimeFunction<float> *marsAngle;
	TimeFunction<float> *marsAxisAngle;

	RotationAnimation *camMarsAni;


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

	float marsRadius = 1.0f;
	float marsRadScale = 0.04f;
	period = 1000 / 120;

	proj = new PerspectiveProjection(45.0f);
	proj->setPlanes(0.01f, 100.0f);
	
	//camMars = new MarsCamera(marsRadius + marsRadScale*10.0f);
	cam = new SpheroidCamera();
	cam->setRadius(1.3f);
	camMars = new CamRotation(cam);
	((CamRotation *) camMars)->setRotation(vec3(0.0f, 0.0f, 1.0f), -90.0f);
	camMars = new CamTranslation(camMars);
	((CamTranslation *) camMars)->position(vec3(1.3f, 0.0f, 0.0f));
	camMars = new CamRotation(camMars);
	
	flyCam = new FreeFlyCamera();
	flyCam->setPosition(vec3(0.0f, 0.0f, 3.0f));
	flyCam->setAngle(180.0f);
	
	splineOverlay = new SplineEditorOverlay(5);
	baseOverlay = new ViewOverlay();

	model = new Model();
	marsMesh = Mesh::newMars(marsRadius, marsRadScale, "mars_low_rez.txt", true);
	sphere = Mesh::newSphere(10,10, 1.0f, true);
	cylinder = Mesh::newCylinder(10,10, 0.5f, 0.1f, true);
	rocketMesh = new Rocket();
	starfieldMesh = PointMesh::newStarField(10000, 8.0f);
	starfield = starfieldMesh->disableDepthTest();

	const0 = new ConstantTimeFunction(0.0f);
	const1 = new ConstantTimeFunction(1.0f);
	yAxis = new Vec3TimeFunction(const0, const1, const0);

	orbitAngle = new LinearTimeFunction(16.0f/1000.0f, 0.0f);
	rocketAngle = new LinearTimeFunction(-13.0f/1000.0f, 0.0f);

	//setup decorator stack to make rocket move specially
	/* This DSL is inspired in part by the Java DSL for Apache's
	 * Camel project.
	 * http://camel.apache.org/java-dsl.html
	 */
	rocket = rocketMesh
					//make rocket spin
					->animateRotation(model, yAxis, orbitAngle)
					//move rocket out to orbit
					->translated(vec3((marsRadius + marsRadScale) * 1.5f, 0.0f, 0.0f))
					//make rocket face its direction of motion
					->rotated(vec3(1.0f, 0.0f, 0.0f), -90.0f)
					//make rocket spin on its axis
					->animateRotation(model, yAxis, rocketAngle)
						//store for use with spinning rocket view
						->store(centeredRocket)
					//scale rocket to manageable size
					->scaled(vec3(0.07f, 0.1f, 0.07f));

	//mars must spin twice as fast since its axis is spinning in the opposite direction.
	marsAngle = new LinearTimeFunction(12.0f/1000.0f, 0.0f);
	marsAxisAngle = new LinearTimeFunction(-6.0f/1000.0f, 0.0f);

	//setup decorator stack for mars
	mars = marsMesh
					//make mars' axis spin
					//->animateRotation(model, yAxis, marsAxisAngle)
					//tilt mars' axis off of y
					//->rotated(vec3(1.0f, 0.0f, 0.0f), 15.0f)
					//make mars spin on its axis
					//->animateRotation(model, yAxis, marsAngle)
					;
	camMarsAni = new RotationAnimation((CamRotation *)camMars, yAxis, rocketAngle);
	model->addAnimation(camMarsAni);

	light = new SpheroidLight();

	light->setAngle(90);
	light->setAxisAngle(90);
	light->setRadius(5);	

	model->addLight(light);
	//the bases are at the top of decorator stacks.
	model->addElement(rocket);
	model->addElement(mars);
	//this pushes the starfield to the beginning of the model, ensuring that it is drawn behind everything else despite the depth buffer.
	model->addLight(starfield);

	view = new View(proj, flyCam, model, baseOverlay);
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
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	


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
	if (!starfield->initialize())
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
	view->setCamera(camMars);
}

void Globals::takeDown() {
	mars->takeDown();
	cylinder->takeDown();
	sphere->takeDown();
	rocket->takeDown();
	starfield->takeDown();
	Graphics::inst()->takeDown();
	ShaderFlyweight::inst()->takeDown();
}

Globals::~Globals() {
	delete proj;
	delete flyCam;
	delete splineOverlay;
	delete camMars;
	delete model;
	delete view;
	delete window;
	
	delete cylinder;
	delete sphere;
	delete starfield;

	delete rocket;
	delete mars;

	delete const0;
	delete const1;
	delete yAxis;

	delete marsAngle;
	delete marsAxisAngle;
	delete rocketAngle;
	delete orbitAngle;
}

void CloseFunc() {
	glutLeaveMainLoop();
	globals.takeDown();
}

/** this function is adapted from a post by Steven Canfield
 * on StackOverflow.com:
 * http://stackoverflow.com/questions/728049/glutpassivemotionfunc-and-glutwarpmousepointer
 */
int lastX = 150;
int lastY = 150;
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
	bool normals;
	Mesh *newHead;

	if (globals.editMode) {
		switch(c) {
		case '4':
			globals.splineOverlay->moveHorizontal(-0.005f);
			break;
		case '6':
			globals.splineOverlay->moveHorizontal(0.005f);
			break;
		case '2':
			globals.splineOverlay->moveVertical(-0.005f);
			break;
		case '8':
			globals.splineOverlay->moveVertical(0.005f);
			break;
		case '7':
			globals.splineOverlay->addAngle(1);
			break;
		case '9':
			globals.splineOverlay->addAngle(-1);
			break;
		case '1':
			globals.splineOverlay->addSize(0.005f);
			break;
		case '3':
			globals.splineOverlay->addSize(-0.005f);
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
		normals = !globals.marsMesh->isDrawingNormals();
		globals.marsMesh->setDrawNormals(normals);
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
		globals.model->addElement(globals.starfield);

		globals.model->addElement(globals.mars);

		break;

	case '.':
		globals.exitEditMode();
		break;

	case '0':
		globals.enterEditMode();
		newHead = Mesh::newSurfaceOfRotation(globals.splineOverlay->getSpline(30), 30, true);
		newHead->initialize();
		globals.rocketMesh->replaceHead(newHead);
		break;

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
		globals.model->addElement(globals.centeredRocket);

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
		case GLUT_KEY_F1:
		{
			globals.model->clearElements();
			globals.model->addLight(globals.light);
			globals.model->addElement(globals.mars);

			globals.model->addElement(globals.rocket);
		}

		case GLUT_KEY_CTRL_R:
			if (globals.flyMode) 
			{
				globals.flyCam->moveUp(-0.05f);
			}
			break;
		case GLUT_KEY_CTRL_L:
			if (globals.flyMode) 
			{
				globals.flyCam->moveUp(0.05f);
			}
			break;
		case GLUT_KEY_LEFT:
			if (globals.flyMode) 
			{
				globals.flyCam->moveRight(-0.05f);
			} else {
				globals.cam->addAngle(-1.0f);
			}
			break;
		case GLUT_KEY_RIGHT:
			if (globals.flyMode) 
			{
				globals.flyCam->moveRight(0.05f);
			} else {
				globals.cam->addAngle(1.0f);
			}
			break;
		case GLUT_KEY_UP:
			if (globals.flyMode) 
			{
				globals.flyCam->moveForward(0.05f);
			} else {
				globals.cam->addAxisAngle(-1.0f);
			}
			break;
		case GLUT_KEY_DOWN:
			if (globals.flyMode) 
			{
				globals.flyCam->moveForward(-0.05f);
			} else {
				globals.cam->addAxisAngle(1.0f);
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
