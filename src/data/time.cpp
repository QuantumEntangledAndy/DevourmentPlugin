#include "data/time.hpp"

using namespace SKSE;
using namespace RE;

namespace {
	inline static float* g_delta_time = (float*)REL::RelocationID(523660, 410199).address();
}

namespace Devr {
	Time& Time::GetSingleton() noexcept {
		static Time instance;
		return instance;
	}

	float Time::WorldTimeDelta() {
		return (*g_delta_time);
	}
	double Time::WorldTimeElapsed() {
		return Time::GetSingleton().worldTimeElapsed;
	}

	std::uint64_t Time::FramesElapsed() {
		return Time::GetSingleton().framesElapsed;
	}

	void Time::Update() {
		this->framesElapsed += 1;
		this->worldTimeElapsed += Time::WorldTimeDelta();
	}
}
