#pragma once
// Module that holds data that is loaded at runtime
// This includes various forms

using namespace std;
using namespace SKSE;
using namespace RE;

namespace Devr {
	class Time {
		public:
			static float WorldTimeDelta();
			static double WorldTimeElapsed();
			static std::uint64_t FramesElapsed();
			[[nodiscard]] static Time& GetSingleton() noexcept;
			void Update();
		private:
			double worldTimeElapsed = 0.0;
			std::uint64_t framesElapsed = 0.0;
	};
}
