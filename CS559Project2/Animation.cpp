#include <list>
#include "Animation.h"

using namespace std;


Animation::Animation() {
	paused = false;
	willPause = false;
	timeSpentPaused = 0;
	timePauseStarted = 0;
}

void Animation::update(int time) {
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

void RotationAnimation::doUpdate(int time) {
	obj->setRotation(axis->evaluate(time), angle->evaluate(time));
}