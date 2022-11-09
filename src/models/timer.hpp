#pragma once

namespace Devr {
	class Timer {
		public:
			Timer(double delta);
			bool ShouldRun();
			double TimeDelta();
			bool ShouldRunFrame();
			std::uint64_t FrameDelta();
		private:
			double delta = 0.01666;

			double last_time = 0.0;
			double elaped_time = 0.0;

			std::uint64_t last_frame = 0;
			std::uint64_t elaped_frame = 0;
	};
}
