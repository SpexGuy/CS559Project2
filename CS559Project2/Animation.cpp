/* See Animation.h for architecture comments */
#include <list>
#include "Animation.h"

using namespace std;


//----------------- Animation -------------------

Animation::Animation() {
	paused = false;
	willPause = false;
	timeSpentPaused = 0;
	timePauseStarted = 0;
}

void Animation::update(int time) {
	/* detect and handle edge on pause flag */
	if (willPause != paused) {
		if (willPause)
			timePauseStarted = time;
		else
			timeSpentPaused += time - timePauseStarted;
		paused = willPause;
	}
	if (!paused)
		doUpdate(time - timeSpentPaused);
}

void Animation::pause() {
	willPause = true;
}

void Animation::play() {
	willPause = false;
}

void Animation:: reset() {
	//trigger a positive edge
	paused = true;
	/* upon positive edge,
	 * timeSpentPaused = time + timeSpentPaused - timePauseStarted
	 * by setting timePauseStarted equal to timeSpentPaused,
	 * this becomes timeSpentPaused = time,
	 * effectively resetting the system. */
	timeSpentPaused = timePauseStarted;
}


//-------------- AnimationGroup ---------------

void AnimationGroup::reset(){
	Animation::reset();
	for (
		list<Animation *>::const_iterator
			iterator = elements.begin(),
			end = elements.end();
		iterator != end;
		++iterator)
	{
		(*iterator)->reset();
	}
}

void AnimationGroup::doUpdate(int time) {
	for (
		list<Animation *>::const_iterator
			iterator = elements.begin(),
			end = elements.end();
		iterator != end;
		++iterator)
	{
		(*iterator)->update(time);
	}
}

AnimationGroup *AnimationGroup::addAnimation(Animation *el) {
	elements.push_back(el);
	return this;
}

void AnimationGroup::clearAnimations() {
	elements.clear();
}

AnimationGroup::~AnimationGroup() {
	for (
		list<Animation *>::const_iterator
			iterator = elements.begin(),
			end = elements.end();
		iterator != end;
		++iterator)
	{
		delete *iterator;
	}
}


//--------------- RotationAnimation ---------------

void RotationAnimation::doUpdate(int time) {
	obj->setRotation(axis->evaluate(time), angle->evaluate(time));
}
