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

	/**
	 * Initializes the  Rocket
	 */
	virtual bool initialize();

	/**
	 * Draws the rocket based on the model matrix.
	 */
	virtual void draw(const glm::mat4 &model);

	/**
	 * Frees GL resources.
	 */
	virtual void takeDown();

	inline SplineEditor *getEditor() {
		return over;
	}
	inline void setEditMode(bool editMode) {
		this->editMode = editMode;
	}
	/**
	 * Returns if the rocket's head can be edited.
	 */
	inline bool getEditMode() {
		return editMode;
	}
	/**
	 * Sets if the normals should be drawn.
	 */
	void setDrawNormals(bool normals);
	/**
	 *	Replaces the head of the rocket with a new Mesh.
	 */
	void replaceHead();

	/*
	 * Replaces the head of the rocket to the oringonal sphere. 
	 */
	void revertHead();

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
