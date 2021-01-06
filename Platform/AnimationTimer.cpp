#include "AnimationTimer.hpp"
#include <cmath>

AnimationTimer::AnimationTimer(int max_timer) {
	this->max_timer = abs(max_timer);
	this->timer = 0;
}

int AnimationTimer::getMaxTimer() {
	return max_timer;
}

void AnimationTimer::reset() {
	this->timer = 0;
}


void AnimationTimer::incTimer() {
	this->timer++;
	if (this->timer > this->max_timer) {
		this->timer = 0;
	}
}

bool AnimationTimer::limit() {
	return this->timer >= this->max_timer;
}