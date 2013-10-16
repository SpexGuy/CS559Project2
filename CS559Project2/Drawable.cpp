#include <list>
#include "Drawable.h"
#include "Graphics.h"
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

DrawableGroup::DrawableGroup() {
	elements = list<Drawable*>();
}

void DrawableGroup::draw(mat4 model) {
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
