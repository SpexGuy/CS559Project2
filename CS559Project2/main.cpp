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
#include "Scene.h"

using namespace std;
using namespace glm;

class Globals {
public:
	char *inFile;
	Window *window;
	View *view;
	ViewOverlay *baseOverlay;
	PerspectiveProjection *proj;
	FreeFlyCamera *flyCam;
	SpheroidCamera *cam;
	Camera *camMars;
	SpheroidCamera *camRocket;
	SpheroidCamera *camStars;

	Camera *currentCamera;

	Camera *chaseCam;
	SpheroidLight *light;
	SpheroidLight *camLight;
	Model *model;
	Model *modelRocket;
	Model *modelStars;
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
	TimeFunction<float> *chaseCamAngle;
	TimeFunction<float> *marsAngle;
	TimeFunction<float> *marsAxisAngle;

	vector<Camera*> cameras;
	vector<Camera*> camerasRocket;
	vector<Camera*> camerasStars;

	vector<Scene*> Scenes;

	Scene *beautyRocket;
	Scene *marsScene;
	Scene *starScene;

	int currentScene;

	int period;
	bool wireframe;
	bool flyMode;

	Globals();
	bool initialize();
	void enterEditMode();
	void exitEditMode();
	void changeCamera();
	void takeDown();
	virtual ~Globals();
} globals;

Globals::Globals() {}

void windowReshape(int x, int y);
void windowClose();
void windowDisplay();
void KeyboardFunc(unsigned char c, int x, int y);
void SpecialFunc(int c, int x, int y);
void TimerFunc(int value);
void PassiveMotionFunc(int x, int y);

bool Globals::initialize() {

	float marsRadius = 1.0f;
	float marsRadScale = 0.03f;
	float rocketScale = marsRadius * 0.3f;
	float distaceRocketMars = marsRadius + 5.0f*marsRadScale;

	period = 1000 / 120;

	proj = new PerspectiveProjection(45.0f);
	proj->setPlanes(0.01f, 100.0f);

	model = new Model();
	modelRocket = new Model();
	modelStars = new Model();

	marsMesh = Mesh::newMars(marsRadius, marsRadScale, inFile, true);
	sphere = Mesh::newSphere(10,10, 1.0f, true);
	cylinder = Mesh::newCylinder(10,10, 0.5f, 0.1f, true);
	rocketMesh = new Rocket();
	rocketMesh->scale(vec3(rocketScale));
	starfieldMesh = PointMesh::newStarField(5000, 8.0f);
	starfield = starfieldMesh
					->disableDepthTest()
					->useMVMode(MV_ROTATION);

	const0 = new ConstantTimeFunction(0.0f);
	const1 = new ConstantTimeFunction(1.0f);
	yAxis = new Vec3TimeFunction(const0, const1, const0);

	orbitAngle = new LinearTimeFunction(16.0f/1000.0f, 0.0f);
	chaseCamAngle = new LinearTimeFunction(-16.0f/1000.0f, 0.0f);
	rocketAngle = new LinearTimeFunction(-13.0f/1000.0f, 0.0f);

	camMars = ((new PointCamera()))
		->animateRotation(model, yAxis, orbitAngle)
		->translated(vec3(distaceRocketMars - marsRadScale,0.0f,0.0f))
		->rotated(vec3(0.0f,0.0f,1.0f),-90.0f);
	camMars->moveUp(-90.0f);
	camMars->moveRight(180.0f);

	cam = new SpheroidCamera();
	cam->setRadius(marsRadius*3.0f);

	camRocket = new SpheroidCamera();
	camRocket->setRadius(rocketScale);

	camStars = new SpheroidCamera();
	camStars->setRadius(16.0f);

	flyCam = new FreeFlyCamera();
	flyCam->setPosition(vec3(0.0f, 0.0f, 3.0f));
	flyCam->setAngle(180.0f);
	
	baseOverlay = new ViewOverlay();
	
	//setup decorator stack to make rocket move specially
	/* This DSL is inspired in part by the Java DSL for Apache's
	 * Camel project.
	 * http://camel.apache.org/java-dsl.html
	 */
	rocket = rocketMesh
					//make rocket orbt
					->animateRotation(model, yAxis, orbitAngle)
					//move rocket out to orbit
					->translated(vec3(distaceRocketMars, 0.0f, 0.0f))
					//make rocket face its direction of motion
					->rotated(vec3(1.0f, 0.0f, 0.0f), -90.0f)
					//make rocket spin on its axis
					->animateRotation(model, yAxis, rocketAngle)
						//store for use with spinning rocket view
						->store(centeredRocket)
					//scale rocket to manageable size
					->scaled(vec3(0.07f, 0.1f, 0.07f))
					->inColor(BLUE)
					->inMaterial(0.1f, vec4(1.0f), 40.0f);

	BoundedSpheroidCamera* bsc = new BoundedSpheroidCamera();
	chaseCam = bsc					
		//all rotations of the camera go BACKWARDS
		->animateRotation(model, yAxis, orbitAngle)
		//so do translations
		->translated(vec3(distaceRocketMars, 0.0f, 0.0f))
		//make the axis face out
		->scaled(vec3(rocketScale))
		->rotated(vec3(0.0f, 0.0f, 1.0f), -90.0f);

	//mars must spin twice as fast since its axis is spinning in the opposite direction.
	marsAngle = new LinearTimeFunction(12.0f/1000.0f, 0.0f);
	marsAxisAngle = new LinearTimeFunction(-6.0f/1000.0f, 0.0f);

	//setup decorator stack for mars
	mars = marsMesh
					//make mars' axis spin
					->animateRotation(model, yAxis, marsAxisAngle)
					//tilt mars' axis off of y
					->rotated(vec3(1.0f, 0.0f, 0.0f), 15.0f)
					//make mars spin on its axis
					->animateRotation(model, yAxis, marsAngle)
					->inColor(MARS)
					->inMaterial(0.1f, vec4(0.0f), 1.0f);
	
	light = new SpheroidLight();

	light->setAngle(90);
	light->setAxisAngle(90);
	light->setRadius(15);	

	model->addLight(light);
	//the bases are at the top of decorator stacks.
	model->addElement(rocket);
	model->addElement(mars);
	//this pushes the starfield to the beginning of the model, ensuring that it is drawn behind everything else despite the depth buffer.
	model->addLight(starfield);

	modelRocket->addLight(light);
	modelRocket->addElement(centeredRocket);

	modelStars->addElement(starfieldMesh);

	cameras.push_back(camMars);
	cameras.push_back(flyCam);
	cameras.push_back(cam);
	cameras.push_back(chaseCam);
	camerasRocket.push_back(camRocket);
	camerasStars.push_back(camStars);

	marsScene = new Scene(cameras, model, baseOverlay);
	beautyRocket = new Scene(camerasRocket, modelRocket, baseOverlay);
	starScene = new Scene(camerasStars, modelStars, baseOverlay);

	Scenes.push_back(marsScene);
	Scenes.push_back(beautyRocket);
	Scenes.push_back(starScene);
	currentScene = 0;
	currentCamera = Scenes[0]->getCamera();

	view = new View(proj, currentCamera, model, baseOverlay);

	window = new SingleViewportWindow(view);
	wireframe = false;
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

	exitEditMode();

	return true;
}

void Globals::enterEditMode() {
	if (globals.rocketMesh->getEditMode())
		return;
	globals.rocketMesh->setEditMode(true);
	//view->setOverlay(splineOverlay);
}

void Globals::exitEditMode() {
	if (!globals.rocketMesh->getEditMode())
		return;
	globals.rocketMesh->setEditMode(false);
	//view->setOverlay(baseOverlay);
}

void Globals::changeCamera()
{
	if(Scenes[currentScene]->endOfCameraList())
	{
		Scenes[currentScene]->resetCameraIndex();
		currentScene++;
		currentScene %= Scenes.size();
	}
	else
	{
		Scenes[currentScene]->changeCamera();
	} 
	view->setCamera(Scenes[currentScene]->getCamera());
	view->setOverlay(Scenes[currentScene]->getOverLay());
	view->setModel(Scenes[currentScene]->getModel());

	currentCamera = Scenes[currentScene]->getCamera();
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
	delete chaseCamAngle;
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
	
	if(!globals.Scenes[0]->getCurrentCameraIndex() == 1 || (deltaX == 0 && deltaY == 0) ) return;

	globals.flyCam->addAngle(-deltaX/10.0f);
	globals.flyCam->addAxisAngle(deltaY/10.0f);

	if(x <= 10 || (y) <= 10 || x >= size.x-10 || y >= size.y-10) {
		lastX = centerx;
		lastY = centery;
		glutWarpPointer( lastX, lastY );
	}
	
}

void KeyboardFunc(unsigned char c, int x, int y) {
	bool normals;
	Drawable *newHead;

	if (globals.rocketMesh->getEditMode()) {
		switch(c) {
		case '2':
			globals.rocketMesh->getEditor()->moveHorizontal(-0.005f);
			break;
		case '8':
			globals.rocketMesh->getEditor()->moveHorizontal(0.005f);
			break;
		case '6':
			globals.rocketMesh->getEditor()->moveVertical(-0.005f);
			break;
		case '4':
			globals.rocketMesh->getEditor()->moveVertical(0.005f);
			break;
		case '7':
			globals.rocketMesh->getEditor()->addAngle(1);
			break;
		case '9':
			globals.rocketMesh->getEditor()->addAngle(-1);
			break;
		case '1':
			globals.rocketMesh->getEditor()->addSize(0.005f);
			break;
		case '3':
			globals.rocketMesh->getEditor()->addSize(-0.005f);
			break;
		case '5':
			globals.rocketMesh->getEditor()->next();
			break;
		}
	}

	switch (c)
	{
	case 'r':
		globals.flyCam->setPosition(vec3(0.0f, 0.0f, 3.0f));
		globals.flyCam->setAngle(180.0f);
		globals.flyCam->setAxisAngle(90.0f);

		break;

	case 'w':
		globals.wireframe = !globals.wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, globals.wireframe ? GL_LINE : GL_FILL);
		break;

	case 'n':
		normals = !globals.marsMesh->isDrawingNormals();
		globals.marsMesh->setDrawNormals(normals);
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

	case '.':
		globals.exitEditMode();
		break;

	case '0':
		globals.enterEditMode();
		newHead = Mesh::newSurfaceOfRotation(globals.rocketMesh->getEditor()->getSpline(30), 30, true)
					->translated(vec3(0.0f, -2.0f, 0.0f))
					->scaled(vec3(4.0f, 4.0f, 4.0f));
		newHead->initialize();
		globals.rocketMesh->replaceHead(newHead);
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
			globals.changeCamera();
			break;

		case GLUT_KEY_CTRL_R:
			globals.currentCamera ->moveForward(1.0f);
			break;
		case GLUT_KEY_CTRL_L:
			globals.currentCamera ->moveForward(-1.0f);
			break;
		case GLUT_KEY_LEFT:
			globals.currentCamera ->moveRight(-1.0f);
			break;
		case GLUT_KEY_RIGHT:
			globals.currentCamera ->moveRight(1.0f);

			break;
		case GLUT_KEY_UP:
			globals.currentCamera ->moveUp(1.0f);

			break;
		case GLUT_KEY_DOWN:
			globals.currentCamera ->moveUp(-1.0f);
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
	if( argc < 2 )
	{
		globals.inFile = "mars_low_rez.txt";
	}
	else
	{
		globals.inFile = argv[1];
	}
	glutInitWindowSize(1024, 512);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

	if (!globals.initialize())
		return -1;

	glutMainLoop();
}
