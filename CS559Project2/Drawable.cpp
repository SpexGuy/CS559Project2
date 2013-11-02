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



Drawable *Drawable::rotated(const vec3 &axis, const float &angle) {
	Rotation *d = new Rotation(this);
	d->setRotation(axis, angle);
	return d;
}

Drawable *Drawable::translated(const vec3 &position) {
	Translation *d = new Translation(this);
	d->position(position);
	return d;
}

Drawable *Drawable::scaled(const vec3 &scale) {
	Scale *d = new Scale(this);
	d->scale(scale);
	return d;
}

Drawable *Drawable::disableDepthTest() {
	return new DisableDepthTest(this);
}

Drawable *Drawable::inColor(vec4 color) {
	return new Color(this, color);
}

Drawable *Drawable::inMaterial(float a, float d, float s, float shiny) {
	return new Material(this, a, d, s, shiny);
}

Drawable *Drawable::resetColor() {
	return new ColorReset(this);
}

Drawable *Drawable::resetMaterial() {
	return new MaterialReset(this);
}

Drawable *Drawable::animateRotation(AnimationGroup *ag, TimeFunction<glm::vec3> *axis, TimeFunction<float> *angle) {
	Rotation *d = new Rotation(this);
	ag->addAnimation(new RotationAnimation(d, axis, angle));
	return d;
}

Drawable *Drawable::store(Drawable *&bucket) {
	bucket = this;
	return this;
}




Drawable *DrawableDecorator::rotated(const vec3 &axis, const float &angle) {
	Drawable *d = child->rotated(axis, angle);
	if (d != child)
		isTos = false;
	child = d;
	return this;
}

Drawable *DrawableDecorator::translated(const vec3 &position) {
	Drawable *d = child->translated(position);
	if (d != child)
		isTos = false;
	child = d;
	return this;
}

Drawable *DrawableDecorator::scaled(const vec3 &scale) {
	Drawable *d = child->scaled(scale);
	if (d != child)
		isTos = false;
	child = d;
	return this;
}

Drawable *DrawableDecorator::animateRotation(AnimationGroup *ag, TimeFunction<vec3> *axis, TimeFunction<float> *angle) {
	Drawable *d = child->animateRotation(ag, axis, angle);
	if (d != child)
		isTos = false;
	child = d;
	return this;
}

Drawable *DrawableDecorator::disableDepthTest() {
	Drawable *d = child->disableDepthTest();
	if (d != child)
		isTos = false;
	child = d;
	return this;
}

Drawable *DrawableDecorator::inColor(vec4 color) {
	Drawable *d = child->inColor(color);
	if (d != child)
		isTos = false;
	child = d;
	return this;
}

Drawable *DrawableDecorator::inMaterial(float a, float d, float s, float shiny) {
	Drawable *dec = child->inMaterial(a, d, s, shiny);
	if (dec != child)
		isTos = false;
	child = dec;
	return this;
}

Drawable *DrawableDecorator::resetColor() {
	Drawable *d = child->resetColor();
	if (d != child)
		isTos = false;
	child = d;
	return this;
}

Drawable *DrawableDecorator::resetMaterial() {
	Drawable *d = child->resetMaterial();
	if (d != child)
		isTos = false;
	child = d;
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
	Graphics::inst()->setMaterial(a, d, s, shiny);
	child->draw(model);
}

void ColorReset::draw(const mat4 &model) {
	vec4 color = Graphics::inst()->getColor();
	child->draw(model);
	Graphics::inst()->setColor(color);
}

void MaterialReset::draw(const mat4 &model) {
	float a = Graphics::inst()->getAmbient();
	float d = Graphics::inst()->getDiffuse();
	float s = Graphics::inst()->getSpecular();
	float shiny = Graphics::inst()->getShininess();
	child->draw(model);
	Graphics::inst()->setMaterial(a, d, s, shiny);
}
