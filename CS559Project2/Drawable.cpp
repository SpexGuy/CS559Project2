/** See Drawable.h for architecture documentation */
#include <list>
#include "Drawable.h"
#include "Graphics.h"
#include "Rotatable.h"
#include "Scaleable.h"
#include "Translatable.h"
#include "Animation.h"
#include <glm/glm.hpp>

using namespace glm;
using namespace std;


//--------------- Drawable --------------
Drawable *Drawable::pushDecorator(DrawableDecorator *d) {
	d->setChild(this);
	return d;
}

Drawable *Drawable::store(Drawable *&bucket) {
	bucket = this;
	return this;
}

//		----- Decorator Convenience Functions ------
Drawable *Drawable::rotated(const vec3 &axis, const float &angle) {
	Rotation *d = new Rotation();
	d->setRotation(axis, angle);
	return pushDecorator(d);
}

Drawable *Drawable::translated(const vec3 &position) {
	Translation *d = new Translation();
	d->position(position);
	return pushDecorator(d);
}

Drawable *Drawable::scaled(const vec3 &scale) {
	Scale *d = new Scale();
	d->scale(scale);
	return pushDecorator(d);
}

Drawable *Drawable::animateRotation(AnimationGroup *ag, TimeFunction<glm::vec3> *axis, TimeFunction<float> *angle) {
	Rotation *d = new Rotation();
	ag->addAnimation(new RotationAnimation(d, axis, angle));
	return pushDecorator(d);
}

Drawable *Drawable::disableDepthTest() {
	return pushDecorator(new DisableDepthTest());
}

Drawable *Drawable::inColor(vec4 color) {
	return pushDecorator(new Color(color));
}

Drawable *Drawable::inMaterial(const float &a, const vec4 &s, const float &shiny) {
	return pushDecorator(new Material(a, s, shiny));
}

Drawable *Drawable::resetColor() {
	return pushDecorator(new ColorReset());
}

Drawable *Drawable::resetMaterial() {
	return pushDecorator(new MaterialReset());
}

Drawable *Drawable::billboard(const vec3 &axis) {
	return pushDecorator(new BillboardTransform(axis));
}

Drawable *Drawable::useMVMode(int mode) {
	return pushDecorator(new ModelviewMode(mode));
}



//---------------- DrawableDecorator ---------------

Drawable *DrawableDecorator::pushDecorator(DrawableDecorator *dec) {
	Drawable *d = child->pushDecorator(dec);
	if (d != child) {
		isTos = false;
		child = d;
	}
	return this;
}

Drawable *DrawableDecorator::store(Drawable *&bucket) {
	if (isTos) {
		bucket = this;
	} else {
		child->store(bucket);
	}
	return this;
}

bool DrawableDecorator::initialize() {
	return child->initialize();
}

void DrawableDecorator::takeDown() {
	child->takeDown();
}

DrawableDecorator::~DrawableDecorator() {
	if (child != NULL)
		delete child;
}


//----------------- DrawableGroup ------------------

DrawableGroup::DrawableGroup() {
	elements = list<Drawable*>();
}

void DrawableGroup::draw(const mat4 &model) {
	for (
		list<Drawable*>::const_iterator
			iterator = elements.begin(),
			end = elements.end();
		iterator != end;
		++iterator)
	{
		(*iterator)->draw(model);
	}
}

bool DrawableGroup::initialize() {
	for (
		list<Drawable*>::const_iterator
			iterator = elements.begin(),
			end = elements.end();
		iterator != end;
		++iterator)
	{
		if (!(*iterator)->initialize())
			return false;
	}
	return true;
}

void DrawableGroup::takeDown() {
	for (
		list<Drawable*>::const_iterator
			iterator = elements.begin(),
			end = elements.end();
		iterator != end;
		++iterator)
	{
		(*iterator)->takeDown();
	}
}

void DrawableGroup::addLight(Drawable *light) {
	elements.push_front(light);
}

void DrawableGroup::addElement(Drawable *el) {
	elements.push_back(el);
}

void DrawableGroup::clearElements() {
	elements.clear();
}

list<Drawable *> *DrawableGroup::getElements() {
	return &elements;
}


//------------ Decorator Implementations ---------------

void DisableDepthTest::draw(const mat4 &model) {
	glDisable(GL_DEPTH_TEST);
	child->draw(model);
	glEnable(GL_DEPTH_TEST);
}

void Color::draw(const mat4 &model) {
	Graphics::inst()->setColor(color);
	child->draw(model);
}

void Material::draw(const mat4 &model) {
	Graphics::inst()->setMaterial(ambient, specularColor, shininess);
	child->draw(model);
}

void ColorReset::draw(const mat4 &model) {
	//save the old color
	vec4 color = Graphics::inst()->getColor();
	//continue down the stack
	child->draw(model);
	//restore the old color
	Graphics::inst()->setColor(color);
}

void MaterialReset::draw(const mat4 &model) {
	//save the old material
	float a = Graphics::inst()->getAmbient();
	vec4 spec = Graphics::inst()->getSpecularColor();
	float shiny = Graphics::inst()->getShininess();
	//continue down the stack
	child->draw(model);
	//restore the old material
	Graphics::inst()->setMaterial(a, spec, shiny);
}

/** this algorithm is adapted from
 * http://nehe.gamedev.net/article/billboarding_how_to/18011/
 */
void BillboardTransform::draw(const mat4 &model) {
	//convert camera position to local coordinate space
	vec4 cameraPos = inverse(Graphics::inst()->getView() * model) * vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	//find the appropriate orthogonal basis
	vec3 look = normalize(vec3(cameraPos));	//calculate the look vector
	vec3 right = cross(vec3(axis), look);	//right = up x look
	if (right != vec3(0.0f))				//handle special case: up = look
		right = normalize(right);
	look = cross(right, axis);				//calculate the orthogonal look vector

	//build a transformation matrix
	mat4 transform;
	transform[0] = vec4(     right      , 0.0f);
	transform[1] = vec4(      axis      , 0.0f);
	transform[2] = vec4(      look      , 0.0f);
	transform[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	//convert back to modelspace
	child->draw(model * transform);
}

void ModelviewMode::draw(const mat4 &model) {
	int oldMode = Graphics::inst()->getModelviewMode();
	Graphics::inst()->setModelviewMode(mode);

	child->draw(model);
	
	Graphics::inst()->setModelviewMode(oldMode);
}
