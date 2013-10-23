#pragma once
#include <list>
#include "Function.h"
#include "Rotatable.h"

class Animation {
protected:
	bool paused;
	bool willPause;
	int timeSpentPaused;
	int timePauseStarted;
	virtual void doUpdate(int time) = 0;
public:
	Animation();
	void update(int time);
	void pause();
	void play();

	inline bool isPaused() { return paused; }
};

/**
 * An Animation which aggregates Animations
 */
class AnimationGroup : public Animation {
protected:
	/* updates all animations in the list */
	virtual void doUpdate(int time);

public:
	/* adds a drawable to the end of the list */
	AnimationGroup *addAnimation(Animation *a);

	/* clears the list */
	void clearAnimations();

	/* returns a pointer to the list */
	inline std::list<Animation *> *getAnimations() {
		return &elements;
	}

private:
	std::list<Animation *> elements;
};

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