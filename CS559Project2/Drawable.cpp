#include <list>
#include "Drawable.h"
#include "Graphics.h"
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

void Drawable::updateLighting(const mat4 & view, mat4 context) const {
	//the default implementation does nothing
}

DrawableGroup::DrawableGroup() {
	elements = list<Drawable*>();
}

void DrawableGroup::updateLighting(const mat4 & view, mat4 context) const {
	for (
		list<Drawable*>::const_iterator
			iterator = elements.begin(),
			end = elements.end();
		iterator != end;
		++iterator)
	{
		(*iterator)->updateLighting(view, context);
	}
}

void DrawableGroup::draw(const mat4 & view, mat4 context, const Graphics & g) const {
	for (
		list<Drawable*>::const_iterator
			iterator = elements.begin(),
			end = elements.end();
		iterator != end;
		++iterator)
	{
		(*iterator)->draw(view, context, g);
	}
}

void DrawableGroup::addElement(Drawable *el) {
	elements.push_back(el);
}

list<Drawable*> *DrawableGroup::getElements() {
	return &elements;
}
