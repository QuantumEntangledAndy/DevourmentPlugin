#include "data/config.hpp"
// Config file that is loaded a init
#include <articuno/archives/ryml/ryml.h>

using namespace articuno::ryml;

namespace Devr {
	const Config& Config::GetSingleton() noexcept {
		static Config instance;

		static std::atomic_bool initialized;
		static std::latch latch(1);
		if (!initialized.exchange(true)) {
			std::ifstream inputFile(R"(Data\SKSE\Plugins\DevrPlugin.yaml)");
			if (inputFile.good()) {
				yaml_source ar(inputFile);
				ar >> instance;
			}
			latch.count_down();
		}
		latch.wait();

		return instance;
	}
}
