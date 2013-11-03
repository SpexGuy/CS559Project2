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

Drawable *Drawable::billboard(const vec3 &axis) {
	return new BillboardTransform(this, axis);
}

Drawable *Drawable::disableDepthTest() {
	return new DisableDepthTest(this);
}

Drawable *Drawable::inColor(vec4 color) {
	return new Color(this, color);
}

Drawable *Drawable::inMaterial(const float &a, const vec4 &s, const float &shiny) {
	return new Material(this, a, s, shiny);
}

Drawable *Drawable::resetColor() {
	return new ColorReset(this);
}

Drawable *Drawable::resetMaterial() {
	return new MaterialReset(this);
}

Drawable *Drawable::useMVMode(int mode) {
	return new ModelviewMode(this, mode);
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

Drawable *DrawableDecorator::billboard(const vec3 &axis) {
	Drawable *d = child->billboard(axis);
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

Drawable *DrawableDecorator::inMaterial(const float &a, const vec4 &s, const float &shiny) {
	Drawable *dec = child->inMaterial(a, s, shiny);
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

Drawable *DrawableDecorator::useMVMode(int mode) {
	Drawable *d = child->useMVMode(mode);
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
	Graphics::inst()->setMaterial(ambient, specularColor, shininess);
	child->draw(model);
}

void ColorReset::draw(const mat4 &model) {
	vec4 color = Graphics::inst()->getColor();
	child->draw(model);
	Graphics::inst()->setColor(color);
}

void MaterialReset::draw(const mat4 &model) {
	float a = Graphics::inst()->getAmbient();
	vec4 spec = Graphics::inst()->getSpecularColor();
	float shiny = Graphics::inst()->getShininess();
	child->draw(model);
	Graphics::inst()->setMaterial(a, spec, shiny);
}

/** this algorithm is adapted from
 * http://nehe.gamedev.net/article/billboarding_how_to/18011/#5
 */
void BillboardTransform::draw(const mat4 &model) {
	//convert everything to modelspace
	vec4 cameraPos = inverse(Graphics::inst()->getView()) * vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 pos = model * vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec3 axis = normalize(vec3(model * vec4(this->axis, 0.0f)));
	
	//find the appropriate orthogonal basis
	vec3 look = normalize(vec3(cameraPos-pos));	//calculate the look vector
	vec3 right = cross(vec3(axis), look);	//right = up x look
	if (right != vec3(0.0f))				//handle special case: up = look
		right = normalize(right);
	look = cross(right, axis);				//calculate the orthogonal look vector

	//build a transformation matrix
	mat4 transform;
	transform[0] = vec4(right.x, axis.x, look.x, pos.x);
	transform[1] = vec4(right.y, axis.y, look.y, pos.y);
	transform[2] = vec4(right.z, axis.z, look.z, pos.z);
	transform[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	transform = transpose(transform);
	//transform inherits information from model, and therefore supercedes it
	child->draw(transform);
}

void ModelviewMode::draw(const mat4 &model) {
	int oldMode = Graphics::inst()->getModelviewMode();
	Graphics::inst()->setModelviewMode(mode);
	child->draw(model);
	Graphics::inst()->setModelviewMode(oldMode);
}
