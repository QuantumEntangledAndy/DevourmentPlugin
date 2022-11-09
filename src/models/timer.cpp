#include "models/timer.hpp"
#include "data/time.hpp"

using namespace RE;
using namespace SKSE;
using namespace Devr;

namespace {
	inline double GetGameTime() {
		return Time::WorldTimeElapsed();
	}
	inline std::uint64_t GetGameFrame() {
		return Time::FramesElapsed();
	}
}

namespace Devr {
	Timer::Timer(double delta) : delta(delta) {
	}
	bool Timer::ShouldRun() {
		double currentTime = GetGameTime();
		if (this->last_time + this->delta <= currentTime) {
			this->elaped_time = currentTime - this->last_time;
			this->last_time = currentTime;

			std::uint64_t currentFrame = GetGameFrame();

			this->elaped_frame = currentFrame - this->last_frame;
			this->last_frame = currentFrame;
			return true;
		}
		return false;
	}

	double Timer::TimeDelta() {
		return this->elaped_time;
	}

	bool Timer::ShouldRunFrame() {
		std::uint64_t currentFrame = GetGameFrame();
		if (Timer::ShouldRun()) {
			return true;
		} else if (currentFrame == this->last_frame) {
			return true;
		} else {
			return false;
		}
	}

	std::uint64_t Timer::FrameDelta() {
		return this->elaped_frame;
	}
}
