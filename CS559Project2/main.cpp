#include <iostream>
#include <assert.h>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
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
#include "ilcontainer.h"

using namespace std;
using namespace glm;

class Globals {
public:
	char *inFile;

	Window *window;
	View *view;

	HudOverlay *mainOverlay;
	HudOverlay *editorOverlay;
	HudOverlay *starOverlay;

	PerspectiveProjection *proj;

	FreeFlyCamera *flyCam;
	SpheroidCamera *cam;
	Camera *camMars;
	SpheroidCamera *camRocket;
	SpheroidCamera *camStars;
	Camera *currentCamera;
	Camera *chaseCam;

	SpheroidLight *light;

	Model *model;
	Model *modelRocket;
	Model *modelStars;

	Mesh *marsMesh;
	PointMesh *starfieldMesh;
	Rocket *rocketMesh;

	Drawable *starfield;
	Drawable *rocket;
	Drawable *centeredRocket;
	Drawable *mars;

	ILContainer *marsTexture;

	TimeFunction<float> *const0;
	TimeFunction<float> *const1;
	TimeFunction<vec3> *yAxis;

	TimeFunction<float> *rocketAngle;
	TimeFunction<float> *orbitAngle;
	TimeFunction<float> *marsAngle;
	TimeFunction<float> *marsAxisAngle;

	vector<Camera*> cameras;
	vector<Camera*> camerasRocket;
	vector<Camera*> camerasStars;

	vector<Scene*> Scenes;

	Scene *beautyRocket;
	Scene *marsScene;
	Scene *starScene;

	int lastUpdateTime;
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
	lastUpdateTime  = 0;
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

	marsTexture = new ILContainer();
	marsMesh = Mesh::newMars(marsRadius, marsRadScale, inFile, marsTexture, true);
	rocketMesh = new Rocket();
	rocketMesh->scale(vec3(rocketScale));
	starfieldMesh = PointMesh::newStarField(8000, 8.0f);
	starfield = starfieldMesh
					->disableDepthTest()
					->useMVMode(MV_ROTATION);

	const0 = new ConstantTimeFunction(0.0f);
	const1 = new ConstantTimeFunction(1.0f);
	yAxis = new Vec3TimeFunction(const0, const1, const0);

	orbitAngle = new LinearTimeFunction(16.0f/1000.0f, 0.0f);
	rocketAngle = new LinearTimeFunction(-13.0f/1000.0f, 0.0f);

	camMars = ((new PointCamera()))
		->rotated(vec3(0.0f, 0.0f, 1.0f), 30)
		->animateRotation(model, yAxis, orbitAngle)
		->translated(vec3(distaceRocketMars - marsRadScale - 0.01f, 0.0f, 0.0f))
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
		
	//setup decorator stack to make rocket move specially
	/* This DSL is inspired in part by the Java DSL for Apache's
	 * Camel project.
	 * http://camel.apache.org/java-dsl.html
	 */
	rocket = rocketMesh
					->rotated(vec3(0.0f, 0.0f, 1.0f), 30)
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

	BoundedSpheroidCamera* bsc = new BoundedSpheroidCamera(360.0f, 25.0f);
	chaseCam = bsc					
		->rotated(vec3(0.0f, 0.0f, 1.0f), 30)
		//rotate the camera with the ship
		->animateRotation(model, yAxis, orbitAngle)
		//make it orbit
		->translated(vec3(distaceRocketMars, 0.0f, 0.0f))
		//make it the right size
		->scaled(vec3(rocketScale))
		//make the axis face out
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

	vector<char*> text;
	text.push_back("Image credit: http://openuniverse.sourceforge.net/");
	text.push_back("");
	text.push_back("Left/Right UP/DOWN PgUP/PgDn - Camera Controls");
	text.push_back("O/L ;/K - Light Controls");
	text.push_back("W - wireframe");
	text.push_back("P - Pause");
	text.push_back("N - Normals");
	text.push_back("F1 - Cycle Camera/Scene");
	text.push_back("F11 - Full Screen");
	text.push_back("");
	text.push_back("MARS MODE");

	vector<char*> star;
	star.push_back("Left/Right UP/DOWN PgUP/PgDn - Camera Controls");
	star.push_back("W - Wireframe");
	star.push_back("P - Pause");
	star.push_back("N - Normals");
	star.push_back("F1 - Cycle Camera/Scene");
	star.push_back("F11 - Flull Screen");
	star.push_back("");
	star.push_back("STAR MODE");
	
	vector<char*> edit;
	edit.push_back("Left/Right UP/DOWN PgUP/PgDn - Camera Controls");
	edit.push_back("O/L ;/K - Light Controls");
	edit.push_back("0  - Open Editing / Commit Change");
	edit.push_back("1/3 - Scale");
	edit.push_back("2/8 - Move Point Vertical");
	edit.push_back("4/6 - Move Point Horizontal");
	edit.push_back("5  - Next Point");
	edit.push_back("7/9 - Rotate Point");
	edit.push_back(".  - Close Editing");
	edit.push_back("W - Wireframe");
	edit.push_back("P - Pause");
	edit.push_back("N - Normals");
	edit.push_back("F1 - Cycle Camera/Scene");
	edit.push_back("F11 - Flull Screen");
	edit.push_back("");
	edit.push_back("EDIT MODE");

	mainOverlay = new HudOverlay(text);

	editorOverlay = new HudOverlay(edit);

	starOverlay = new HudOverlay(star);

	marsScene = new Scene(cameras, model, mainOverlay);
	beautyRocket = new Scene(camerasRocket, modelRocket, editorOverlay);
	starScene = new Scene(camerasStars, modelStars, starOverlay);
	Scenes.push_back(marsScene);
	Scenes.push_back(beautyRocket);
	Scenes.push_back(starScene);
	currentScene = 0;
	currentCamera = Scenes[0]->getCamera();
	
	view = new View(proj, currentCamera, model, mainOverlay);

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
	if (!marsTexture->initialize("mars_texture.jpg"))
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
}

void Globals::exitEditMode() {
	if (!globals.rocketMesh->getEditMode())
		return;
	globals.rocketMesh->setEditMode(false);
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
	rocket->takeDown();
	starfield->takeDown();
	marsTexture->takeDown();
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
	
	delete starfield;

	delete rocket;
	delete mars;

	delete marsTexture;

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
	
	if(!globals.Scenes[0]->getCurrentCameraIndex() == 1 || (deltaX == 0 && deltaY == 0) ) return;

	globals.flyCam->addAngle(-deltaX/10.0f);
	globals.flyCam->addAxisAngle(deltaY/10.0f);

	if(globals.currentScene == 0 && globals.Scenes[globals.currentScene]->getCurrentCameraIndex() == 1)
	{
		if(x <= 10 || (y) <= 10 || x >= size.x-10 || y >= size.y-10) {
			lastX = centerx;
			lastY = centery;
			glutWarpPointer( lastX, lastY );
		}
	}
	
}

void KeyboardFunc(unsigned char c, int x, int y) {
	bool normals;

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
		globals.model->reset();
		globals.rocketMesh->revertHead();
		globals.exitEditMode();
	
		break;

	case 'w':
		globals.wireframe = !globals.wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, globals.wireframe ? GL_LINE : GL_FILL);
		break;

	case 'n':
		normals = !globals.marsMesh->isDrawingNormals();
		globals.rocketMesh->setDrawNormals(normals);
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
		globals.rocketMesh->replaceHead();
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
		Graphics::inst()->setTime(time);
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
	ilInit();
	iluInit();
	ilutInit();
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
