#pragma once
#include "Transformable.h"
#include <vector>
#include "Mesh.h"
#include "SplineEditor.h"

class Rocket: public TransformableGroup
{
public:
	/**
	 * Constructs a Rocket
	 */
	Rocket();
	virtual bool initialize();
	virtual void draw(const glm::mat4 &model);
	virtual void takeDown();
	inline SplineEditor *getEditor() {
		return over;
	}
	inline void setEditMode(bool editMode) {
		this->editMode = editMode;
	}
	inline bool getEditMode() {
		return editMode;
	}
	void setDrawNormals(bool normals);
	/**
	 *	Replaces the Head of the rocket with a new Mesh.
	 */
	void replaceHead();
 	~Rocket();	

protected:
	Drawable *head;
	Drawable *spline;
	SplineEditor *over;
	int stacks;
	int slices;
	bool editMode;
	float radius;
	Mesh* headMesh;

};
