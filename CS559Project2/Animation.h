#pragma once
#include <list>
#include "Function.h"
#include "Rotatable.h"

/** a base class for an animation.
 * it encapsulates the pause functionality. */
class Animation {
protected:
	bool paused;
	bool willPause;
	int timeSpentPaused;
	int timePauseStarted;

	/* a virtual update function
	 * for subclasses to override */
	virtual void doUpdate(int time) = 0;
public:
	/** starts the animation */
	Animation();

	/** alters the time based on pause functionality
	 * then calls doUpdate() */
	void update(int time);

	/** pauses the animation */
	void pause();
	/** continues the animation */
	void play();
	/** resets the animation */
	virtual void reset();

	virtual ~Animation() {}

	inline bool isPaused() { return paused; }
};

/**
 * An Animation which aggregates Animations
 */
class AnimationGroup : public Animation {
private:
	std::list<Animation *> elements;
protected:
	/* updates all animations in the list */
	virtual void doUpdate(int time);

public:
	/* adds a drawable to the end of the list */
	AnimationGroup *addAnimation(Animation *a);

	/* recursively resets children */
	virtual void reset();

	/* clears the list */
	void clearAnimations();

	/* returns a pointer to the list */
	inline std::list<Animation *> *getAnimations() {
		return &elements;
	}
	
	/* also deletes children */
	virtual ~AnimationGroup();
};

/** An animation to be applied to a Rotatable.
 * It uses TimeFunctions to define the path of motion. */
class RotationAnimation : public Animation {
private:
	RotationAnimation();
protected:
	Rotatable *obj;
	TimeFunction<glm::vec3> *axis;
	TimeFunction<float> *angle;
public:
	RotationAnimation(Rotatable *r, TimeFunction<glm::vec3> *axis, TimeFunction<float> *angle)
		: obj(r), axis(axis), angle(angle) {}

	virtual void doUpdate(int time);
};