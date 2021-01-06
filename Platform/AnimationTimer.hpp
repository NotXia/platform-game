#pragma once

class AnimationTimer {
	protected:
		int timer;
		int max_timer;

	public:
		AnimationTimer(int max_timer=1000);
		
		int getMaxTimer();

		void reset();
		void incTimer();
		bool limit();
};

